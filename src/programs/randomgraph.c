
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 10
#define G_MAX_EDGES 45 * 2


int randomgraph() {

  srand(time(NULL));

  // Initialize the xoshiro generator (XSR) with a seed (using current time as a seed)
  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(rand(), paramA, paramB);  // Initialize XSR with a seed

  float p = 0.0;

  while (p <= 1.0) {
    Neighbor* neighborhoods[G_ORDER] = { 0 };
    Neighbor edges[G_MAX_EDGES] = { 0 };
    NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

    //printf("probability %f: \n", p);
    printf("p: %f\n", p);
    fill_graph_binomial(&g, p, random_state);
    
    print_graph(g);
    
    p += 0.01;
  }

  fnDelocXSR(random_state);

  
}
