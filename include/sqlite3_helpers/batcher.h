
#ifndef SQLITE3_BATCHER_H_
#define SQLITE3_BATCHER_H_

#include <sqlite3.h>
#include <stddef.h>

#define MAX_QUERY_LEN 8192    // Maximum query length
#define MAX_BATCH_SIZE 1000   // Max number of rows to insert per batch
#define MAX_INSERT_LEN 128

int iterator_batch(sqlite3 *db, const char *table_name, size_t num_rows, void (*generator)(size_t, const char*, char*, void*), void* args);

int insert_rows_batch(sqlite3 *db, const char *table_name, const char *column_name, size_t num_rows);

#endif