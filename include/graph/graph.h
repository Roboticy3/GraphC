#ifndef GRAPH_H_
#define GRAPH_H_

#include <stddef.h>
#include <collections/array.h>
#include <stdint.h>
#include <combinations/xoshiro.h>

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

struct Forest {
  size_t* paths;
  size_t order;
} typedef Forest;

void add_neighbor(NeighborhoodGraph* g, size_t vertex, size_t edge);

void fill_graph_binomial(NeighborhoodGraph* g, float p, pXSR seed);

void bfs(NeighborhoodGraph g, Forest* t);

size_t get_height(Forest t, size_t v);

size_t lca(Forest t, size_t a, size_t b);

void get_cycle(Forest t, PairEdge e, array* out);

void print_graph(NeighborhoodGraph g);

void print_graph_raw(NeighborhoodGraph g);

void print_forest(Forest t);

#endif