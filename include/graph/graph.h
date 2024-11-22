#ifndef GRAPH_H_
#define GRAPH_H_

#include <stddef.h>
#include <collections/array.h>
#include <stdint.h>
#include <combinations/xoshiro.h>

struct Neighbor {
  size_t vertex;
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

struct NeighborhoodGraphMinor {
  NeighborhoodGraph original;
  size_t* hierarchy;
} typedef NeighborhoodGraphMinor;

struct FatNeighbor {
  size_t vertex;
  Neighbor* neighbor;
} typedef FatNeighbor;

void add_neighbor(NeighborhoodGraph* g, size_t vertex, size_t edge);

int has_edge(NeighborhoodGraph g, PairEdge e);

void fill_graph_binomial(NeighborhoodGraph* g, float p, pXSR seed);

void bfs(NeighborhoodGraph g, Forest* t);

size_t get_height(size_t* paths, size_t v);

size_t lca(size_t* paths, size_t a, size_t b);

int get_cycle(size_t* paths, PairEdge e, array* out);

void contract_single(size_t from, size_t to, NeighborhoodGraphMinor* g);

size_t root(size_t* paths, size_t from);

int contract(NeighborhoodGraphMinor* g, array vertices);

int expand(NeighborhoodGraphMinor g, size_t v, array out_mask);

FatNeighbor next_neighbor(FatNeighbor n, NeighborhoodGraphMinor g);

size_t next_subvertex(size_t v, NeighborhoodGraphMinor g);

int lift(NeighborhoodGraphMinor g, size_t v, NeighborhoodGraph* h);

void print_graph(NeighborhoodGraph g);

void print_graph_raw(NeighborhoodGraph g);

void print_forest(Forest t);

void print_hierarchy(NeighborhoodGraphMinor g);

void print_hierarchy_to_string(NeighborhoodGraphMinor g, char* buffer, size_t buffer_size);

void print_paths_raw(size_t* paths, size_t len);

void print_minor_path(NeighborhoodGraphMinor g, NeighborhoodGraph p, FatNeighbor start);

#endif