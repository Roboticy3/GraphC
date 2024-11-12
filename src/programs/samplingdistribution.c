#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>

#include <graph/graph.h>
#include <combinations/xoshiro.h>
#include <sample/sample.h>

#define SAMPLES_PER_THREAD 10
#define THREAD_COUNT 10

#define RANGE {-0, 100, 1, 0.0, 1.0, 0.01};

int sample_range_thread(void* t_data) {

  binomialrangedata s = *(binomialrangedata*)t_data;

  sample_range(s);

  return 0;
}

int samplingdistribution(int argc, char** argv) {

  // Debugging: Print values for vertical_steps, horizontal_steps, t_range, t_chunk_size
  //printf("Vertical steps: %zu, Horizontal steps: %zu\n", vertical_steps, horizontal_steps);
  //printf("Thread range per thread (order_steps x p_steps): (%zu x %zu)\n", t_range.order_steps, t_range.p_steps);
  //printf("Total chunk size (order_steps * p_steps): %zu\n", t_chunk_size);

  // Initialize the xoshiro generator (XSR) with a seed (using current time as a seed)
  
  srand(time(NULL));

  thrd_t threads[THREAD_COUNT];
  mtx_t mutex;
  mtx_init(&mutex, mtx_plain);

  binomialrange total_range = RANGE;

  for (int t = 0; t < THREAD_COUNT; t++) {
    binomialrangedata s = {rand(), total_range, SAMPLES_PER_THREAD, THREAD_COUNT};
    thrd_create(&threads[t], sample_range_thread, &s);
  }

  for (int t = 0; t < THREAD_COUNT; t++) {
    thrd_join(threads[t], NULL);  // Ensure all threads are finished
  }

  mtx_destroy(&mutex);

  return 0;
}
