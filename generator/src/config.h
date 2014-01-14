#ifndef CONFIG_H
#define CONFIG_H

struct config {

  char *input_filename, *output_filename;
  int output_specified;

};

struct config *return_config(int out_specified, char *in_filename, char *out_filename);

#endif
