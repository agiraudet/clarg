#ifndef CLA_H
#define CLA_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char short_name;
  const char *long_name;
  const char *description;
  bool provided;
  bool value_required;
  const char *value;
  char **allowed_values;
} t_clarg;

typedef struct {
  int argc;
  char **argv;
  t_clarg *args;
  size_t args_count;
  size_t args_capacity;
} t_cla;

t_cla *cla_init(t_cla *cla, int argc, char **argv);
t_clarg *clarg_init(t_clarg *clarg, char short_name, const char *long_name,
                    const char *description);
t_clarg *cla_new(t_cla *cla);
t_clarg *cla_arg(t_cla *cla, char short_name, const char *long_name,
                 const char *description);
int cla_help(char **argv);
bool clarg_match(t_clarg *clarg, const char *str);
int cla_parse(t_cla *cla);
void cla_usage(t_cla *cla);
bool cla_provided(t_cla *cla, char short_name);
t_clarg *cla_get_arg(t_cla *cla, char short_name);
void clarg_debug_print(t_clarg *clarg);
void cla_debug_print(t_cla *cla);

#endif // !CLA_H
