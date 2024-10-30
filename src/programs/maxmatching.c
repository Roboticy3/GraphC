#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <combinations/choose.h>
#include <programs/maxmatching.h>
#include <graph/graph.h>

#define MAXMATCHING_ARGS ":p:n:s"

struct maxmatching_args {
  int n;
  float p;
  int error;
};

struct maxmatching_args process_maxmatching_args(int argc, char** argv) {
  int opt;
  int n = 0;
  float p = 1.0;
  while((opt = getopt(argc, argv, MAXMATCHING_ARGS)) != -1) {
    switch(opt)  {  
      case 'n':
        n = atoi(optarg);
        break;
      case 'p':
        p = atof(optarg);
        if (p > 1.0 || p < 0.0) {
          return (struct maxmatching_args){n, p, 3};
        }
        break;
      case '?':
        printf("argument %c is undefined\n", optopt);
        return (struct maxmatching_args){n, p, 1};
      case ':':
        printf("argument %c requires a value\n", optopt);
        return (struct maxmatching_args){n, p, 2};
    }  
  }

  return (struct maxmatching_args){n, p, 0};
}

int maxmatching(int argc, char** argv) {

  srand(time(NULL));

  struct maxmatching_args a = process_maxmatching_args(argc, argv);

  if (a.error) return a.error;

  printf("using %d vertices...\n", a.n);
  printf("probability %f is out of range (0.0 to 1.0 inclusive)\n", a.p);

  size_t max_edges = choose(a.n, 2);
  printf("using %ld edges...\n", max_edges);

  printf("estimated memory cost: %ld bits.\n", sizeof(Neighbor*) * a.n + sizeof(Neighbor) * max_edges * 2);

  Neighbor** vertices = calloc(a.n, sizeof(Neighbor*));
  Neighbor* edges = calloc(max_edges * 2, sizeof(Neighbor));

  NeighborhoodGraph g = {vertices, a.n, edges, 0};

  fill_graph_random(&g, a.p);

  print_graph(g);

  free(vertices);
  free(edges);

  return 0;
}