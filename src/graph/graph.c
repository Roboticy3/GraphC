
#include <stddef.h>

void add_neighbor(size_t* neighborhood, size_t* edges, size_t neighbor, size_t edge) {
  size_t start = neighborhood[0];
  neighborhood[0] = edge;
  edges[edge] = neighbor;
  edges[edge + 1] = start;
}

void print_graph(size_t* neighborhoods, size_t order, size_t* edges) {
  for (size_t i = 0; i < order; i++) {
    size_t n = neighborhoods[i];
    printf("[%ld]", i);
    while (n != -1) {
      printf(" -> %ld", edges[n]);
      n = edges[n + 1];
    }
    printf("\n");
  }
}