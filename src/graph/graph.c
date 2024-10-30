
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <graph/graph.h>
#include <combinations/choose.h>
#include <collections/queue.h>

void add_neighbor(NeighborhoodGraph* g, size_t vertex, size_t neighbor) {
  Neighbor* start = g->neighborhoods[vertex];
  Neighbor new_neighbor = {neighbor, start};
  g->neighbors[g->edges] = new_neighbor;
  g->neighborhoods[vertex] = g->neighbors + g->edges;
  g->edges++;
}


void fill_graph_random(NeighborhoodGraph* graph, float p) {

  int cutoff = p == 1.0 ? INT_MAX : RAND_MAX * p;

  if (graph->order < 2) {
    return;
  }
  else if (graph->order == 2) {
    int x = rand();
    if (x <= cutoff) {
      //printf("added edge! %ld %ld, edge %d\n", 0, 1, graph->edges);
      add_neighbor(graph, 0, 1);
      add_neighbor(graph, 1, 0);
    }
    return;
  }

  size_t max_edges = choose(graph->order, 2);

  //printf("cutoff %d\n", cutoff);
  //decide how many edges to insert into the graph
  size_t i = 0;
  size_t pair[2] = {1, 2};
  while (pair[0] < graph->order || pair[1] < graph->order + 1) {
    if (i >= max_edges) {
      printf("ran out of edges! pair: [%ld, %ld]; edge: %ld (out of %ld)\n", pair[0], pair[1], i, max_edges);
      break;
    }

    int x = rand();
    if (x <= cutoff) {
      //printf("added edge! %ld %ld, edge %d\n", pair[0], pair[1], graph->edges);
      add_neighbor(graph, pair[0] - 1, pair[1] - 1);
      add_neighbor(graph, pair[1] - 1, pair[0] - 1);
    }
    i++;

    choose_step(graph->order, 2, pair);
  }

}

void bfs(NeighborhoodGraph g, SinkTree f) {
  //vertex queue
  queue q = { 0 };
  //allocate space for every vertex in the graph
  q.buffer = calloc(g.order, sizeof(size_t));
  q.capacity = g.order * sizeof(size_t);
  //add the starting vertex
  queue_push(&q, &(f.center), sizeof(size_t));

  //distance array
  size_t* d = malloc(g.order * sizeof(size_t));
  //fill with the maximum possible distance. -1 = size_t max
  for (int i = 0; i < g.order; i++) d[i] = -1;
  //set the distance of the starting vertex to 0
  d[f.center] = 0;

  //while there are vertices in the queue,
  while (queue_size(q)) {
    size_t v;
    //pop the oldest vertex in the queue, v, and look through its neighbors
    queue_pop(&q, &v, sizeof(size_t));
    for (Neighbor* n = g.neighborhoods[v]; n; n = n->next) {
      //if the distance of a neighbor has not been set yet, it has yet to be explored, so add it to the queue
      if (d[n->neighbor] == -1) {
        queue_push(&q, &(n->neighbor), sizeof(size_t));
        
      }

      //if the distance to the neighbor through v is better than its current distance, update its entry in the distance array
      //since the "default" distance is maximum, finding a neighbor that hasn't been explored yet will always update its distance
      if (d[n->neighbor] > d[v] + 1) {
        d[n->neighbor] = d[v] + 1;
        //this is also when the shortest path to that vertex is updated.
        f.paths[n->neighbor] = v;
      }
    }
  }

  //free the queue and distance array
  free(q.buffer);
  free(d);
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

void print_sinktree(SinkTree fan) {
  for (size_t i = 0; i < fan.order; i++) {
    size_t n = fan.paths[i];
    printf("[%ld] -> %ld\n", i, n);
  }
}