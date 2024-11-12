#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>

#include <graph/graph.h>
#include <combinations/xoshiro.h>

#define SAMPLES_PER_THREAD 10

struct sampledata {
  size_t order;
  float p;
  size_t chunk_size;
  pXSR random_state;
  double mean;
} typedef sampledata;

struct binomialrange {
  size_t order_min;
  size_t order_max;
  size_t order_step;
  float p_min;
  float p_max;
  float p_step;
} typedef binomialrange;

struct threaddata {
  int id;
  mtx_t* mutex_p;
  uint64_t seed;
} typedef threaddata;

#define START_BRL(range, i, j, o, p) \
  o = range.order_min; for (size_t i = 0; o < range.order_max; i++) { \
  p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {

#define END_BRL(range, i, j, o, p) \
  p += range.p_step;}\
  o += range.order_step;}

#define RANGE {-0, 100, 1, 0.0, 1.0, 0.01};

// Function to calculate the edge count of a graph
size_t edge_count(NeighborhoodGraph g) {
    return g.edges;
}

// Thread function to perform partial sampling
int sample_point(sampledata* t_data, size_t* samples) {

    binomial_graph_random_sample(t_data->chunk_size, t_data->order, t_data->p, edge_count, samples, t_data->random_state);

    double mean = 0.0;
    for (int i = 0; i < t_data->chunk_size; i++) {
      //printf("\t\tout (n:%ld, p:%f): %ld\n", t_data->order, t_data->p, out[i]);
      mean += (double)samples[i] / (double)t_data->chunk_size;
    }
    t_data->mean = mean;
    
    return 0;
}

#define THREAD_COUNT 10

int sample_range(void* t_data) {

  threaddata s = *(threaddata*)t_data;

  binomialrange r = RANGE;
  size_t vertical_steps = (r.order_max - r.order_min) / r.order_step;
  size_t horizontal_steps = (r.p_max - r.p_min) / r.p_step;
  size_t table_size = vertical_steps * horizontal_steps;

  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(s.seed, paramA, paramB);

  //printf("total range: %lu %lu %f %f\n", total_range.order_min, total_range.order_max, total_range.p_min, total_range.p_max);
  //printf("step sizes: %lu %f\n", total_range.order_step, total_range.p_step);

  size_t* out = (size_t*)calloc(table_size, sizeof(size_t));
  size_t* samples = malloc(SAMPLES_PER_THREAD * sizeof(size_t));

  size_t stride = (r.p_max - r.p_min) / r.p_step;
  
  for (int i = 0; i < table_size; i++) {
    sampledata sample = {
      r.order_min + i / stride * r.order_step, 
      r.p_min + i % stride * r.p_step, 
      SAMPLES_PER_THREAD, random_state, 
    };
    sample_point(&sample, samples);
    //printf("mean: %lf\n", s.mean);
    out[i] += sample.mean / (double)THREAD_COUNT;
  }

  fnDelocXSR(random_state);
  free(samples);
  free(out);

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

  for (int t = 0; t < THREAD_COUNT; t++) {
    threaddata s = {-1, &mutex, rand()};
    thrd_create(&threads[t], sample_range, &s);
  }

  for (int t = 0; t < THREAD_COUNT; t++) {
    thrd_join(threads[t], NULL);  // Ensure all threads are finished
  }

  mtx_destroy(&mutex);

  return 0;
}
