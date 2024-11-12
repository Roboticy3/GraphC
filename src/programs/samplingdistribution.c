#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <graph/graph.h>
#include <combinations/xoshiro.h>

#define SAMPLE_CHUNK_SIZE 100

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

#define START_BRL(range, i, j, o, p) \
  o = range.order_min; for (size_t i = 0; o < range.order_max; i++) { \
  p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {

#define END_BRL(range, i, j, o, p) \
  p += range.p_step;}\
  o += range.order_step;}

#define RANGE {-1, 0, 100, 1, 0.0, 1.0, 0.01};

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

int samplingdistribution(int argc, char** argv) {

  binomialrange total_range = RANGE;
  size_t vertical_steps = (total_range.order_max - total_range.order_min) / total_range.order_step;
  size_t horizontal_steps = (total_range.p_max - total_range.p_min) / total_range.p_step;
  total_range.out_range = vertical_steps * horizontal_steps;

  //printf("total range: %lu %lu %f %f\n", total_range.order_min, total_range.order_max, total_range.p_min, total_range.p_max);
  //printf("step sizes: %lu %f\n", total_range.order_step, total_range.p_step);

  total_range.out = (sampledata*)malloc(total_range.out_range * sizeof(sampledata));

  // Debugging: Print values for vertical_steps, horizontal_steps, t_range, t_chunk_size
  //printf("Vertical steps: %zu, Horizontal steps: %zu\n", vertical_steps, horizontal_steps);
  //printf("Thread range per thread (order_steps x p_steps): (%zu x %zu)\n", t_range.order_steps, t_range.p_steps);
  //printf("Total chunk size (order_steps * p_steps): %zu\n", t_chunk_size);

  // Initialize the xoshiro generator (XSR) with a seed (using current time as a seed)
  srand(time(NULL));
  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(rand(), paramA, paramB);  // Initialize XSR with a seed

  for (int i = 0; i < total_range.out_range; i++) {
    sampledata s = {
        total_range.order_min + i / horizontal_steps * total_range.order_step, 
        total_range.p_min + i % horizontal_steps * total_range.p_step, 
        SAMPLE_CHUNK_SIZE, random_state
      };
      sample_point(&s);
      //printf("mean: %lf\n", s.mean);
      total_range.out[i] = s;
  }

  fnDelocXSR(random_state);

  FILE* file_out = fopen("out.csv", "w");
  fprintf(file_out, "order,edge probability,property\n");
  size_t o;
  float p;
  START_BRL(total_range, i, j, o, p)

    size_t index = horizontal_steps * i + j;

    fprintf(file_out, "%lu,%f,%lf\n", o,p,total_range.out[index].mean); 

  END_BRL(total_range, i, j, o, p)
  fclose(file_out);

  
  free(total_range.out);
  return 0;
}
