
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <graph/graph.h>
#include <combinations/choose.h>
#include <collections/queue.h>
#include <collections/array.h>

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

void bfs(NeighborhoodGraph g, ShortestPathGraph* t) {
  //vertex queue
  queue q = { 0 };
  //allocate space for every vertex in the graph
  q.buffer = calloc(g.order, sizeof(size_t));
  q.capacity = g.order * sizeof(size_t);
  //add the starting vertex
  queue_push(&q, &(t->center), sizeof(size_t));

  //distance array
  size_t* d = malloc(g.order * sizeof(size_t));
  //fill with the maximum possible distance. -1 = size_t max
  for (int i = 0; i < g.order; i++) d[i] = -1;
  //set the distance of the starting vertex to 0
  d[t->center] = 0;

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
      //otherwise, we've run into a neighbor we've seen before, implying a cycle
      else if (n->neighbor != t->paths[v]) {
        //otherwise, we have found a cycle
        PairEdge c = {n->neighbor, v};
        t->cycles[t->cycle_count] = c;
        t->cycle_count++;
      }

      //if the distance to the neighbor through v is better than its current distance, update its entry in the distance array
      //since the "default" distance is maximum, finding a neighbor that hasn't been explored yet will always update its distance
      if (d[n->neighbor] > d[v] + 1) {
        d[n->neighbor] = d[v] + 1;
        //this is also when the shortest path to that vertex is updated.
        t->paths[n->neighbor] = v;
      }
    }
  }

  //free the queue and distance array
  free(q.buffer);
  free(d);
}

size_t get_height(ShortestPathGraph t, size_t v) {
  size_t height = 0;
  if (t.paths[v] == -1) return -1;
  while (v != t.center) {
    v = t.paths[v];
    height++;
  }
  return height;
}

size_t lca(ShortestPathGraph t, size_t a, size_t b) {
  //get the height of both vertices.
  size_t height_a = get_height(t, a), height_b = get_height(t, b);
  //printf("found heights (%ld %ld)\n", height_a, height_b);
  //sort the vertices so a is at least as far from the root of the tree as b
  if (height_b > height_a) {
    //printf("sorting input...\n");
    size_t height_temp = height_a;
    size_t temp = a;
    height_a = height_b;
    a = b;
    height_b = height_temp;
    b = temp;
  }

  //printf("sorted (%ld %ld)\n", a, b);
  
  //trace a to the same height as b
  while (height_a > height_b) {
    a = t.paths[a];
    height_a--;
    //printf("%ld %ld\n", a, b);
  }

  //printf("normalized (%ld %ld)\n", a, b);

  //now trace each vertex up the tree until the ancestor is found.
  while (a != b) {
    a = t.paths[a];
    b = t.paths[b];
    //printf("%ld %ld\n", higher, lower);
  }

  return a;
}

//A Shortest Path Graph with a record of cycles as edges between branches in the tree can retrieve a whole cycle given an edge in 
void get_cycle(ShortestPathGraph t, PairEdge e, array* out) {
  size_t* block = (size_t*)out->block;

  //preserve the starting vertex to add onto the end of the cycle as the end delimiter
  size_t start = e.left;
  //get the lowest common ancestor of the endpoints of e
  size_t a = lca(t, e.left, e.right);
  //printf("forming cycle through %ld %ld %ld\n", e.left, a, e.right);
  //if one end of e was the ancestor of the other, e is not in a cycle
  if (a == e.left || a == e.right){
    block[0] = e.left;
    return;
  }

  //otherwise, walk down to the lca from one endpoint, reverse that, then concatenate the walk from the other endpoint to define a cycle
  out->length = 0;
  while (e.left != a) {
    block[out->length] = e.left;
    //printf("set %ld to %ld\n", out->length, e.left);
    e.left = t.paths[e.left];
    out->length++;
  }

  //reverse the first section
  //reverse(out, sizeof(size_t));
  block[out->length] = a;
  //printf("(common ancestor) set %ld to %ld\n", out->length, a);
  out->length++;

  //concatenate the other half of the cycle
  while (e.right != a) {
    block[out->length] = e.right;
    //printf("set %ld to %ld\n", out->length, e.right);
    e.right = t.paths[e.right];
    out->length++;
  }

  block[out->length] = start;
  out->length++;
  
  
}

void maximize_matching(NeighborhoodGraph graph, Neighbor** matching) {
  
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

void print_shortest_paths(ShortestPathGraph t) {
  printf("shortest paths from %ld:\n", t.center);
  for (size_t i = 0; i < t.order; i++) {
    size_t n = t.paths[i];
    printf("[%ld]%s -> %ld\n", i, i == t.center ? "*":"", n);
  }

  size_t* cycle_register = malloc(sizeof(size_t) * (t.order + 1));
  array cycle_array = {cycle_register, 0, sizeof(size_t) * (t.order + 1)};
  
  printf("cycles:\n");
  for (size_t i = 0; i < t.cycle_count; i++) {
    PairEdge c = t.cycles[i];
    get_cycle(t, c, &cycle_array);

    printf("cycle containing (%ld, %ld): %ld", c.left, c.right, cycle_register[0]);
    for (size_t j = 1; j < cycle_array.length; j++) {
      printf(" -> %ld", cycle_register[j]);
    }
    printf("\n");

  }

  free(cycle_register);
}