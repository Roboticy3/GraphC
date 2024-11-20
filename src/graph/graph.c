
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

int has_edge(NeighborhoodGraph g, PairEdge e) {
  Neighbor* n = g.neighborhoods[e.left];
  while (n) {
    if (n->vertex == e.right) return 1;
    n = n->next;
  }

  return 0;
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
    queue q;
    q.buffer = malloc(sizeof(size_t) * g.order);
    q.capacity = sizeof(size_t) * g.order;
    q.front = 0;
    q.back = 0;
    queue_push(&q, &component, sizeof(size_t));

    if (d[component] != -1) {
      free(q.buffer);
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
        if (d[n->vertex] == -1) {
          queue_push(&q, &(n->vertex), sizeof(size_t));    
        }
        //if the distance to the neighbor through v is better than its current distance, update its entry in the distance array
        //since the "default" distance is maximum, finding a neighbor that hasn't been explored yet will always update its distance
        if (d[n->vertex] > d[v] + 1) {
          d[n->vertex] = d[v] + 1;
          //this is also when the shortest path to that vertex is updated.
          t->paths[n->vertex] = v;
        }
      }
    }

    free(q.buffer);
  }

  free(d);
}

size_t get_height(Forest t, size_t v) {
  size_t height = 0;
  while (t.paths[v] != -1) {
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
  while (height_a > height_b && a != -1) {
    //printf("reading slot %ld out of %ld\n", a, t.order);
    a = t.paths[a];
    height_a--;
    //printf("%ld %ld\n", a, b);
  }

  //printf("normalized (%ld %ld)\n", a, b);

  //now trace each vertex up the tree until the ancestor is found.
  while (a != b && a != -1) {
    //printf("reading slot %ld out of %ld\n", a, t.order);
    a = t.paths[a];
    //printf("reading slot %ld out of %ld\n", b, t.order);
    b = t.paths[b];
    //printf("%ld %ld\n", a, b);
  }

  return a;
}

//this thing is starting to piss me off
// The smallest cycle containing an edge e and a root of t
int get_cycle(Forest t, PairEdge e, array* out) {
    size_t start = e.left;
    size_t a = lca(t, e.left, e.right);

    //printf("Cycle seeds e: (%ld %ld, %ld) lca: %ld\n",start, e.left, e.right, a);

    // If one endpoint is the ancestor of the other, not a cycle
    if (a == e.left || a == e.right) {
      //fprintf(stderr, "Error: Edge is not in a cycle.\n");
      out->length = 0;
      return 1;
    }

    size_t* block = (size_t*)out->block;
    size_t st_len = 0;

    // Walk down the path from the first endpoint to the LCA and store it in block
    size_t first_path_start = st_len;
    while (e.left != a) {
    if (st_len >= out->capacity / sizeof(size_t)) {
        fprintf(stderr, "Error: Cycle exceeds output array capacity.\n");
        return -1;
    }
    block[st_len++] = e.left;
    e.left = t.paths[e.left];
    }

    // Reverse the first path in-place
    size_t first_path_end = st_len;
    for (size_t i = 0; i < (first_path_end - first_path_start) / 2; ++i) {
        size_t temp = block[first_path_start + i];
        block[first_path_start + i] = block[first_path_end - 1 - i];
        block[first_path_end - 1 - i] = temp;
    }


    // Add the LCA to the cycle
    if (st_len >= out->capacity / sizeof(size_t)) {
        fprintf(stderr, "Error: Cycle exceeds output array capacity.\n");
        return -1;
    }
    block[st_len++] = a;

    // Append the path from the second endpoint to the LCA
    while (e.right != a) {
        if (st_len >= out->capacity / sizeof(size_t)) {
            fprintf(stderr, "Error: Cycle exceeds output array capacity.\n");
            return -1;
        }
        block[st_len++] = e.right;
        e.right = t.paths[e.right];
    }

    // Close the cycle by appending the starting vertex
    if (st_len >= out->capacity / sizeof(size_t)) {
        fprintf(stderr, "Error: Cycle exceeds output array capacity.\n");
        return -1;
    }
    block[st_len++] = start;

    //for some reason this shit doesn't work when a == 0. Chat really struggled with that one so... hack fix
    //tbh this is far from the most egregious thing in this project, but I'm on a schedule and in over my head
    if (a == 0) {
      for (size_t i = 0; i < st_len - 1; i++) {
        block[i] = block[i + 1];
      }
      block[st_len - 1] = 0;
    }

    // Validate the cycle is closed
    if (block[0] != block[st_len - 1]) {
        st_len--;
        if (block[0] != block[st_len - 1]) {
          fprintf(stderr, "Error: Cycle is malformed. Contents: ");
        for (size_t i = 0; i < st_len; ++i) {
            fprintf(stderr, "%zu ", block[i]);
        }
        fprintf(stderr, "\n");
        return -1;
        }
        
    }

    out->length = st_len * sizeof(size_t);
    return 0;
}

size_t root(size_t* paths, size_t from) {
  size_t v = from;
  while (paths[v] != -1) {
    v = paths[v];
  }
  return v;
}

void contract(size_t* vertices, size_t vertices_count, NeighborhoodGraphMinor* g) {
  if (!vertices_count) return;

  //select the supervertex of the first vertex
  //then, contract the rest of the vertices to it
  size_t u = root(g->hierarchy, vertices[0]);
  for (size_t i = 1; i < vertices_count; i++) {
    size_t v = root(g->hierarchy, vertices[i]);
    printf("contracting %ld from %ld into %ld\n", vertices[i], v, u);
    g->hierarchy[v] = u;
  }
}

FatNeighbor next_neighbor(FatNeighbor n, NeighborhoodGraphMinor g) {
  if (n.neighbor && n.neighbor->next) {
    return (FatNeighbor){n.vertex, n.neighbor->next};
  }

  size_t r = root(g.hierarchy, n.vertex);
  for (size_t i = n.vertex + 1; i < g.original.order; i++) {
    if (root(g.hierarchy, i) == r) {
      if (g.original.neighborhoods[i]) return (FatNeighbor){i, g.original.neighborhoods[i]};
    }
  }

  return (FatNeighbor){-1, 0};
}

void print_graph(NeighborhoodGraph graph) {
  for (size_t i = 0; i < graph.order; i++) {
    Neighbor* n = graph.neighborhoods[i];
    printf("[%ld]", i);
    int i = 0;
    while (n < graph.neighbors + graph.edges && n >= graph.neighbors) {
      printf(" -> %ld", n->vertex);
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
    printf("(%ld : (%ld : %ld)), ", i, g.neighbors[i].vertex, g.neighbors[i].next - g.neighbors);
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

//you can tell when I'm desparate enough to use AI because GPT uses 4-spaces to indent and I use 2.
//took some edits, but definitely sped things up
void print_hierarchy(NeighborhoodGraphMinor g) {
  //for each vertex in the original graph
  for (size_t i = 0; i < g.original.order; ++i) {
        //print out the relationships of the original vertex to its supervertex/ces
        size_t current = i;
        size_t j = 0;
        int first = 1; 
        while (g.hierarchy[current] != -1 && j < 20) {
            if (first) {
                printf("(%zu)", current);
                first = 0;
            } else {
                printf(" -> (%zu)", current);
            }
            current = g.hierarchy[current];
            j++;
        }
        //then print out the vertex itself
        if (first) {
          printf("[%lu]", current);
        }
        else {
          printf(" -> [%zu]", current);
        }

        Neighbor* n = g.original.neighborhoods[i];
        while (n) {
          printf(" -> %lu", n->vertex);
          n = n->next;
        }

        printf("\n");
    }
}

void print_minor_path(NeighborhoodGraphMinor g, NeighborhoodGraph p, FatNeighbor start) {
  printf("%ld", start.vertex);

  size_t i = 0;
  PairEdge last_edge = {-1, -1};
  while (start.neighbor && i < 20) {
    //printf("%ld %p %ld %p\n", start.vertex, start.neighbor, start.neighbor->vertex, start.neighbor->next);

    if (start.neighbor) {
      size_t u = start.vertex;
      size_t v = start.neighbor->vertex;
      PairEdge e = {u, v};
      PairEdge backwards_e = {v, u};
      if (
        !(backwards_e.left == last_edge.left && backwards_e.right == last_edge.right) && //make sure not retracing step O(1)
        has_edge(p, e) && //make sure edge exists in path O(e(G))
        root(g.hierarchy, u) != root(g.hierarchy, v) //make sure not under same supervertex O(n(G))
      ) {
        start = (FatNeighbor){start.neighbor->vertex, g.original.neighborhoods[start.neighbor->vertex]};
        printf(", %ld", start.vertex);
        last_edge = e;
        i++;
        continue;
      }
    }

    start = next_neighbor(start, g);
    //if (start.neighbor) printf("ext %ld %ld\n", start.vertex, start.neighbor->vertex);
    i++;
  }

  printf("\n");
  
}