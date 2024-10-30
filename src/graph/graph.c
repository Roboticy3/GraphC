
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <graph/graph.h>

#include <combinations/choose.h>

void add_neighbor(NeighborhoodGraph* g, size_t vertex, size_t neighbor) {
  Neighbor* start = g->neighborhoods[vertex];
  Neighbor new_neighbor = {neighbor, start};
  g->neighbors[g->edges] = new_neighbor;
  g->neighborhoods[vertex] = g->neighbors + g->edges;
  g->edges++;
}


void fill_graph_random(NeighborhoodGraph* graph, float p) {

  int cutoff = p == 1.0 ? INT_MAX : RAND_MAX * p;

  if (graph->order < 2) {
    return;
  }
  else if (graph->order == 2) {
    int x = rand();
    if (x <= cutoff) {
      //printf("added edge! %ld %ld, edge %d\n", 0, 1, graph->edges);
      add_neighbor(graph, 0, 1);
      add_neighbor(graph, 1, 0);
    }
    return;
  }

  size_t max_edges = choose(graph->order, 2);

  //printf("cutoff %d\n", cutoff);
  //decide how many edges to insert into the graph
  size_t i = 0;
  size_t pair[2] = {1, 2};
  while (pair[0] < graph->order || pair[1] < graph->order + 1) {
    if (i >= max_edges) {
      printf("ran out of edges! pair: [%ld, %ld]; edge: %ld (out of %ld)\n", pair[0], pair[1], i, max_edges);
      break;
    }

    int x = rand();
    if (x <= cutoff) {
      //printf("added edge! %ld %ld, edge %d\n", pair[0], pair[1], graph->edges);
      add_neighbor(graph, pair[0] - 1, pair[1] - 1);
      add_neighbor(graph, pair[1] - 1, pair[0] - 1);
    }
    i++;

    choose_step(graph->order, 2, pair);
  }

}

void max_matching(NeighborhoodGraph graph, size_t* matching) {
  
}

void print_graph(NeighborhoodGraph graph) {
  for (size_t i = 0; i < graph.order; i++) {
    Neighbor* n = graph.neighborhoods[i];
    printf("[%ld]", i);
    int i = 0;
    while (n) {
      printf(" -> %ld", n->neighbor);
      n = n->next;
      //printf("%ld\n",n);
    }
    printf("\n");
  }
}