#include <stdio.h>
#include <combinations/choose.h>

//stack-allocate a Petersen graph.

#define PETERSON_K 6
#define PETERSON_N 14
#define N_CHOOSE_K 3003
#define NmK_CHOOSE_K 28
#define EDGES N_CHOOSE_K * NmK_CHOOSE_K * 2

int is_petersen_neighbor(size_t* u, size_t len_u, size_t* v, size_t len_v) {
  for (size_t i = 0; i < len_u; i++) {
    for (size_t j = 0; j < len_v; j++) {
      if (u[i] == v[j]) return 0;
    }
  }

  return 1;
}

void print_graph(size_t* neighborhoods, size_t vertex_count, size_t* edges) {
  for (size_t i = 0; i < vertex_count; i++) {
    size_t n = neighborhoods[i];
    printf("[%ld]", i);
    while (n != -1) {
      printf(" -> %ld", edges[n]);
      n = edges[n + 1];
    }
    printf("\n");
  }
}

int petersen() {
  //allocate vertices
  size_t vertices[N_CHOOSE_K][PETERSON_K];

  //compute vertices
  for (int i = 0; i < N_CHOOSE_K; i++) {
    choose_i(PETERSON_N, PETERSON_K, i, vertices[i]);
  }

  //allocate edges
  size_t neighborhoods[N_CHOOSE_K] = { [ 0 ... (N_CHOOSE_K - 1) ] = -1 };
  size_t edges[EDGES] = { [ 0 ... (EDGES - 1) ] = -1};

  //compute edges
  //edge being computed
  size_t k = 0;
  //for each vertex
  for (size_t i = 0; i < N_CHOOSE_K; i++) {
    //for each vertex
    for (size_t j = 0; j < N_CHOOSE_K; j++) {

      //check if the vertex is a neighbor
      if (is_petersen_neighbor(vertices[i], PETERSON_K, vertices[j], PETERSON_K)) {

        //if there's an edge, make it the first edge in i's chain, pushing the rest down
        size_t start = neighborhoods[i];
        neighborhoods[i] = k;
        edges[k] = j;
        edges[k + 1] = start;

        k += 2;

      }
    }
  }

  print_graph(neighborhoods, N_CHOOSE_K, edges);

}
