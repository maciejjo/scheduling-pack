#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "interface.h"

void die(char *message) {

  if(errno != 0) perror(message);
  else fprintf(stderr, "ERROR: %s\n", message);
  exit(1);

}
