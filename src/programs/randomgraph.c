
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 20
#define G_MAX_EDGES 190

#define EDGES G_MAX_EDGES * 4

/*
int randomgraph() {
  float p = 0.0;

  srand(time(NULL));

  while (p <= 1.0) {
  //create a graph with random edges by the something-something erdos method
  size_t neighborhoods[G_ORDER] = { [ 0 ... (G_ORDER - 1) ] = -1 };
  size_t edges[EDGES] = { [ 0 ... (G_MAX_EDGES - 1) ] = -1 };

  printf("\nprobability %f: \n\n", p);
  fill_graph_random(neighborhoods, G_ORDER, edges, G_MAX_EDGES, p);
  
  print_graph(neighborhoods, G_ORDER, edges);

  p += 0.00001;
  }
}
*/