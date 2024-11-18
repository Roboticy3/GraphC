
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

  // Initialize the xoshiro generator (XSR) with a seed (using current time as a seed)
  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(rand(), paramA, paramB);  // Initialize XSR with a seed

  for (float p = 0.2; p <= 0.21; p += 0.05) {
    Neighbor* neighborhoods[G_ORDER] = { 0 };
    Neighbor edges[G_MAX_EDGES] = { 0 };
    NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

    //printf("probability %f: \n", p);
    printf("p: %f\n", p);
    fill_graph_binomial(&g, p, random_state);
    
    print_graph(g);

    size_t paths[G_ORDER];
    Forest f = {paths, G_ORDER};
    bfs(g, &f);

    printf("shortest path forest:\n");

    print_forest(f);
  }

  fnDelocXSR(random_state);

  
}
