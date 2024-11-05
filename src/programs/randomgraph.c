
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 100
#define G_MAX_EDGES 4950 * 2


int randomgraph() {

  srand(time(NULL));

  float p = 0.0;

  while (p <= 1.0) {
    Neighbor* neighborhoods[G_ORDER] = { 0 };
    Neighbor edges[G_MAX_EDGES] = { 0 };
    NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

    //printf("probability %f: \n", p);
    printf("%f, ", p);
    fill_graph_random(&g, p);
    
    print_graph(g);
  }

  
}
