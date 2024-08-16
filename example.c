#include "clarg.h"

t_cla cla;

void initialize_cla(int argc, char **argv) {
  cla_init(&cla, argc, argv);
  cla_arg(&cla, 'v', "verbose", "Enable verbose mode");
  cla_arg(&cla, 'o', "output", "Specify the output file")->value_required =
      true;
  t_clarg *encode = cla_arg(&cla, 'e', "encoding", "Specify the encoding");
  encode->value_required = true;
  clarg_add_allowed_value(encode, "utf-8");
  clarg_add_allowed_value(encode, "utf-16");
  clarg_add_allowed_value(encode, "utf-32");
  clarg_add_allowed_value(encode, "ascii");
}

int main(int argc, char **argv) {
  initialize_cla(argc, argv);
  if (cla_parse(&cla) != 0) {
    cla_destroy(&cla);
    return 1;
  }
  if (cla_provided(&cla, 'h')) {
    cla_usage(&cla);
    cla_destroy(&cla);
    return 0;
  }
  cla_debug_print(&cla);
  cla_destroy(&cla);
  return 0;
}
