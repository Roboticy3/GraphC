#ifndef GRAPH_H_
#define GRAPH_H_

#include <stddef.h>

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

struct Fan {
  size_t* paths;
  size_t order;
  size_t center;
} typedef Fan;

void add_neighbor(NeighborhoodGraph* graph, size_t vertex, size_t edge);

void fill_graph_random(NeighborhoodGraph* graph, float p);

void bfs(NeighborhoodGraph g, Fan f);

void print_graph(NeighborhoodGraph graph);

void print_fan(Fan fan);

#endif