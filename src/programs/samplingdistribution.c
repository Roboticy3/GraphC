#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>

#include <graph/graph.h>

#define CHUNK_SIZE 100000
#define NUM_THREADS 16

#define ORDER 10
#define E_P 0.25

struct sampledata {
  int id;
  size_t chunk_size;
  double mean;
};

// Function to calculate the edge count of a graph
size_t edge_count(NeighborhoodGraph g) {
    return g.edges;
}

// Thread function to perform partial sampling
int sample(void* args) {
    struct sampledata* t_data = (struct sampledata*)args;

    size_t* out = malloc(t_data->chunk_size * sizeof(size_t));

    srand(time(NULL) + t_data->id);  // Seed each thread differently
    binomial_graph_random_sample(t_data->chunk_size, ORDER, E_P, edge_count, out);

    double mean = 0.0;
    for (int i = 0; i < t_data->chunk_size; i++) {
      mean += (double)out[i] / (double)t_data->chunk_size;
    }
    t_data->mean = mean;

    free(out);
    
    return 0;
}

int samplingdistribution(int argc, char** argv) {
    srand(time(NULL));
    
    // Array to hold threads
    thrd_t threads[NUM_THREADS];
    struct sampledata t_data[NUM_THREADS];

    // Create threads and assign sampling work
    for (int i = 0; i < NUM_THREADS; i++) {
      struct sampledata t = {i, CHUNK_SIZE};
      t_data[i] = t;
      if (thrd_create(&threads[i], sample, &t_data[i]) != thrd_success) {
        fprintf(stderr, "Error creating thread %d\n", i);
        return 1;
      }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
      thrd_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
      struct sampledata t = t_data[i];
      printf("%lf, ", t.mean);
    }
    printf("\n");
    return 0;
}
