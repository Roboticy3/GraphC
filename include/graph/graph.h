#ifndef GRAPH_H_
#define GRAPH_H_

struct Neighbor {
  size_t neighbor;
  struct Neighbor* next;
} typedef Neighbor;

void add_neighbor(Neighbor** current_neighborhood_location, Neighbor* new_neighbor_location, size_t neighbor);

void fill_graph_random(Neighbor** neighborhoods, size_t n_len, Neighbor* neighbors, size_t e_len, float p);

void print_graph(Neighbor** neighborhoods, size_t order, Neighbor* neighbors);

#endif