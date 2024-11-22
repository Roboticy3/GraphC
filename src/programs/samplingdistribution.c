#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>
#include "sqlite3.h"

#include <graph/graph.h>
#include <combinations/xoshiro.h>
#include <sample/sample.h>
#include <sqlite3_helpers/batcher.h>
#include <combinations/choose.h>

struct threadmutexdata {
  sqlite3* db;
};

struct threaddata {
  int id;
  binomialrangedata b_data;
  size_t (*property)(NeighborhoodGraph g);
  mtx_t* mutex;
  struct threadmutexdata m_data;
};

#define SAMPLES_PER_THREAD (size_t)10
#define THREAD_COUNT 10

#define RANGE {1, 300, 3, 0.0, 0.025, 0.0005};

#define OUT_DB "out.db"
#define OUT_TABLE "samples"
#define OUT_COLUMNS {"order", "edge_probability", "edges"}
#define INITIAL_COLUMN "sample_id"

const char* columns[3] = OUT_COLUMNS;

struct row_update_generator_bind {
  struct threaddata t_data;
  double* out;
} typedef row_update_generator_bind;

void row_update_generator(size_t i, const char *table_name, char *insert_sql, void* args) {
  row_update_generator_bind bind = *(row_update_generator_bind*)args;
  struct threaddata t_data = bind.t_data;
  binomialrange r = t_data.b_data.range;
  double mean = bind.out[i];

  snprintf(insert_sql, MAX_INSERT_LEN, "UPDATE samples SET %s_%x = %ld, %s_%x = %f, %s_%x = %lf WHERE sample_id = %ld;", 
    columns[0], t_data.id, get_row(r, i),
    columns[1], t_data.id, get_col(r, i),
    columns[2], t_data.id, mean,
    i
  );
}

// Function to calculate the edge count of a graph
size_t default_property(NeighborhoodGraph g) {
  size_t* paths = malloc(g.order * sizeof(size_t));

  Forest f = {paths, g.order};
  bfs(g, &f);

  size_t components = 0;

  for (size_t i = 0; i < g.order; i++) {
    if (paths[i] == -1) components++;
  }

  //printf("%ld: %ld\n", g.order, components);

  free(paths);

  return components;
}

int sample_range_thread(void* any) {

  struct threaddata t_data = *(struct threaddata*)any;

  binomialrange r = t_data.b_data.range;

  size_t stride = R_STRIDE(r);
  size_t height = R_HEIGHT(r);
  
  //allocate the samples and fill them
  double* out = malloc(sizeof(double) * stride * height);

  sample_range(t_data.b_data, out, t_data.property);

  //pipe the samples to an sqlite database in the running directory
  mtx_lock(t_data.mutex);

  row_update_generator_bind bind = {t_data, out};

  sqlite3* db = t_data.m_data.db;
  char sql[128];
  
  iterator_batch(db, OUT_TABLE, stride * height, row_update_generator, &bind);

  mtx_unlock(t_data.mutex);

  free(out);
  return 0;
}

int samplingdistribution(int argc, char** argv) {

  //open the database
  sqlite3* db;
  char* db_name = OUT_DB;

  int db_err = sqlite3_open(db_name, &db);
  if (db_err != SQLITE_OK) {
    printf("failed to open database %s with code %d. Exiting...", db_name, db_err);
    return db_err;
  }

  //clear the previous readings under the same table
  char sql[MAX_QUERY_LEN];
  char* err_msg;
  snprintf(sql, MAX_QUERY_LEN, "DROP TABLE %s;", OUT_TABLE);
  printf("%s\n",sql);

  db_err = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (db_err != SQLITE_OK && db_err != 1) {
    printf("Failed to clear table with error \"%s\" (error code %d)\n", err_msg, db_err);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return db_err;
  }

  printf("Cleared previous table %s!\n", OUT_TABLE);

  snprintf(sql, MAX_QUERY_LEN, "CREATE TABLE %s (%s INTEGER, PRIMARY KEY (%s));", OUT_TABLE, INITIAL_COLUMN, INITIAL_COLUMN);
  printf("%s\n",sql);
  db_err = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (db_err != SQLITE_OK) {
    printf("Failed to initialize table with error \"%s\" (error code %d)\n", err_msg, db_err);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return db_err;
  }

  printf("Added new table %s!\n", OUT_TABLE);
  
  //initialize the stdlib random state to generate random seeds for the xoshiro generators in each thread
  srand(time(NULL));

  //initialize the threads
  thrd_t threads[THREAD_COUNT];
  mtx_t mutex;
  mtx_init(&mutex, mtx_plain);

  //the range this distribution will cover
  binomialrange r = RANGE;
  size_t stride = R_STRIDE(r);
  size_t height = R_HEIGHT(r);

  insert_rows_batch(db, OUT_TABLE, INITIAL_COLUMN, stride * height);

  //for each thread, get SAMPLES_PER_THREAD samples at each point in RANGE
  for (int t = 0; t < THREAD_COUNT; t++) {

    const char* columns[3] = OUT_COLUMNS;
    for (int c = 0; c < 3; c++) {
      
      snprintf(sql, MAX_QUERY_LEN, "ALTER TABLE %s ADD %s_%x;", OUT_TABLE,
        columns[c], t
      );
      printf("%s\n",sql);

      db_err = sqlite3_exec(db, sql, 0, 0, &err_msg);
      if (db_err != SQLITE_OK) {
        printf("Failed to execute \"%s\" with error \"%s\" (error code %d)\n", sql, err_msg, db_err);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        continue;
      }

      printf("Added new column %s_%x!\n", columns[c], t);
    }

    //give a random seed to each of the threads, otherwise the thread data is unique
    //crucially for multithreading, b_data stores no pointers
    binomialrangedata b_data = {rand(), r, SAMPLES_PER_THREAD, THREAD_COUNT};
    //the mutex section of the thread data stores only pointers, and should only be accessed under lock
    struct threadmutexdata m_data = {db};
    struct threaddata t_data = {t, b_data, default_property, &mutex, m_data};
    int err = thrd_create(&threads[t], sample_range_thread, &t_data);
    if (err) {
      printf("failed to create thread %d with error %d, lost %lu samples.\n", t, err, SAMPLES_PER_THREAD);
    }
  }

  //join the threads and clear them
  for (int t = 0; t < THREAD_COUNT; t++) {
    int err = thrd_join(threads[t], NULL);  
    if (err) {
      printf("failed to join thread %d with error %d... I'm not sure what happens now.\n", t, err);
      continue;
    }
  }
  mtx_destroy(&mutex);

  db_err = sqlite3_close(db);
  if (db_err != SQLITE_OK) {
    printf("failed to close database %s with error \"%s\" (error code: %d)... I'm not sure what happens now.\n", db_name, err_msg, db_err);
  }

  return 0;
}
