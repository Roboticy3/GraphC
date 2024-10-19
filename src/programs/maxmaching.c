
#include <programs/maxmatching.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define G_ORDER 6
#define G_MAX_EDGES 15

#define EDGES G_MAX_EDGES * 2

void fill_graph_random(size_t* neighborhoods, int n_len, size_t* edges, int e_len, int seed, float p) {
  srand(seed);

  int cutoff = RAND_MAX * p;

  //printf("cutoff %d\n", cutoff);
  //decide how many edges to insert into the graph
  size_t i = 0;
  size_t j = 0;
  size_t pair[2];
  while (i < e_len) {
    choose_step(n_len, 2, pair);
    int x = rand();
    if (x <= cutoff) {
      printf("attempt: %d endpoints: %d pair: %d %d\n", i, j, pair[0], pair[1]);
      add_neighbor(&(neighborhoods[pair[0]]), edges, pair[1], j);
      j += 2;
      add_neighbor(&(neighborhoods[pair[1]]), edges, pair[0], j);
      j += 2;
    }
    i++;
  }

}

int maxmatching() {
  //create a graph with random edges by the something-something erdos method
  size_t neighborhoods[G_ORDER] = { [ 0 ... (G_ORDER - 1) ] = -1 };
  size_t edges[EDGES] = { [ 0 ... (G_MAX_EDGES - 1) ] = -1 };

  fill_graph_random(neighborhoods, G_ORDER, edges, G_MAX_EDGES, time(NULL), 0.9);
  
  print_graph(neighborhoods, G_ORDER, edges);
}