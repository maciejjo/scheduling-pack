#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "scheduling.h"

float *aco_scheduling(int ants_no, int iter_no, struct graph *op_graph, struct operation **op_array, int op_no, int jobs_no) {

//  printf("ACO Scheduling\n");

  float ph_init = 0.001;
  float ph_decay = 0.1;
  float ph_evapor = 0.01;
  float q_treshold = 0.8;
  float atractiveness_factor = 2.0;

  float **pheromone_array = malloc(op_graph->node_no * sizeof(float *));
  for(int i = 0; i < op_graph->node_no; i++) {
    pheromone_array[i] = malloc(op_graph->node_no * sizeof(float));
    for(int j = 0; j < op_graph->node_no; j++) {
      if(i != j)
        pheromone_array[i][j] = ph_init;
    }
  }

  float *global_cmax_array = malloc(iter_no * sizeof(int));


  for(int iter = 0; iter < iter_no; iter++) {
    printf("%d generacja\n", iter);

  int **solutions_array = malloc(ants_no * sizeof(int *));
  for(int i = 0; i < ants_no; i++) {
    solutions_array[i] = malloc(op_graph->node_no * sizeof(int));
  }
  int *cmax_array = malloc(ants_no * sizeof(int));

    for(int ant = 0; ant < ants_no; ant++) {
    printf("%d mrówka\n", ant);
      /*
      printf("Pheromones:\n");
      for(int i = 0; i < op_graph->node_no; i++) {
        printf("[PH]");
        for(int j = 0; j < op_graph->node_no; j++) {
          printf(" %.5f", pheromone_array[i][j]);
        }
        printf("\n");
      }
        printf("[PH]\n");
      */


      int *solution = malloc(op_graph->node_no * sizeof(int));
      int solution_count = 0;
      int *scheduled_list = malloc(op_graph->node_no * sizeof(int));
      for(int i = 0; i < op_graph->node_no; i++) {
        if(i == 0)   
        {
          scheduled_list[i] = 1;
          solution[0] = i;
        }
        else  
          scheduled_list[i] = 0;
      }

      int scheduling_complete = 0;
      while(!scheduling_complete) {

        int *available_nodes = malloc(op_graph->node_no * sizeof(int));
        for(int i = 0; i < op_graph->node_no; i++) {
            available_nodes[i] = 0;
        }

        for(int i = 0; i < op_graph->node_no; i++) {
          if(scheduled_list[i])
            add_available_nodes(i, available_nodes, scheduled_list, op_graph);
        }

        int chosen_node = 0;
        int av_nodes_no = 0;
        for(int i = 0; i < op_graph->node_no; i++) {
          if(available_nodes[i]) {
            av_nodes_no++;
          }
        }

        if(av_nodes_no == 1 && available_nodes[op_graph->node_no - 1])
          chosen_node = op_graph->node_no - 1;

        else {

          float rand_q = ((float)(rand()%100)/100);

          if(rand_q < q_treshold) {
            float ph_total = 0.0;
            for(int i = 1; i < op_graph->node_no; i++) {
//              printf("Sprawdzam czy %d dostępne[%d]\n", i, available_nodes[i]);
              if(available_nodes[i]) {
//                printf("%d poziom feromonów dla %d->%d : %.9f\n", solution_count, solution[solution_count], i, pheromone_array[solution[solution_count]][i]);
                float g = pheromone_array[solution[solution_count]][i] * pow((1.0 / (float) op_array[i - 1]->duration), atractiveness_factor);
                ph_total += g;
//                printf("[%d]Obecna suma ruletki: %.9f Przyrost: %.9f = %.9f * (1 / %4d) ^ %.9f\n", i, 
//                ph_total, g, pheromone_array[solution[solution_count]][i], op_array[i - 1]->duration, atractiveness_factor);
              }
            }

            float roulette_val = ((float)(rand()%1000000)/1000000)*ph_total;
            float roulette_check = 0.0;
            for(int i = 1; i < op_graph->node_no; i++) {
              if(available_nodes[i]) {
                roulette_check += pheromone_array[solution[solution_count]][i] * pow((1.0 / (float) op_array[i - 1]->duration), atractiveness_factor);
                if(roulette_val <=  roulette_check) {
//                  printf("Wybieram operację %d, gdyż %.9f <= %.9f. Suma: %.9f \n", i, roulette_val, roulette_check, ph_total);
                  chosen_node = i;
                  break;
                }
              }
            }
          }

          else {
            float best_sum = 0.0;
            int best_node = 1;
            for(int i = 1; i < op_graph->node_no; i++) {
              if(available_nodes[i]) {
                float g = pheromone_array[solution[solution_count]][i] * pow((1.0 / (float) op_array[i - 1]->duration), atractiveness_factor);
                if(g > best_sum) {
                  best_sum = g;
                  best_node = i;
                }
              }
            }

            chosen_node = best_node;
          }

        }

        for(int i = 0; i < op_graph->node_no; i++) {
          if(available_nodes[i]) {
            if(i == chosen_node) {
              solution_count++;
              solution[solution_count] = i;
              scheduled_list[i] = 1;
              break;
            }
          }
        }

        int all_scheduled = 1;
        for(int i = 0; i < op_graph->node_no; i++) {
          if(!scheduled_list[i])
            all_scheduled = 0;
        }

        free(available_nodes);

        if(all_scheduled)
          scheduling_complete = 1;

      }

      for(int i = 0; i < op_graph->node_no - 1; i++) {
        pheromone_array[solution[i]][solution[i + 1]] *= (1 - ph_evapor);
        pheromone_array[solution[i]][solution[i + 1]] += (ph_evapor * ph_init);
      }

      /*
      printf("Rozwiązanie: ");
      for(int i = 0; i < op_graph->node_no; i++) {
        printf("%d ", solution[i]);
      }
      printf("\n");
      */

      for(int i = 0; i < op_graph->node_no; i++) {
        solutions_array[ant][i] = solution[i];
      }

      int cmax = compute_cmax(solution, op_array, jobs_no, op_no);
      cmax_array[ant] = cmax;


      free(scheduled_list);
      free(solution);
    }

    for(int i = 0; i < op_graph->node_no; i++) {
      for(int j = 0; j < op_graph->node_no; j++) {
        pheromone_array[i][j] *= (1 - ph_decay);
      }
    }

    int best_sol = 0;
    for(int i = 1; i < ants_no; i++) {
      if(cmax_array[i] < cmax_array[best_sol])
        best_sol = i;
    }

    for(int i = 0; i < op_graph->node_no - 1; i++) {
      pheromone_array[solutions_array[best_sol][i]][solutions_array[best_sol][i + 1]] += ph_decay * (1.0 / cmax_array[best_sol]);
    }


    float mean_cmax = 0.0, cmax_sum = 0.0;
    for(int i = 0; i < ants_no; i++) {
      cmax_sum += (float) cmax_array[i];
      free(solutions_array[i]);
    }
    mean_cmax = cmax_sum / (float) ants_no;
    global_cmax_array[iter] = mean_cmax;
    free(solutions_array);
    free(cmax_array);

  }

  for(int i = 0; i < op_graph->node_no; i++)
    free(pheromone_array[i]);
  free(pheromone_array);

  return global_cmax_array;

}

int rnd_scheduling(struct graph *op_graph, struct operation **op_array, int op_no, int jobs_no) {
    
    
//      printf("RND Scheduling\n");

      int *solution = malloc(op_graph->node_no * sizeof(int));
      int solution_count = 0;
      int *scheduled_list = malloc(op_graph->node_no * sizeof(int));
      for(int i = 0; i < op_graph->node_no; i++) {
        if(i == 0)   
        {
          scheduled_list[i] = 1;
          solution[0] = i;
        }
        else  
          scheduled_list[i] = 0;
      }

      int scheduling_complete = 0;
      while(!scheduling_complete) {

        int *available_nodes = malloc(op_graph->node_no * sizeof(int));
        for(int i = 0; i < op_graph->node_no; i++) {
            available_nodes[i] = 0;
        }

        for(int i = 0; i < op_graph->node_no; i++) {
          if(scheduled_list[i])
            add_available_nodes(i, available_nodes, scheduled_list, op_graph);
        }

        int chosen_node = 0;
        int av_nodes_no = 0;
        for(int i = 0; i < op_graph->node_no; i++) {
          if(available_nodes[i]) {
            av_nodes_no++;
          }
        }

        int rand_node = rand() % av_nodes_no;
        int count_nodes = 0;
        for(int i = 0; i < op_graph->node_no; i++) {
          if(available_nodes[i]) {
            if(rand_node == count_nodes) {
              chosen_node = i;
              break;
            }
            count_nodes++;
          }
        }

        for(int i = 0; i < op_graph->node_no; i++) {
          if(available_nodes[i]) {
            if(i == chosen_node) {
              solution_count++;
              solution[solution_count] = i;
              scheduled_list[i] = 1;
              break;
            }
          }
        }

        int all_scheduled = 1;
        for(int i = 0; i < op_graph->node_no; i++) {
          if(!scheduled_list[i])
            all_scheduled = 0;
        }

        free(available_nodes);

        if(all_scheduled)
          scheduling_complete = 1;

      }

      int cmax = compute_cmax(solution, op_array, jobs_no, op_no);

      free(scheduled_list);
      return cmax;
    }


void add_available_nodes(int node, int *av_nodes, int *sched_list, struct graph *op_graph) {

  for(int i = 0; i < op_graph->node_no; i++) {

    if(op_graph->adj_matrix[node][i]) {
      int node_available = 0;
      if(!sched_list[i]) {
        node_available = 1;
        for(int j = 0; j < op_graph->node_no; j++) {
          if(op_graph->adj_matrix[j][i] && !sched_list[j]) {
            node_available = 0;
            break;
          }
        }
      }
      if(node_available) {
        av_nodes[i] = 1;
      }
    }
  }

}

int compute_cmax(int *solution, struct operation **op_array, int jobs_no, int op_no) {

  int **op_matrix = malloc(NUMBER_OF_MACHINES * sizeof(int *));
  for(int i = 0; i < NUMBER_OF_MACHINES; i++) {
    op_matrix[i] = malloc(jobs_no * sizeof(int));
    for(int j = 0; j < jobs_no; j++)
      op_matrix[i][j] = 0;
  }

  for(int i = 1; i < op_no + 1; i++) {
    int op = solution[i] - 1;
    int prev_op_job = 0;
    if(op_array[op]->mach > 0) {
      prev_op_job = op_matrix[op_array[op]->mach - 1][op_array[op]->job];
    }

    int prev_op_mach = 0;
    for(int j = 0; j < jobs_no; j++) {
      if(op_matrix[op_array[op]->mach][j] > prev_op_mach)
        prev_op_mach = op_matrix[op_array[op]->mach][j];
    }

    if(prev_op_job > prev_op_mach) 
      op_matrix[op_array[op]->mach][op_array[op]->job] = prev_op_job + op_array[op]->duration;
    else
      op_matrix[op_array[op]->mach][op_array[op]->job] = prev_op_mach + op_array[op]->duration;

  }
  /*
    printf("Macierz:\n");
    for(int j = 0; j < NUMBER_OF_MACHINES; j++) {
      for(int k = 0; k < jobs_no; k++) {
        printf(" %3d", op_matrix[j][k]);
      }
      printf("\n");
    }
    printf("\n");
  */

  int cmax = 0;
  for(int i = 0; i < NUMBER_OF_MACHINES; i++) {
    for(int j = 0; j < jobs_no; j++) {
      if(op_matrix[i][j] > cmax)
        cmax = op_matrix[i][j];
    }
  }

  for(int i = 0; i < NUMBER_OF_MACHINES; i++)
    free(op_matrix[i]);
  free(op_matrix);

  return cmax;
}
