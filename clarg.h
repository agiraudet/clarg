#ifndef CLA_H
#define CLA_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  const char **data;
  size_t size;
  size_t capacity;
} t_cla_vector;

typedef struct {
  char short_name;
  const char *long_name;
  const char *description;
  bool provided;
  bool value_required;
  const char *value;
  t_cla_vector allowed_values;
} t_clarg;

typedef struct {
  int i;
  int argc;
  char **argv;
  t_clarg *args;
  size_t args_count;
  size_t args_capacity;
  t_cla_vector inputs;
} t_cla;

// BUILD
t_cla *cla_init(t_cla *cla, int argc, char **argv);
void cla_destroy(t_cla *cla);
t_clarg *clarg_init(t_clarg *clarg, char short_name, const char *long_name,
                    const char *description);
t_clarg *cla_new(t_cla *cla);
t_clarg *cla_arg(t_cla *cla, char short_name, const char *long_name,
                 const char *description);
t_clarg *clarg_add_allowed_value(t_clarg *clarg, const char *value);

// MANAGE
int cla_parse(t_cla *cla);

// INSPECT
void cla_usage(t_cla *cla);
bool cla_provided(t_cla *cla, char short_name);
t_clarg *cla_get(t_cla *cla, char short_name);
const char *cla_value(t_cla *cla, char short_name);

// DEBUG
void clarg_debug_print(t_clarg *clarg);
void cla_debug_print(t_cla *cla);

#endif // !CLA_H
