
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

  int keep_going = 1;
  float p = 0.1;

  while (keep_going && p <= 1.0) {
    Neighbor* neighborhoods[G_ORDER] = { 0 };
    Neighbor edges[G_MAX_EDGES] = { 0 };
    NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

    //printf("probability %f: \n", p);
    printf("p: %f\n", p);
    fill_graph_binomial(&g, p, random_state);
    
    //print_graph(g);

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

    if (cycle_buffer.length == 0 || (cycle_buffer.length / sizeof(size_t) - 1) % 2 == 0) {
      free(cycle_buffer_block);
      p += 0.01;
      continue;
    }

    printf("found odd cycle! : ");
    print_array(cycle_buffer, sizeof(size_t));
    print_graph(g);

    printf("contracting cycle...\n");
    size_t hierarchy[G_ORDER];
    NeighborhoodGraphMinor m = {g, hierarchy};
    for (int i = 0; i < g.order; i++) {
      m.hierarchy[i] = -1;
    }

    contract(cycle_buffer_block, cycle_buffer.length / sizeof(size_t) - 1, &m);

    print_hierarchy(m);

    printf("building path from contracted cycle...\n");
    Neighbor* path_neighborhoods[G_ORDER] = { 0 };
    Neighbor path_edges[4] = { 0 };
    NeighborhoodGraph path = {path_neighborhoods, G_ORDER, path_edges, 0};

    FatNeighbor start = {cycle_buffer_block[0], g.neighborhoods[cycle_buffer_block[0]]};
    while (start.neighbor) {
      //printf("%ld %p %ld %p\n", start.vertex, start.neighbor, start.neighbor->vertex, start.neighbor->next);

      if (
        start.neighbor && 
        root(m.hierarchy, start.neighbor->vertex) != root(m.hierarchy, cycle_buffer_block[0]) &&
        !has_edge(path, (PairEdge){start.neighbor->vertex, start.vertex})
      ) {
        add_neighbor(&path, start.neighbor->vertex, start.vertex);
        add_neighbor(&path, start.vertex, start.neighbor->vertex);

        if (path.edges >= 4) break;
      }

      start = next_neighbor(start, m);
    }

    if (path.edges == 4) {
      keep_going = 0;
    } else {
      continue;
    }

    print_graph(path);

    print_minor_path(m, path, (FatNeighbor){cycle_buffer_block[0], g.neighborhoods[cycle_buffer_block[0]]});

    

    free(cycle_buffer_block);
  }

  fnDelocXSR(random_state);

  
}
