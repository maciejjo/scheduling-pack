#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "interface.h"

void die(char *message) {

  if(errno != 0) perror(message);
  else fprintf(stderr, "ERROR: %s\n", message);
  exit(1);

}

int count_lines(FILE *input_file) {
  rewind(input_file);
  int line_num = 0;
  while((fscanf(input_file, "%*[^\n]"), fscanf(input_file, "%*c")) != EOF)
    ++line_num;
  rewind(input_file);
  return line_num;
}

