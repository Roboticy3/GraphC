
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 100
#define G_MAX_EDGES 4590 * 2


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

    int cycle_found = 0;
    array cycle_buffer = {0};
    cycle_buffer.block = malloc(sizeof(size_t) * g.order);
    cycle_buffer.capacity = sizeof(size_t) * g.order;
    for (size_t i = 0; i < g.order; i++) {
      Neighbor* n = g.neighborhoods[i];
      while (n) {
        PairEdge e = {i, n->neighbor};
        //printf("checking cycle at (%ld %ld)\n", e.left, e.right);
        cycle_found = (get_cycle(f, e, &cycle_buffer) == 0);

        if (cycle_found) {
          //printf("cycle found: ");
          //print_array(cycle_buffer, sizeof(size_t));
          break;
        }

        n = n->next;
      }
      if (cycle_found) break;
    }

    if (cycle_buffer.length > sizeof(size_t)) {
      NeighborhoodGraphMinor m = {g, malloc(sizeof(size_t) * g.order)};
      for (int i = 0; i < g.order; i++) {
        m.hierarchy[i] = -1;
      }

      contract((size_t*)cycle_buffer.block, cycle_buffer.length / sizeof(size_t) - 1, &m);

      //print_hierarchy(m);

      free(m.hierarchy);
    }

    free(cycle_buffer.block);
  }

  fnDelocXSR(random_state);

  
}
