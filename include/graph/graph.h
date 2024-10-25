#ifndef GRAPH_H_
#define GRAPH_H_

void add_neighbor(size_t* neighborhood, size_t* edges, size_t neighbor, size_t edge);

void fill_graph_random(size_t* neighborhoods, int n_len, size_t* edges, int e_len, float p);

void print_graph(size_t* neighborhoods, size_t order, size_t* edges);

#endif