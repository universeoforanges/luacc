#ifndef CMDLINE_H
#define CMDLINE_H

#include "util.h"

void set_args(const char *args[], const size_t arg_len);

const char **get_args(void);
size_t get_arg_len(void);

bool_t arg_exists(const char *arg);

void print_usage(void);

void parse_args(void);

#endif // CMDLINE_H