
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

  //printf("cutoff %d\n", cutoff);
  //decide how many edges to insert into the graph
  size_t i = 0;
  size_t pair[2] = {1, 2};
  while (pair[0] < graph->order || pair[1] < graph->order + 1) {
    int x = rand();
    if (x <= cutoff) {
      printf("added edge! %ld %ld, edge %d\n", pair[0], pair[1], graph->edges);
      add_neighbor(graph, pair[0] - 1, pair[1] - 1);
      add_neighbor(graph, pair[1] - 1, pair[0] - 1);
    }
    i++;

    choose_step(graph->order, 2, pair);
  }

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