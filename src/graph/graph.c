
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <graph/graph.h>

#include <combinations/choose.h>

void add_neighbor(Neighbor** current_neighborhood_location, Neighbor* new_neighbor_location, size_t neighbor) {
  Neighbor* start = current_neighborhood_location[0];
  Neighbor new_neighbor = {neighbor, start};
  new_neighbor_location[0] = new_neighbor;
  current_neighborhood_location[0] = new_neighbor_location;
}


void fill_graph_random(Neighbor** neighborhoods, size_t n_len, Neighbor* neighbors, float p) {

  int cutoff = p == 1.0 ? INT_MAX : RAND_MAX * p;

  //printf("cutoff %d\n", cutoff);
  //decide how many edges to insert into the graph
  size_t i = 0;
  size_t j = 0;
  size_t pair[2] = {1, 2};
  while (pair[0] != n_len - 1 || pair[1] != n_len) {
    int x = rand();
    //printf("attempt: %d endpoint: %d pair: %d %d roll: %d\n", i, j, pair[0], pair[1], x);
    if (x <= cutoff) {
      //printf("added edge!\n");
      add_neighbor(neighborhoods + pair[0] - 1, neighbors + pair[1] - 1, j);
      j++;
      add_neighbor(neighborhoods + pair[1] - 1, neighbors + pair[0] - 1, j);
      j++;
    }
    i ++;

    choose_step(n_len, 2, pair);
  }

}


void print_graph(Neighbor** neighborhoods, size_t order, Neighbor* neighbors) {
  for (size_t i = 0; i < order; i++) {
    Neighbor* n = neighborhoods[i];
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