// (c) 2025 Pablo Ibarz
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef SQLITE3_BATCHER_H_
#define SQLITE3_BATCHER_H_

#include <sqlite3.h>
#include <stddef.h>

#define MAX_QUERY_LEN 8192    // Maximum query length

// Perform an operation on `num_rows` rows of table `table_name` in sqlite3 
// database `db`, using `generator`.
// The arguments of `generator` are, in order:
// - current row index
// - current table name
// - the sql command buffer
// - extra args buffer
// `generator`'s job is to write to the command buffer, which is then executed
// once for each row in the table.
// Commands are batched into transactions based on MAX_QUERY_LEN, which should
// be at most the maximum query length of the sqlite3 engine.
//
// See src/programs/samplingdistribution.c for an example of this *completely*
// *necessary* madness in action.
int iterator_batch(sqlite3 *db, const char *table_name, size_t num_rows, void (*generator)(size_t, const char*, char*, void*), void* args);

#define MAX_INSERT_LEN 128

// Template for iterator batch to insert a whole lot of rows, with one 
// insertion being at most MAX_INSERT_LEN characters long.
int insert_rows_batch(sqlite3 *db, const char *table_name, const char *column_name, size_t num_rows);

#endif