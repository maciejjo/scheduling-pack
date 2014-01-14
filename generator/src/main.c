#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "interface.h"

#define MAX_LINE_LEN 30
#define NUMBER_OF_MACHINES 3


int main(int argc, char **argv) {

  srand(time(NULL));

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
  int line_num = 0;
  while(fgets(buff, MAX_LINE_LEN, input_file) != NULL) {
    line_num++;
    int jobs, time_min, time_max, ready_min, ready_max;
    if(sscanf(buff, "%d %d %d %d %d", &jobs, &time_min, &time_max, &ready_min, &ready_max) != 5) {
      sprintf(error, "Error parsing config on line %d", line_num);
      die(error);
    }
    for(int i = 0; i < jobs; i++) {
      int ready_time = rand() % (ready_max - ready_min + 1) + ready_min;
      int op_times[NUMBER_OF_MACHINES];
      for(int m = 0; m < NUMBER_OF_MACHINES; m++) {
        op_times[m] = rand() % (time_max - time_min + 1) + time_min;
      }

      fprintf(output_file, "%d %d %d %d\n", ready_time, op_times[0], op_times[1], op_times[2]);
    }
  }





  free(input_filename);
  fclose(input_file);
  if(output_specified) {
    free(output_filename);
    fclose(output_file);
  }
}
