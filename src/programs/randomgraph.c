
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 20
#define G_MAX_EDGES 190 * 2


int randomgraph() {
  float p = 0.0;

  srand(time(NULL));

  while (p <= 1.0) {
    //create a graph with random edges by the something-something erdos method
    Neighbor* neighborhoods[G_ORDER] = { 0 };
    Neighbor edges[G_MAX_EDGES] = { 0 };
    NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

    printf("\nprobability %f: \n\n", p);
    fill_graph_random(&g, p);
    
    print_graph(g);

    p += 0.05;
  }

  Neighbor* neighborhoods[G_ORDER] = { 0 };
  Neighbor edges[G_MAX_EDGES] = { 0 };
  NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

  printf("\nprobability %f: \n\n", p);
  fill_graph_random(&g, 1.0);
  
  print_graph(g);
}
