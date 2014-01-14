#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

struct graph *return_graph(int node_no) {
  struct graph *new_graph = malloc(sizeof(struct graph));
  new_graph->node_no = node_no;
  new_graph->adj_matrix = malloc(node_no * sizeof(int *));
  for(int i = 0; i < node_no; i++) {
      new_graph->adj_matrix[i] = malloc(node_no * sizeof(int));
      for(int j = 0; j < node_no; j++) {
        new_graph->adj_matrix[i][j] = 0;
      }
  }
  return new_graph;
}

void add_edge(struct graph *g, int src, int dst) {

  g->adj_matrix[src][dst] = 1;

}

void del_graph(struct graph *g) {

  for(int i = 0; i < g->node_no; i++) {
    free(g->adj_matrix[i]);
  }
  free(g->adj_matrix);
  free(g);
}

void print_graph(struct graph *g) {

  printf("Graph:\n");
  for(int i = 0; i < g->node_no; i++) {
    for(int j = 0; j < g->node_no; j++) {
      printf("%3d", g->adj_matrix[i][j]);
    }
    printf("\n");
  }

}

