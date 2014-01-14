#ifndef OPERATION_H
#define OPERATION_H

struct operation {
  int duration;
  int mach;
  int job;
};

struct operation *return_operation(int d, int m, int j);

#endif
