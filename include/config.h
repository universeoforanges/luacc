#ifndef CONFIG_H
#define CONFIG_H

#include "util.h"

void init_config_data(void);
void free_config_data(void);

void set_linker(const char *linker);
const char *get_linker(void);

void set_assembler(const char *assembler);
const char *get_assembler(void);

void set_linker_arg_usage(const bool_t do_usage);
bool_t get_linker_arg_usage(void);

void set_assembler_arg_usage(const bool_t do_usage);
bool_t get_assembler_arg_usage(void);

result_t add_linker_arg(const char *arg);
const char **get_linker_args(void);

result_t add_assembler_arg(const char *arg);
const char **get_assembler_args(void);

#endif