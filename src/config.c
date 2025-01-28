#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "config.h"

static struct
{
    const char *linker;
    const char *assembler;

    bool_t do_default_linker_args;
    bool_t do_default_asm_args;

    char **linker_args;
    size_t linker_arg_len;

    char **asm_args;
    size_t asm_arg_len;
} _gs_conf;

void init_config_data(void)
{
    _gs_conf.linker = "ld";
    _gs_conf.assembler = "nasm";

    _gs_conf.do_default_linker_args = TRUE;
    _gs_conf.do_default_asm_args = TRUE;

    _gs_conf.linker_args = NULL;
    _gs_conf.asm_args = NULL;
}

void free_config_data(void)
{
    free(_gs_conf.linker_args);
    free(_gs_conf.asm_args);
}

void set_linker(const char *linker)
{
    _gs_conf.linker = linker;
}

const char *get_linker(void)
{
    return _gs_conf.linker;
}

void set_assembler(const char *assembler)
{
    _gs_conf.assembler = assembler;
}

const char *get_assembler(void)
{
    return _gs_conf.assembler;
}

void set_linker_arg_usage(const bool_t do_usage)
{
    _gs_conf.do_default_linker_args = do_usage;
}

bool_t get_linker_arg_usage(void)
{
    return _gs_conf.do_default_linker_args;
}

void set_assembler_arg_usage(const bool_t do_usage)
{
    _gs_conf.do_default_asm_args = do_usage;
}

bool_t get_assembler_arg_usage(void)
{
    return _gs_conf.do_default_asm_args;
}

result_t add_linker_arg(const char *arg)
{
    result_t result = success_res();

    _gs_conf.linker_arg_len++;
    char **linker_args = _gs_conf.linker_args ? realloc(_gs_conf.linker_args, sizeof(_gs_conf.linker_args) + sizeof(arg)) : malloc(sizeof(arg));
    if (!linker_args)
    {
        result.code = errno;
        result.msg = "memory allocation failure";

        fprintf(stderr, "%s\n", result.msg);
        return result;
    }

    linker_args[_gs_conf.linker_arg_len - 1] = arg;
    _gs_conf.linker_args = linker_args;
    return result;
}

const char **get_linker_args(void)
{
    return _gs_conf.linker_args;
}

result_t add_assembler_arg(const char *arg)
{
    result_t result = success_res();

    _gs_conf.asm_arg_len++;
    char **asm_args = _gs_conf.linker_args ? realloc(_gs_conf.asm_args, sizeof(_gs_conf.asm_args) + sizeof(arg)) : malloc(sizeof(arg));
    if (!asm_args)
    {
        result.code = errno;
        result.msg = "memory allocation failure";

        fprintf(stderr, "%s\n", result.msg);
        return result;
    }

    asm_args[_gs_conf.linker_arg_len - 1] = arg;
    _gs_conf.asm_args = asm_args;
    return result;
}

const char **get_assembler_args(void)
{
    return _gs_conf.asm_args;
}