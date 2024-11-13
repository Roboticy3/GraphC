
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3_helpers/batcher.h>

//thanks chatgpt
//...well, there was a little help from me. Original version caused stack smashing up the wazoo. strcat is wack

struct insert_i_generator_bind {
  const char* column_name;
} typedef insert_i_generator_bind;

// Function to generate the insert statement based on the row index i
void insert_i(size_t i, const char *table_name, char *insert_sql, void* args) {
    const char* column_name = (*(insert_i_generator_bind*)args).column_name;
    // Create the insert SQL for the current row
    snprintf(insert_sql, MAX_INSERT_LEN, "\tINSERT INTO %s (%s) VALUES (%zu);\n", table_name, column_name, i);
}

// Function to insert rows into a table in batches
int iterator_batch(sqlite3 *db, const char *table_name, size_t num_rows, void (*generator)(size_t, const char*, char*, void*), void* args) {
    char sql[MAX_QUERY_LEN] = {0};  // Buffer for the insert statements
    char insert_sql[256];     // Temporary buffer for a single insert statement
    int db_err;
    char *err_msg = NULL;
    size_t query_length = 0;  // Tracks the length of the accumulated query

    // Start the transaction
    db_err = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &err_msg);
    if (db_err != SQLITE_OK) {
        printf("Failed to begin transaction: %s\n", err_msg);
        sqlite3_free(err_msg);
        return db_err;
    }

    // Accumulate insert statements in batches
    for (size_t i = 0; i < num_rows; i++) {
        // Construct the insert statement for the current row
        generator(i, table_name, insert_sql, args);

        // Check if adding this insert would exceed the max query length
        if (query_length + strlen(insert_sql) + 1 > MAX_QUERY_LEN) {
            // Execute the current batch of inserts
            db_err = sqlite3_exec(db, sql, 0, 0, &err_msg);
            if (db_err != SQLITE_OK) {
                printf("Failed to execute batch \"\n%s\n\" with error \"%s\" (error code %d).\n", sql, err_msg, db_err);
                sqlite3_free(err_msg);
                sqlite3_exec(db, "ROLLBACK;", 0, 0, &err_msg);
                return db_err;
            }

            // Reset the query length and start a new batch
            query_length = 0;
            memset(sql, 0, MAX_QUERY_LEN);  // Clear the buffer
        }

        strcat(sql, insert_sql);  // Add the current insert statement
        query_length += strlen(insert_sql);  // Update the current query length
    }

    // Execute any remaining rows in the final batch
    if (query_length > 0) {
        db_err = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (db_err != SQLITE_OK) {
            printf("Failed to execute remaining rows \"\n%s\n\" with error \"%s\" (error code %d).\n", sql, err_msg, db_err);
            sqlite3_free(err_msg);
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err_msg);
            return db_err;
        }
    }

    // Commit the transaction
    db_err = sqlite3_exec(db, "COMMIT;", 0, 0, &err_msg);
    if (db_err != SQLITE_OK) {
        printf("Failed to commit transaction: %s\n", err_msg);
        sqlite3_free(err_msg);
        return db_err;
    }

    printf("Successfully executed %zu commands.\n", num_rows);
    return SQLITE_OK;
}

int insert_rows_batch(sqlite3 *db, const char *table_name, const char* column_name, size_t num_rows) {
  insert_i_generator_bind bind = {column_name};
  return iterator_batch(db, table_name, num_rows, insert_i, &bind);
}