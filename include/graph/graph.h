#ifndef GRAPH_H_
#define GRAPH_H_

struct Neighbor {
  size_t neighbor;
  struct Neighbor* next;
} typedef Neighbor;

struct NeighborhoodGraph {
  Neighbor** neighborhoods;
  size_t order;
  Neighbor* neighbors;
  size_t edges;
} typedef NeighborhoodGraph;

void add_neighbor(NeighborhoodGraph* graph, size_t vertex, size_t edge);

void fill_graph_random(NeighborhoodGraph* graph, float p);

void print_graph(NeighborhoodGraph graph);

#endif