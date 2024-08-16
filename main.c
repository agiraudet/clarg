#include "cla.h"

t_cla cla;

void initialize_cla(int argc, char **argv) {
  cla_init(&cla, argc, argv);
  cla_arg(&cla, 'v', "verbose", "Enable verbose mode");
  cla_arg(&cla, 'i', "inject", "Inject a payload and hijack entrypoint");
  cla_arg(&cla, 'p', "payload", "Specify the payload to inject");
  cla_arg(&cla, 'o', "output", "Specify the output file");
  cla_arg(&cla, 'e', "encrypt", "Encrypt the binary");
  cla_arg(&cla, 'P', "protocol", "Specify the encryption protocol to use");
}

int main(int argc, char **argv) {
  initialize_cla(argc, argv);
  cla_parse(&cla);
  if (cla_provided(&cla, 'h')) {
    cla_usage(&cla);
    return 0;
  }
  cla_debug_print(&cla);
  return 0;
}
