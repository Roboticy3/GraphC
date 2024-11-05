#ifndef GRAPH_H_
#define GRAPH_H_

#include <stddef.h>
#include <collections/array.h>

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

struct PairEdge {
  size_t left;
  size_t right;
} typedef PairEdge;

struct ShortestPathGraph {
  size_t* paths;
  size_t order;
  size_t center;
  PairEdge* cycles;
  size_t cycle_count;
} typedef ShortestPathGraph;

void add_neighbor(NeighborhoodGraph* g, size_t vertex, size_t edge);

void fill_graph_random(NeighborhoodGraph* g, float p);

void bfs(NeighborhoodGraph g, ShortestPathGraph* t);

size_t get_height(ShortestPathGraph t, size_t v);

size_t lca(ShortestPathGraph t, size_t a, size_t b);

void get_cycle(ShortestPathGraph t, PairEdge e, array* out);

void cycle_lengths(ShortestPathGraph t, array* out);

void print_graph(NeighborhoodGraph g);

void print_shortest_paths(ShortestPathGraph t);

#endif