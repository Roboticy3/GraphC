
#include <programs/maxmatching.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 6
#define G_MAX_EDGES 15

#define EDGES G_MAX_EDGES * 4

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

int maxmatching() {
  float p = 0.0;

  srand(time(NULL));

  while (1) {
  //create a graph with random edges by the something-something erdos method
  size_t neighborhoods[G_ORDER] = { [ 0 ... (G_ORDER - 1) ] = -1 };
  size_t edges[EDGES] = { [ 0 ... (G_MAX_EDGES - 1) ] = -1 };

  printf("\nprobability %f: \n\n", p);
  fill_graph_random(neighborhoods, G_ORDER, edges, G_MAX_EDGES, p);
  
  print_graph(neighborhoods, G_ORDER, edges);

  p += 0.000001;
  }
}