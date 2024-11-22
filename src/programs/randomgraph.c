
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 7
#define G_MAX_EDGES 21 * 2


int randomgraph() {

  char asserterA[8192];
  char asserterB[8192];

  srand(time(NULL) + (int)(&asserterA));

  // Initialize the xoshiro generator (XSR) with a seed (using current time as a seed)
  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(rand(), paramA, paramB);  // Initialize XSR with a seed

  int keep_going = 1;
  float p = 0.1;

  

  while (keep_going && p <= 1.0) {
    Neighbor* neighborhoods[G_ORDER] = { 0 };
    Neighbor edges[G_MAX_EDGES] = { 0 };
    NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

    //printf("probability %f: \n", p);
    //printf("p: %f\n", p);
    fill_graph_binomial(&g, p, random_state);

    size_t paths[G_ORDER];
    Forest f = {paths, G_ORDER};
    bfs(g, &f);

    //printf("shortest path forest:\n");

    //print_forest(f);

    int cycle_found = 0;
    size_t* cycle_buffer_block = malloc(sizeof(size_t) * g.order);
    array cycle_buffer = {(char*)cycle_buffer_block, 0, sizeof(size_t) * g.order};
    
    for (size_t i = 0; i < g.order; i++) {
      Neighbor* n = g.neighborhoods[i];
      while (n) {
        PairEdge e = {i, n->vertex};
        //printf("checking cycle at (%ld %ld)\n", e.left, e.right);
        cycle_found = (get_cycle(f.paths, e, &cycle_buffer) == 0);

        if (cycle_found) {
          //printf("cycle found: ");
          //print_array(cycle_buffer, sizeof(size_t));
          break;
        }

        n = n->next;
      }
      if (cycle_found) break;
    }

    if (cycle_buffer.length == 0 || (cycle_buffer.length / sizeof(size_t) - 1) % 2 == 0) {
      free(cycle_buffer_block);
      p += 0.01;
      continue;
    }

    printf("graph:\n");
    print_graph(g);
    printf("found odd cycle! : ");
    for (size_t i = 0; i < cycle_buffer.length / sizeof(size_t); i++) {
      printf("%ld, ", cycle_buffer_block[i]);
    }
    printf("\n");

    printf("contracting cycle...\n");
    size_t hierarchy[G_ORDER];
    NeighborhoodGraphMinor m = {g, hierarchy};
    for (int i = 0; i < g.order; i++) {
      m.hierarchy[i] = -1;
    }

    print_hierarchy_to_string(m, asserterA, 8192);

    cycle_buffer.length -= sizeof(size_t);
    contract(&m, cycle_buffer);

    print_hierarchy(m);

    //printf("building path from contracted cycle...\n");
    Neighbor* path_neighborhoods[G_ORDER] = { 0 };
    Neighbor path_edges[4] = { 0 };
    NeighborhoodGraph path = {path_neighborhoods, G_ORDER, path_edges, 0};

    FatNeighbor start = {cycle_buffer_block[0], g.neighborhoods[cycle_buffer_block[0]]};
    while (start.neighbor) {
      //printf("%ld %p %ld %p\n", start.vertex, start.neighbor, start.neighbor->vertex, start.neighbor->next);

      PairEdge e = {start.neighbor->vertex, start.vertex};
      if (
        start.neighbor && 
        root(m.hierarchy, start.neighbor->vertex) != root(m.hierarchy, cycle_buffer_block[0]) &&
        !has_edge(path, e)
      ) {
        add_neighbor(&path, e.left, e.right);
        add_neighbor(&path, e.right, e.left);

        if (path.edges >= 4) break;
      }

      start = next_neighbor(start, m);
    }

    if (path.edges == 4) {
      keep_going = 0;
    } else {
      printf("no path found through odd cycle, restarting...\n");
      p += 0.01;
      continue;
    }

    printf("constructed path: \n");
    print_graph(path);
    //printf("on cycle: ");
    //print_array(cycle_buffer, sizeof(size_t));

    lift(m, cycle_buffer_block[0], &path);
    printf("lifted path: \n");
    print_graph(path);

    printf("resulting graph:\n");

    print_hierarchy(m);

    print_hierarchy_to_string(m, asserterB, 8192);

    printf("GRAPH STATE (SHOULD BE 0): %d\n", strcmp(asserterA, asserterB));

    free(cycle_buffer_block);
  }

  fnDelocXSR(random_state);

  
}
