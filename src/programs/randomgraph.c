
#include <programs/randomgraph.h>
#include <combinations/choose.h>
#include <graph/graph.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define G_ORDER 10
#define G_MAX_EDGES 45 * 2
#define E_P 0.5


int randomgraph() {

  srand(time(NULL));

  Neighbor* neighborhoods[G_ORDER] = { 0 };
  Neighbor edges[G_MAX_EDGES] = { 0 };
  NeighborhoodGraph g = {neighborhoods, G_ORDER, edges, 0};

  printf("\nprobability %f: \n\n", E_P);
  fill_graph_random(&g, E_P);
  
  print_graph(g);

  size_t shortest_paths[G_ORDER] = {0};
  Fan f = {shortest_paths, G_ORDER, 0};

  bfs(g, f);

  printf("\nshortest paths from %ld: \n\n", f.center);

  print_fan(f);
}
