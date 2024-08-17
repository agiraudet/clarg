#include "clarg.h"

void initialize_cla(int argc, char **argv) {
  cla_init(argc, argv);
  cla_add_description("This is a simple example program");
  cla_add_required_input("SOURCE");
  cla_arg('d', "debug", "Show debug information");
  cla_arg('v', "verbose", "Enable verbose mode");
  cla_arg('o', "output", "Specify the output file")->value_required = true;
  t_clarg *encode = cla_arg('e', "encoding", "Specify the encoding");
  encode->value_required = true;
  clarg_add_allowed_value(encode, "utf-8");
  clarg_add_allowed_value(encode, "utf-16");
  clarg_add_allowed_value(encode, "utf-32");
  clarg_add_allowed_value(encode, "ascii");
}

int main(int argc, char **argv) {
  initialize_cla(argc, argv);
  if (cla_parse() < 0)
    return 1;
  if (cla_provided('h')) {
    cla_usage();
    return 0;
  }
  if (cla_provided('d'))
    cla_debug_print();
  return 0;
}
