#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "interface.h"
#include "operation.h"
#include "graph.h"
#include "scheduling.h"


int main(int argc, char **argv) {

  srand(time(NULL));
  int ants_no = 10;
  int iter_no = 5000;
  int iter_avg = 3;

  int jobs_no;

  char *input_filename = NULL, 
       *output_filename = NULL;
  int output_specified = 0;

  {
    char c, error[64];
    if(argc < 2) {
      sprintf(error, "Usage: %s -i input_file [ -o output_file ]", argv[0]);
      die(error);
    }

    while((c = getopt(argc, argv, "i:o:")) != -1) {
      switch(c) {
        case 'i':
          if(!(input_filename = malloc((strlen(optarg) + 1) * sizeof(char)))) die("Memory allocation");
          strcpy(input_filename, optarg);
          break;
        case 'o':
          output_specified = 1;
          if(!(output_filename = malloc((strlen(optarg) + 1) * sizeof(char)))) die("Memory allocation");
          strcpy(output_filename, optarg);
          printf("Output specified: %s\n", output_filename);
          break;
        case '?':
        default:
          sprintf(error, "Usage: %s -i input_file [ -o output_file ]", argv[0]);
          die(error);
          break;
      }
    }
  }

  FILE *input_file, *output_file;
  if(!(input_file = fopen(input_filename, "r"))) die("Opening config file");
  if(output_specified) {
    if(!(output_file = fopen(output_filename, "w+"))) die("Opening output file");
  }
  else output_file = stdout;

  char buff[MAX_LINE_LEN];
  char error[64];
  jobs_no = count_lines(input_file);
  printf("Reading jobs file '%s': %d jobs\n", input_filename, jobs_no);

  int op_no = jobs_no * NUMBER_OF_MACHINES;
  struct operation **op_array = malloc(op_no * sizeof(struct operation));

  int line_num = 0;
  while(fgets(buff, MAX_LINE_LEN, input_file) != NULL) {
    int ready_time, op_times[NUMBER_OF_MACHINES];
    if(sscanf(buff, "%d %d %d %d", &ready_time, &op_times[0], &op_times[1], &op_times[2]) != 4) {
      sprintf(error, "Error parsing jobs file on line %d", line_num + 1);
      die(error);
    }
    for(int i = 0; i < NUMBER_OF_MACHINES; i++) {
      op_array[3 * line_num + i] = return_operation(op_times[i], i, line_num);
    }
    line_num++;
  }

  struct graph *op_graph = return_graph(op_no + 2);
  for(int i = 0; i < op_no; i++) {
    if(op_array[i]->mach == 0) {
      add_edge(op_graph, 0, i + 1);
      add_edge(op_graph, i + 1, i + 2);
    } else if(op_array[i]->mach == NUMBER_OF_MACHINES - 1) {
      add_edge(op_graph, i + 1, op_no + 1);
    } else {
      add_edge(op_graph, i + 1, i + 2);
    }
  }
  
  /*
  float *rnd_results = malloc(iter_no * sizeof(int));
  printf("RND scheduling - %d iterations: ", iter_no);
  for(int i = 0; i < iter_no; i++) {
    printf("%d... ", i);
    int *rnd_avg_results = malloc(iter_avg * sizeof(int));
    for(int j = 0; j < iter_avg; j++) {
      rnd_avg_results[j] = rnd_scheduling(op_graph, op_array, op_no, jobs_no);
    }
    int result_sum = 0;
    for(int j = 0; j < iter_avg; j++) {
      result_sum += rnd_avg_results[j];
    }
    rnd_results[i] = (float) result_sum / (float) iter_avg;
  }
  printf("\n");
  */

  float **global_results = malloc(iter_no * sizeof(float *));
  for(int i = 0; i < iter_no; i++) global_results[i] = malloc(4 * sizeof(float));

  float beta[4] = { 0.0, 2.0, 4.0, 6.0 };
  for(int k = 0; k < 4; k++) {

    float **aco_results = malloc(iter_no * sizeof(float *));
    
    printf("ACO scheduling - %d iterations\n", iter_no);
    for(int i = 0; i < iter_avg; i++) {
      printf("Avg: %d\n", i);
      aco_results[i] = aco_scheduling(ants_no, iter_no, op_graph, op_array, op_no, jobs_no, beta[k]);
    }

    for(int i = 0; i < iter_no; i++) {
      float aco_avg = 0.0;
      for(int j = 0; j < iter_avg; j++)
        aco_avg += aco_results[j][i];
      aco_avg /= (float) iter_avg;
      global_results[i][k] = aco_avg;
    }
    free(aco_results);
  }

  float ideal_cmax = 0.0;
  for(int i = 0; i < op_no; i++)
    ideal_cmax += op_array[i]->duration;
  ideal_cmax /= (float) NUMBER_OF_MACHINES;



  for(int i = 0; i < iter_no; i++) {
    fprintf(output_file, "%d", i);
    for(int j = 0; j < 4; j++) {
    fprintf(output_file, ",%.3f", global_results[i][j]);
    }
    fprintf(output_file, ",%.3f\n", ideal_cmax);
  }

  // cleanup
  del_graph(op_graph);
  for(int i = 0; i < op_no; i++) {
    free(op_array[i]);
  }
  free(op_array);
  free(input_filename);
  fclose(input_file);
  if(output_specified) {
    free(output_filename);
    fclose(output_file);
  }
}
