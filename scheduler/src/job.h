#ifndef OPERATION_H
#define OPERATION_H

struct operation {
  int duration;
  int machine_no;
  int job_no;
}

struct operation *return_operation(int d, int m, int j) {
  
  struct operation *new_op = malloc(sizeof(operation));
  new_op->duration = d;
  new_op->machine_no = m;
  new_op->job_no = j;
  return new_op;

}
