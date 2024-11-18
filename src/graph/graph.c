
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

#include <graph/graph.h>
#include <combinations/choose.h>
#include <combinations/xoshiro.h>
#include <collections/queue.h>
#include <collections/array.h>
#include <sample/sample.h>

void add_neighbor(NeighborhoodGraph* g, size_t vertex, size_t neighbor) {
  Neighbor* start = g->neighborhoods[vertex];
  Neighbor new_neighbor = {neighbor, start};
  g->neighbors[g->edges] = new_neighbor;
  g->neighborhoods[vertex] = g->neighbors + g->edges;
  g->edges++;
}

void fill_graph_binomial(NeighborhoodGraph* g, float p, pXSR random_state) {

    size_t num;

    if (g->order < 2) {
        return;
    } else if (g->order == 2) {
        num = XSRGen(XSR_SS, random_state);  // Generate a number with XSR
        if ((double)num / (double)UINT64_MAX <= (double)p) {
            //printf("added edge! %ld %ld, edge %ld\n", 0, 1, g->edges);
            add_neighbor(g, 0, 1);
            add_neighbor(g, 1, 0);
        }
        return;
    }

    size_t max_edges = choose(g->order, 2);
    size_t i = 0;
    size_t pair[2] = {1, 2};
    
    // Insert edges while ensuring the number of edges does not exceed the max
    while (pair[0] < g->order || pair[1] < g->order + 1) {
        if (i >= max_edges) {
            printf("ran out of edges! pair: [%ld, %ld]; edge: %ld (out of %ld)\n", pair[0], pair[1], i, max_edges);
            break;
        }

        num = XSRGen(XSR_SS, random_state);  // Generate a number with XSR
        if ((double)num / (double)UINT64_MAX <= (double)p) {
            //printf("added edge! %ld %ld, edge %ld\n", pair[0], pair[1], g->edges);
            add_neighbor(g, pair[0] - 1, pair[1] - 1);
            add_neighbor(g, pair[1] - 1, pair[0] - 1);
        }
        i++;

        choose_step(g->order, 2, pair);
    }
}


void bfs(NeighborhoodGraph g, Forest* t) {

  //distance array
  size_t* d = malloc(g.order * sizeof(size_t));
  //fill with the maximum possible distance. -1 = size_t max
  for (size_t i = 0; i < g.order; i++) d[i] = -1;

  //clear the vertices from t
  for (size_t i = 0; i < t->order; i++) t->paths[i] = -1;

  for (size_t component = 0; component < g.order; component++) {

    //vertex queue
    queue q = qalloc(sizeof(size_t) * g.order);
    queue_push(&q, &component, sizeof(size_t));

    if (d[component] != -1) {
      continue;
    }

    d[component] = 0;

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
          t->paths[n->neighbor] = v;
        }
      }
    }

    qfree(q);
  }

  free(d);
}

size_t get_height(Forest t, size_t v) {
  size_t height = 0;
  if (t.paths[v] == -1) return -1;
  while (v != -1) {
    v = t.paths[v];
    height++;
  }
  return height;
}

size_t lca(Forest t, size_t a, size_t b) {
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

//the smallest cycle containing an edge e and a root of t
void get_cycle(Forest t, PairEdge e, array* out) {
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

void print_graph(NeighborhoodGraph graph) {
  for (size_t i = 0; i < graph.order; i++) {
    Neighbor* n = graph.neighborhoods[i];
    printf("[%ld]", i);
    int i = 0;
    while (n < graph.neighbors + graph.edges && n >= graph.neighbors) {
      printf(" -> %ld", n->neighbor);
      n = n->next;
      //printf("%ld\n",n);
    }
    printf("\n");
  }
}

void print_graph_raw(NeighborhoodGraph g) {
  for (size_t i = 0; i < g.order; i++) {
    printf("(%ld : %ld), ", i, g.neighborhoods[i] - g.neighbors);
  }

  printf("\n");

  for (size_t i = 0; i< g.edges; i++) {
    printf("(%ld : (%ld : %ld)), ", i, g.neighbors[i].neighbor, g.neighbors[i].next - g.neighbors);
  }

  printf("\n");
}

void print_forest(Forest t) {
  for (size_t i = 0; i < t.order; i++) {
    size_t n = t.paths[i];
    printf("[%ld]%s -> %ld\n", i, t.paths[i] == -1 ? "*":"", n);
  }
}

void binomial_graph_random_sample(sampledata params, sample* sample,size_t (*property)(NeighborhoodGraph)) {

  Neighbor** neighborhoods = malloc(params.order * sizeof(Neighbor*));
  Neighbor* neighbors = malloc(params.order * (params.order - 1) * sizeof(Neighbor));

  for (int i = 0; i < sample->len; i++) { 

    for (int j = 0; j < params.order; j++) {
      neighborhoods[j] = 0;
    }

    NeighborhoodGraph g = {neighborhoods, params.order, neighbors, 0};
    fill_graph_binomial(&g, params.p, params.random_state);
    sample->x[i] = (*property)(g);
  }

  free(neighborhoods);
  free(neighbors);
}