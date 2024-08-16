#include "cla.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_cla *cla_init(t_cla *cla, int argc, char **argv) {
  if (!cla) {
    return NULL;
  }
  cla->argc = argc;
  cla->argv = argv;
  cla->args_count = 0;
  cla->args_capacity = 1;
  cla->args = (t_clarg *)calloc(cla->args_capacity, sizeof(t_clarg));
  if (!cla->args) {
    return NULL;
  }
  cla_arg(cla, 'h', "help", "Print this help message");
  return cla;
}

t_clarg *clarg_init(t_clarg *clarg, char short_name, const char *long_name,
                    const char *description) {
  clarg->short_name = short_name;
  clarg->long_name = long_name;
  clarg->description = description;
  clarg->provided = false;
  clarg->value_required = false;
  clarg->value = NULL;
  clarg->allowed_values = NULL;
  return clarg;
}

t_clarg *cla_new(t_cla *cla) {
  if (!cla)
    return NULL;
  if (cla->args_count >= cla->args_capacity) {
    cla->args_capacity *= 2;
    cla->args =
        (t_clarg *)realloc(cla->args, cla->args_capacity * sizeof(t_clarg));
    if (!cla->args)
      return NULL;
  }
  return cla->args + cla->args_count++;
}

t_clarg *cla_arg(t_cla *cla, char short_name, const char *long_name,
                 const char *description) {
  return clarg_init(cla_new(cla), short_name, long_name, description);
}

bool clarg_match(t_clarg *clarg, const char *str) {
  bool ret = false;
  if (str[0] == '-' && str[1] == clarg->short_name)
    ret = true;
  const char *equal = strchr(str, '=');
  if (equal) {
    if (clarg->long_name &&
        strncmp(clarg->long_name, str + 2, equal - str - 2) == 0) {
      clarg->value = equal + 1;
      ret = true;
    }
  } else {
    if (clarg->long_name && strcmp(clarg->long_name, str + 2) == 0)
      ret = true;
  }
  if (ret)
    clarg->provided = true;
  return ret;
}

int cla_parse(t_cla *cla) {
  if (!cla)
    return -1;
  for (int i = 1; i < cla->argc; i++) {
    bool found = false;
    for (int j = 0; j < cla->args_count; j++) {
      if (clarg_match(cla->args + j, cla->argv[i])) {
        if (cla->args[j].value_required && !cla->args[j].value) {
          if (i + 1 >= cla->argc) {
            fprintf(stderr, "Error: %s requires a value\n", cla->argv[i]);
            return -1;
          }
          cla->args[j].value = cla->argv[++i];
        }
        found = true;
        break;
      }
    }
    if (!found) {
      fprintf(stderr, "Error: Unknown option: %s\n", cla->argv[i]);
      return -1;
    }
    return -1;
  }
  return 0;
}

t_clarg *cla_get(t_cla *cla, char short_name) {
  if (!cla)
    return NULL;
  for (int i = 0; i < cla->args_count; i++) {
    if (cla->args[i].short_name == short_name)
      return cla->args + i;
  }
  return NULL;
}

bool cla_provided(t_cla *cla, char short_name) {
  if (!cla)
    return NULL;
  t_clarg *clarg = cla_get(cla, short_name);
  if (!clarg)
    return false;
  return clarg->provided;
}

void cla_usage(t_cla *cla) {
  printf("Usage: %s [OPTIONS]\n", cla->argv[0]);
  printf("Options:\n");
  for (int i = 0; i < cla->args_count; i++) {
    printf("  ");
    if (cla->args[i].short_name) {
      printf("-%c, ", cla->args[i].short_name);
      if (cla->args[i].value_required)
        printf("<option>");
    }
    if (cla->args[i].long_name) {
      printf("--%s", cla->args[i].long_name);
      if (cla->args[i].value_required)
        printf("=<option>");
    }
    printf("\t\t%s\n", cla->args[i].description);
  }
}

void clarg_debug_print(t_clarg *clarg) {
  printf("--------------\n");
  printf("short_name: %c\n", clarg->short_name);
  printf("long_name: %s\n", clarg->long_name);
  printf("description: %s\n", clarg->description);
  printf("provided: %d\n", clarg->provided);
  printf("value_required: %d\n", clarg->value_required);
  printf("value: %s\n", clarg->value);
  printf("allowed_values: %p\n", clarg->allowed_values);
}

void cla_debug_print(t_cla *cla) {
  printf("argc: %d\n", cla->argc);
  for (int i = 0; i < cla->args_count; i++) {
    clarg_debug_print(cla->args + i);
  }
}
