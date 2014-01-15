#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "operation.h"
#include "graph.h"

//float *aco_scheduling(int ants_no, int iter_no, struct graph *op_graph, struct operation **op_array, int op_no, int jobs_no);
float *aco_scheduling(int ants_no, int iter_no, struct graph *op_graph, struct operation **op_array, int op_no, int jobs_no, float beta);
int rnd_scheduling(struct graph *op_graph, struct operation **op_array, int op_no, int jobs_no);
void add_available_nodes(int node, int *av_nodes, int *sched_list, struct graph *op_graph);
int compute_cmax(int *solution, struct operation **op_array, int jobs_no, int op_no);

#endif
