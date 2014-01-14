#include <stdlib.h>

#include "operation.h"

struct operation *return_operation(int d, int m, int j) {
  
  struct operation *new_op = malloc(sizeof(struct operation));
  new_op->duration = d;
  new_op->mach = m;
  new_op->job = j;
  return new_op;

}

