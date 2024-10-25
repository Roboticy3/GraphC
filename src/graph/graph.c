
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <combinations/choose.h>

void add_neighbor(size_t* neighborhood, size_t* edges, size_t neighbor, size_t edge) {
  size_t start = neighborhood[0];
  neighborhood[0] = edge;
  edges[edge] = neighbor;
  edges[edge + 1] = start;
}

void fill_graph_random(size_t* neighborhoods, int n_len, size_t* edges, int e_len, float p) {

  int cutoff = p == 1.0 ? INT_MAX : RAND_MAX * p;

  //printf("cutoff %d\n", cutoff);
  //decide how many edges to insert into the graph
  size_t i = 0;
  size_t j = 0;
  size_t pair[2] = {1, 2};
  while (i < e_len) {
    int x = rand();
    //printf("attempt: %d endpoint: %d pair: %d %d roll: %d\n", i, j, pair[0], pair[1], x);
    if (x <= cutoff) {
      //printf("added edge!\n");
      add_neighbor(&(neighborhoods[pair[0] - 1]), edges, pair[1] - 1, j);
      j += 2;
      add_neighbor(&(neighborhoods[pair[1] - 1]), edges, pair[0] - 1, j);
      j += 2;
    }
    i ++;

    choose_step(n_len, 2, pair);
  }

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