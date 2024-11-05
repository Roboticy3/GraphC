
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
    
    //print_graph(g);

    size_t shortest_paths[G_ORDER] = { [ 0 ... G_ORDER - 1] = -1};
    PairEdge shortest_paths_cycles[G_MAX_EDGES] = {0};
    ShortestPathGraph t = {shortest_paths, G_ORDER, 0, shortest_paths_cycles, 0};

    bfs(g, &t);

    size_t cycle_lengths_block[G_MAX_EDGES];
    array cycle_lengths_array = {cycle_lengths_block, 0, G_MAX_EDGES};

    cycle_lengths(t, &cycle_lengths_array);

    //printf("found %ld cycles\n", cycle_lengths_array.length);

    for (size_t i = 0; i < cycle_lengths_array.length; i++) {
      printf("%ld, ", cycle_lengths_block[i]);
    }
    printf("\n");

    p += 0.01;
  }

  
}
