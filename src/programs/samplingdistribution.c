#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>

#include <graph/graph.h>
#include <combinations/xoshiro.h>

#define SAMPLE_CHUNK_SIZE 2

struct sampledata {
  size_t order;
  float p;
  size_t chunk_size;
  pXSR random_state;
  double mean;
} typedef sampledata;

struct binomialrange {
  int id;
  size_t order_min;
  size_t order_max;
  size_t order_step;
  float p_min;
  float p_max;
  float p_step;
  sampledata* out;
  size_t out_range;
  int seed;
} typedef binomialrange;

struct binomialrangethreadsize {
  size_t order_steps;
  size_t p_steps;
} typedef binomialrangethreadsize;

#define RANGE {-1, 0, 100, 1, 0.0, 1.0, 0.1};

#define START_BRL(range, i, j, o, p) \
  o = range.order_min; for (size_t i = 0; o < range.order_max; i++) { \
  p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {

#define END_BRL(range, i, j, o, p) \
  p += range.p_step;}\
  o += range.order_step;}

void print_binomial_range_to_csv(binomialrange range, binomialrange total_range) {
  size_t row_length = (total_range.p_max - total_range.p_min) / total_range.p_step;
  size_t row_start = (range.order_min - total_range.order_min) / total_range.order_step;
  size_t col_start = (range.p_min - total_range.p_min) / range.p_step;

  size_t o;
  float p;

  printf(",");
  p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {
    printf("%f, ", p);
  p += range.p_step;}
  printf("\n");

  o = range.order_min; for (size_t i = 0; o < range.order_max; i++) {
  printf("%lu, ", o); p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {

    size_t index = row_length * (row_start + i) + col_start + j;

    printf("%lf, ", total_range.out[index].mean); 

  p += range.p_step;}
  printf("\n"); o += range.order_step;}
}

// Function to calculate the edge count of a graph
size_t edge_count(NeighborhoodGraph g) {
    return g.edges;
}

// Thread function to perform partial sampling
int sample_point(sampledata* t_data) {

    size_t* out = malloc(t_data->chunk_size * sizeof(size_t));

    binomial_graph_random_sample(t_data->chunk_size, t_data->order, t_data->p, edge_count, out, t_data->random_state);

    double mean = 0.0;
    for (int i = 0; i < t_data->chunk_size; i++) {
      //printf("\t\tout (n:%ld, p:%f): %ld\n", t_data->order, t_data->p, out[i]);
      mean += (double)out[i] / (double)t_data->chunk_size;
    }
    t_data->mean = mean;

    free(out);
    
    return 0;
}

int thread_range(void* args) {
  binomialrange range = ((binomialrange*)args)[0];
  binomialrange total_range = ((binomialrange*)args)[1];

  size_t row_length = (total_range.p_max - total_range.p_min) / total_range.p_step;
  size_t row_start = (range.order_min - total_range.order_min) / total_range.order_step;
  size_t col_start = (range.p_min - total_range.p_min) / range.p_step;

  // Initialize the xoshiro generator (XSR) with a seed (using current time as a seed)
  srand(range.seed);
  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(rand(), paramA, paramB);  // Initialize XSR with a seed

  //printf("searching range %ld %ld %f %f (index range %ld to %ld out of %ld):\n", range.order_min, range.order_max, range.p_min, range.p_max, row_length * range.order_min / range.order_step + (size_t)(range.p_min / range.p_step), row_length * ((range.order_max / range.order_step) - 1) + (size_t)(range.p_max / range.p_step) - 1, total_range.out_range);
  size_t o;
  float p;
  START_BRL(range, i, j, o, p)

      sampledata s = {
        o, p, SAMPLE_CHUNK_SIZE, random_state
      };
      sample_point(&s);
      size_t index = row_length * (row_start + i) + col_start + j;
      total_range.out[index] = s;

      //printf("\t sampling position (%lu %f) into (%lu %lu) with mean %f\n", o, p, i, j, s.mean); 

  END_BRL(range, i, j, o, p)
  

  //de initialize the random numbers for this thread
  fnDelocXSR(random_state);
}

#define NUM_THREADS_VERTICAL 1
#define NUM_THREADS_HORIZONTAL 1
#define NUM_THREADS NUM_THREADS_VERTICAL * NUM_THREADS_HORIZONTAL

int samplingdistribution(int argc, char** argv) {

  binomialrange total_range = RANGE;
  size_t vertical_steps = (total_range.order_max - total_range.order_min) / total_range.order_step;
  size_t horizontal_steps = (total_range.p_max - total_range.p_min) / total_range.p_step;
  total_range.out_range = vertical_steps * horizontal_steps;

  //printf("total range: %lu %lu %f %f\n", total_range.order_min, total_range.order_max, total_range.p_min, total_range.p_max);
  //printf("step sizes: %lu %f\n", total_range.order_step, total_range.p_step);

  binomialrangethreadsize t_range = {vertical_steps / NUM_THREADS_VERTICAL, horizontal_steps / NUM_THREADS_HORIZONTAL};
  size_t t_chunk_size = t_range.order_steps * t_range.p_steps;

  total_range.out = (sampledata*)malloc(total_range.out_range * sizeof(sampledata));
  

  // Array to hold threads
  thrd_t threads[NUM_THREADS];
  struct sampledata t_data[NUM_THREADS];

  // Debugging: Print values for vertical_steps, horizontal_steps, t_range, t_chunk_size
  //printf("Vertical steps: %zu, Horizontal steps: %zu\n", vertical_steps, horizontal_steps);
  //printf("Thread range per thread (order_steps x p_steps): (%zu x %zu)\n", t_range.order_steps, t_range.p_steps);
  //printf("Total chunk size (order_steps * p_steps): %zu\n", t_chunk_size);

  srand(time(NULL));

  for (int i = 0; i < NUM_THREADS; i++) {
    int row = (i / NUM_THREADS_HORIZONTAL);
    int col = (i % NUM_THREADS_HORIZONTAL);

    // Debugging: Print row, col, and corresponding chunk size for this thread
    //printf("Thread %d: row %d, col %d\n", i, row, col);
    //printf("Thread %d: chunk start (order, p): (%zu, %zu)\n", i, row * t_range.order_steps, col * t_range.p_steps);
    //printf("Thread %d: chunk end (order, p): (%zu, %zu)\n", i, (row + 1) * t_range.order_steps, (col + 1) * t_range.p_steps);

    binomialrange chunk = {
      i, 
      total_range.order_min + row * t_range.order_steps * total_range.order_step, 
      total_range.order_min + (row + 1) * t_range.order_steps,
      total_range.order_step,
      total_range.p_min + col * t_range.p_steps * total_range.p_step,
      total_range.p_min + (col + 1) * t_range.p_steps * total_range.p_step,
      total_range.p_step,
      NULL,
      t_chunk_size,
      rand()
    };

    binomialrange args[2] = {chunk, total_range}; 

    // Debugging: Print the chunk size and data pointer
    //printf("Thread %d: chunk size (steps): %zu, data pointer: %p\n", i, t_chunk_size, &sample_points[i * t_chunk_size]);

    thrd_create(&threads[i], thread_range, args);
  }

  // Wait for threads to finish
  for (int i = 0; i < NUM_THREADS; i++) {
    thrd_join(threads[i], NULL);
  }

  print_binomial_range_to_csv(total_range, total_range);

  
  free(total_range.out);
  return 0;
}
