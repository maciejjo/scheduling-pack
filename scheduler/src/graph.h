#ifndef GRAPH_H
#define GRAPH_H

struct graph {
  int **adj_matrix;
  int node_no;
};

struct graph *return_graph(int node_no);
void add_edge(struct graph *g, int src, int dst);
void del_graph(struct graph *g);
void print_graph(struct graph *g);

#endif

