#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clarg.h"

static int cla_vector_init(t_cla_vector *vec, size_t capacity) {
  vec->size = 0;
  vec->capacity = capacity;
  if (capacity == 0)
    return vec->data = NULL, 0;
  else
    vec->data = (const char **)calloc(capacity, sizeof(char *));
  return 0;
}

static int cla_vector_pushback(t_cla_vector *vec, const char *str) {
  if (!vec)
    return -1;
  if (vec->size >= vec->capacity) {
    if (vec->capacity == 0)
      vec->capacity = 1;
    else
      vec->capacity *= 2;
    vec->data =
        (const char **)realloc(vec->data, vec->capacity * sizeof(char *));
    if (!vec->data)
      return -1;
  }
  vec->data[vec->size++] = str;
  return 0;
}

static const char *cla_vector_at(t_cla_vector *vec, size_t index) {
  if (!vec || index >= vec->size)
    return NULL;
  return vec->data[index];
}

static const char *cla_vector_popfront(t_cla_vector *vec) {
  if (!vec || vec->size == 0)
    return NULL;
  const char *str = vec->data[0];
  for (size_t i = 0; i < vec->size - 1; i++) {
    vec->data[i] = vec->data[i + 1];
  }
  vec->size--;
  return str;
}

static bool cla_vector_empty(t_cla_vector *vec) { return vec->size == 0; }

bool cla_vector_in(t_cla_vector *vec, const char *str) {
  for (size_t i = 0; i < vec->size; i++) {
    if (strcmp(vec->data[i], str) == 0)
      return true;
  }
  return false;
}

static const char *cla_vector_popback(t_cla_vector *vec) {
  if (!vec || vec->size == 0)
    return NULL;
  return vec->data[--vec->size];
}

void cla_destroy(t_cla *cla) {
  if (!cla)
    return;
  if (cla->args)
    free(cla->args);
  if (cla->inputs.data)
    free(cla->inputs.data);
}

static void clarg_destroy(t_clarg *clarg) {
  if (!clarg)
    return;
  if (clarg->allowed_values.capacity)
    free(clarg->allowed_values.data);
}

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
  cla_vector_init(&cla->inputs, 1);
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
  cla_vector_init(&clarg->allowed_values, 0);
  return clarg;
}

t_clarg *clarg_add_allowed_value(t_clarg *clarg, const char *value) {
  if (!clarg)
    return NULL;
  cla_vector_pushback(&clarg->allowed_values, value);
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

static int cla_match_shortname(t_cla *cla, const char *str) {
  str++;
  while (*str) {
    bool found = false;
    for (size_t i = 0; !found && i < cla->args_count; i++) {
      if (*str == cla->args[i].short_name) {
        t_clarg *clarg = cla->args + i;
        clarg->provided = true;
        if (clarg->value_required) {
          if (cla->i + 1 >= cla->argc) {
            fprintf(stderr, "Option -%c requires an argument\n", *str);
            return -1;
          }
          clarg->value = cla->argv[cla->i + 1];
          cla->i++;
        }
        found = true;
      }
    }
    if (!found) {
      fprintf(stderr, "Unknown option: -%c\n", *str);
      return -1;
    }
    str++;
  }
  return 0;
}

static int cla_match_longname(t_cla *cla, const char *str) {
  str += 2;
  const char *equal = strchr(str, '=');
  for (size_t i = 0; i < cla->args_count; i++) {
    t_clarg *clarg = cla->args + i;
    if (equal) {
      if (strncmp(clarg->long_name, str, equal - str) == 0) {
        clarg->provided = true;
        clarg->value = equal + 1;
        if (clarg->allowed_values.size > 0 &&
            !cla_vector_in(&clarg->allowed_values, clarg->value)) {
          fprintf(stderr, "Invalid value for --%s: %s\n", clarg->long_name,
                  clarg->value);
          return -1;
        }

        return 0;
      }
    } else {
      if (strcmp(clarg->long_name, str) == 0) {
        clarg->provided = true;
        if (clarg->value_required) {
          if (cla->i + 1 >= cla->argc) {
            fprintf(stderr, "Option --%s requires an argument\n", str);
            return -1;
          }
          clarg->value = cla->argv[cla->i + 1];
          cla->i++;
        }
        return 0;
      }
    }
  }
  fprintf(stderr, "Unknown option: --%s\n", str);
  return -1;
}

int cla_parse(t_cla *cla) {
  if (!cla)
    return -1;
  cla->i = 1;
  while (cla->i < cla->argc) {
    const char *arg = cla->argv[cla->i];
    if (arg[0] && arg[0] == '-' && arg[1] != '-') {
      if (cla_match_shortname(cla, arg) < 0)
        return -1;
    } else if (arg[0] && arg[0] == '-' && arg[1] == '-') {
      if (cla_match_longname(cla, arg) < 0)
        return -1;
    } else {
      cla_vector_pushback(&cla->inputs, arg);
    }
    cla->i++;
  }
  return 0;
}

t_clarg *cla_get(t_cla *cla, char short_name) {
  if (!cla)
    return NULL;
  for (size_t i = 0; i < cla->args_count; i++) {
    if (cla->args[i].short_name == short_name)
      return cla->args + i;
  }
  return NULL;
}

const char *cla_value(t_cla *cla, char short_name) {
  if (!cla)
    return NULL;
  t_clarg *clarg = cla_get(cla, short_name);
  if (!clarg)
    return NULL;
  return clarg->value;
}

bool cla_provided(t_cla *cla, char short_name) {
  if (!cla)
    return NULL;
  t_clarg *clarg = cla_get(cla, short_name);
  if (!clarg)
    return false;
  return clarg->provided;
}

static int cla_longest_cmd(t_cla *cla) {
  int max_len = 0;
  char buffer[256];
  for (size_t i = 0; i < cla->args_count; i++) {
    int len = 2;
    if (cla->args[i].short_name) {
      len += snprintf(buffer, sizeof(buffer), "-%c", cla->args[i].short_name);
      if (cla->args[i].value_required)
        len += snprintf(buffer, sizeof(buffer), " <option>");
      len += snprintf(buffer, sizeof(buffer), ", ");
    }
    if (cla->args[i].long_name) {
      len += snprintf(buffer, sizeof(buffer), "--%s", cla->args[i].long_name);
      if (cla->args[i].value_required)
        len += snprintf(buffer, sizeof(buffer), "=<option>");
    }
    if (len > max_len)
      max_len = len;
  }
  return max_len;
}

void cla_usage(t_cla *cla) {
  printf("Usage: %s [OPTIONS]\n", cla->argv[0]);
  printf("Options:\n");
  int max_len = cla_longest_cmd(cla);
  for (size_t i = 0; i < cla->args_count; i++) {
    int written = 0;
    written += printf("  ");
    if (cla->args[i].short_name) {
      written += printf("-%c", cla->args[i].short_name);
      if (cla->args[i].value_required)
        written += printf(" <option>");
      written += printf(", ");
    }
    if (cla->args[i].long_name) {
      written += printf("--%s", cla->args[i].long_name);
      if (cla->args[i].value_required)
        written += printf("=<option>");
    }
    printf("%*s %s\n", max_len - written, "", cla->args[i].description);
  }
}

// void cla_usage(t_cla *cla) {
//   int max_len = 0;
//   char buffer[256];
//   for (size_t i = 0; i < cla->args_count; i++) {
//     int len = 0;
//     if (cla->args[i].short_name) {
//       len += snprintf(buffer, sizeof(buffer), "-%c",
//       cla->args[i].short_name); if (cla->args[i].value_required)
//         len += snprintf(buffer, sizeof(buffer), " <option>");
//       len += snprintf(buffer, sizeof(buffer), ", ");
//     }
//     if (cla->args[i].long_name) {
//       len += snprintf(buffer, sizeof(buffer), "--%s",
//       cla->args[i].long_name); if (cla->args[i].value_required)
//         len += snprintf(buffer, sizeof(buffer), "=<option>");
//     }
//     if (len > max_len)
//       max_len = len;
//   }
//   printf("Usage: %s [OPTIONS]\n", cla->argv[0]);
//   printf("Options:\n");
//   for (size_t i = 0; i < cla->args_count; i++) {
//     if (cla->args[i].short_name) {
//       printf("-%c", cla->args[i].short_name);
//       if (cla->args[i].value_required)
//         printf(" <option>");
//       printf(", ");
//     }
//     if (cla->args[i].long_name) {
//       printf("--%s", cla->args[i].long_name);
//       if (cla->args[i].value_required)
//         printf("=<option>");
//     }
//     printf("%*s%s\n", (int)(max_len - strlen(buffer)), "",
//            cla->args[i].description);
//   }
// }

void clarg_debug_print(t_clarg *clarg) {
  printf("--------------\n");
  printf("short_name: %c\n", clarg->short_name);
  printf("long_name: %s\n", clarg->long_name);
  printf("description: %s\n", clarg->description);
  printf("provided: %d\n", clarg->provided);
  printf("value_required: %d\n", clarg->value_required);
  printf("value: %s\n", clarg->value);
  printf("allowed_values: ");
  for (size_t i = 0; i < clarg->allowed_values.size; i++) {
    printf("%s, ", clarg->allowed_values.data[i]);
  }
  if (clarg->allowed_values.size > 0)
    printf("\n");
  else
    printf("Any\n");
}

void cla_debug_print(t_cla *cla) {
  printf("argc: %d\n", cla->argc);
  for (size_t i = 0; i < cla->inputs.size; i++) {
    printf("input: %s\n", cla_vector_at(&cla->inputs, i));
  }
  for (size_t i = 0; i < cla->args_count; i++) {
    clarg_debug_print(cla->args + i);
  }
}
