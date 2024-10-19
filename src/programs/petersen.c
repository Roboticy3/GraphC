#include <stdio.h>
#include <stddef.h>
#include <combinations/choose.h>
#include <graph/graph.h>

//stack-allocate a Petersen graph.

#define PETERSON_K 2
#define PETERSON_N 5
#define N_CHOOSE_K 10
#define NmK_CHOOSE_K 3
#define EDGES N_CHOOSE_K * NmK_CHOOSE_K * 2

int is_petersen_neighbor(size_t* u, size_t len_u, size_t* v, size_t len_v) {
  for (size_t i = 0; i < len_u; i++) {
    for (size_t j = 0; j < len_v; j++) {
      if (u[i] == v[j]) return 0;
    }
  }

  return 1;
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
        add_neighbor(&(neighborhoods[i]), edges, j, k);

        k += 2;

      }
    }
  }

  print_graph(neighborhoods, N_CHOOSE_K, edges);

}
