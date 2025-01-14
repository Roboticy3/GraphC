// (c) 2025 Pablo Ibarz
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stddef.h>
#include <collections/array.h>
#include <stdint.h>
#include <combinations/xoshiro.h>


// One directed edge in the NeighborhoodGraph structure
struct Neighbor {
  size_t vertex;
  struct Neighbor* next;
} typedef Neighbor;

// Simple representation of an adjacency list graph
// Chains of Neighbors grow down from entries in the `neighborhoods` buffer to 
// describe the neighborhoods of each vertex.
// vertices are implicitly labeled by the size of the `neighborhoods` buffer,
// represented by `order`, while edges are written into the `neighbors` buffer
// as the graph is constructed, with `edges` representing the current number of
// edges.
struct NeighborhoodGraph {
  Neighbor** neighborhoods;
  size_t order;
  Neighbor* neighbors;
  size_t edges;
} typedef NeighborhoodGraph;

// One undirected edge
struct PairEdge {
  size_t left;
  size_t right;
} typedef PairEdge;

// Simple representation of a forest graph
struct Forest {
  size_t* paths;
  size_t order;
} typedef Forest;

// Simple representation of a graph minor, using NeighborhoodGraph as a base
// `hierarchy` maps nodes to their "contracting parent", chosen arbitrarily from
// the set of nodes being contracted in any given contraction step
// See contract and expand functions.
struct NeighborhoodGraphMinor {
  NeighborhoodGraph original;
  size_t* hierarchy;
} typedef NeighborhoodGraphMinor;

// A vertex and one reference to a neighbor in its chain in a NeighborhoodGraph
// While technically equivalent in structure to a regular neighbor, the `vertex`
// field here represents the "center" of a neighborhood, rather than the last
// neighbor in a chain.
// For example, Neighbor {i, next_neighbor} does not imply that i and
// next_neighbor->vertex are neighbors, while FatNeighbor {i, next_neighbor}
// does
struct FatNeighbor {
  size_t vertex;
  Neighbor* neighbor;
} typedef FatNeighbor;

// Add an undirected edge to `g` between `u` and `v`.
// Assumes `u` and `v` are less than `g->order`, and that `g->neighbors` has 
// more space to allocate for edges. Increments `g->edges` by 2.
void add_neighbor(NeighborhoodGraph* g, size_t u, size_t v);

// Checks if `g` contains the edge `e`.
int has_edge(NeighborhoodGraph g, PairEdge e);

// Fill `g` as a binomial random graph with edge probabiliy `p`.
// Assumes `g` is allocated (`g->order` spaces in `g->neighborhoods`, and 
// `g->order` choose 2 spaces in `g->neighbors`), and not filled.
// `seed` is a thread-safe, random state for generating random numbers.
void fill_graph_binomial(NeighborhoodGraph* g, float p, pXSR seed);

// Generalized breadth-first-search. Fill `t` with one shortest-path-tree for
// each component of `g`. Assumes `g` is valid and `t` is allocated and
// not filled.
void bfs(NeighborhoodGraph g, Forest* t);

// Gets the height vertex `v` in a tree represented by `paths`.
size_t get_height(size_t* paths, size_t v);

// Finds the vertex furthest from a tree-root in `paths` that is an ancestor of
// `a` and `b`. If `a` and `b` do not belong to the same tree, returns -1.
size_t lca(size_t* paths, size_t a, size_t b);

// Populates `out` with a cycle defined by a tree `paths` and an extra edge `e`
// Should fail if the endpoints of `e` do not have a valid lca, but has a bug
// that makes it fail in some other mysterious cases in the current iteration
int get_cycle(size_t* paths, PairEdge e, array* out);

// Contract 2 vertices `from`, `to`, into one in `g` with `to` as the parent in
// `g->hierarchy`
void contract_single(size_t from, size_t to, NeighborhoodGraphMinor* g);

// Find the root of `from` in the tree `paths`.
size_t root(size_t* paths, size_t from);

// Contract a set `vertices` into a single vertex in `g` with `vertices[0]` as
// the parent in `g->hierarchy`
int contract(NeighborhoodGraphMinor* g, array vertices);

// Expand a vertex `v` in `g`, populating `out_mask` with the expanded vertices
// in the process. `out_mask` is a bitmask of `g->original.order` bits.
int expand(NeighborhoodGraphMinor g, size_t v, array out_mask);

// Get the next neighbor of vertex `n.vertex` in a graph minor `g`. Used to
// iterate through the neighbors of a contracted vertex in `g`.
// FatNeighbors are required to emulate the chain structure in a graph minor.
FatNeighbor next_neighbor(FatNeighbor n, NeighborhoodGraphMinor g);

// Find the next vertex higher than `v` in `g` whose parent in `g.hierarchy` is
// the same as `v`'s
size_t next_subvertex(size_t v, NeighborhoodGraphMinor g);

// Lift a subgraph `h` of `g` through `g.hierarchy`. I'm pretty sure this 
// function is also bugged because I haven't tested it in a while and its a
// pretty involved operation.
int lift(NeighborhoodGraphMinor g, size_t v, NeighborhoodGraph* h);

// Print `g`
void print_graph(NeighborhoodGraph g);

// Print `g` with less pretty formatting, even if its contents are corrupt.
void print_graph_raw(NeighborhoodGraph g);

// Print `t`
void print_forest(Forest t);

// Print the minor hierarchy of `g`
void print_hierarchy(NeighborhoodGraphMinor g);

// Print the minor hierarchy of `g` to `buffer` of size `buffer_size`
void print_hierarchy_to_string(NeighborhoodGraphMinor g, char* buffer, size_t buffer_size);

// Print the tree `paths` as if it were just a list of `len` size_t elements.
void print_paths_raw(size_t* paths, size_t len);

#endif