
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include <graph/graph.h>

#include <programs/petersen.h>


size_t edge_count(NeighborhoodGraph g) {
  //print_graph_raw(g);
  //print_graph(g);
  return g.edges;
}

#define SAMPLE_SIZE 1000

int main(int argc, char** argv) {
  srand(time(NULL));
  size_t* out = malloc(SAMPLE_SIZE * sizeof(size_t));
  binomial_graph_random_sample(SAMPLE_SIZE, 10, 0.25, edge_count, out);

  for (int i = 0; i < SAMPLE_SIZE; i++) {
    printf("%ld,", out[i]);
  }

  free(out);
}