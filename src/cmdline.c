#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#ifdef _WIN32
    #include <Windows.h>
#else
    #error "Unsupported platform" // more supported platforms will come out in future versions!
#endif

#include "cmdline.h"
#include "config.h"

static const char **_gs_ARGS;
static size_t _gs_ARG_LEN;

void set_args(const char *args[], const size_t arg_len)
{
    _gs_ARGS = args;
    _gs_ARG_LEN = arg_len;
}

const char **get_args(void)
{
    return _gs_ARGS;
}

size_t get_arg_len(void)
{
    return _gs_ARG_LEN;
}

bool_t arg_exists(const char *arg)
{
    for (size_t i = 0; i < _gs_ARG_LEN; i++)
        if (strcmp(_gs_ARGS[i], arg) == 0)
            return TRUE;
    return FALSE;
}

void print_usage(void)
{
    printf("usage: luacc(.exe) <input files> [switches]\n");
    printf("command line switches:\n\t");
    printf("-P:                 parse only, no compiling, assembling or linking\n\t");
    printf("-S:                 parse and compile only, no assembling or linking\n\t");
    printf("-O:                 parse, compile and assemble only, no linking\n\t");
    printf("-li <linker>:       use <linker> as the linker (default is ld)\n\t");
    printf("-as <assembler>:    use <assembler> as the assembler (default is nasm)\n\t");
    printf("-l-arg <arg>:       pass <arg> as an argument to the linker\n\t");
    printf("-a-arg <arg>:       pass <arg> as an argument to the assembler\n\t");
    printf("-xl:                remove all arguments passed to the linker except arguments added with -l-arg \n\t");
    printf("-xa:                remove all arguments passed to the assembler except arguments added with -a-arg \n\t");
    printf("-v | -version:      show the compiler's version\n\t");
}

void parse_args(void)
{
    bool_t param_consumed = FALSE;
    for (size_t i = 1; i < _gs_ARG_LEN; i++)
    {
        if (param_consumed)
        {
            param_consumed = FALSE;
            continue;
        }

        const char *arg = _gs_ARGS[i];
        const char *param = i + 1 < _gs_ARG_LEN ? _gs_ARGS[i + 1] : NULL;

        if (strcmp(arg, "-li") == 0)
        {
            set_linker(param);
            param_consumed = TRUE;
            continue;
        }

        if (strcmp(arg, "-as") == 0)
        {
            set_assembler(param);
            param_consumed = TRUE;
            continue;
        }

        if (strcmp(arg, "-l-arg") == 0)
        {
            result_t result = add_linker_arg(param);
            if (IS_FAIL(result))
                fprintf(stderr, "error parsing -l-arg parameter: %s, error code %i", result.msg, result.code);
            else
                printf("passing argument %s to the linker", param);
            param_consumed = TRUE;
            continue;
        }

        if (strcmp(arg, "-a-arg") == 0)
        {
            result_t result = add_assembler_arg(param);
            if (IS_FAIL(result))
                fprintf(stderr, "error parsing -a-arg parameter: %s, error code %i", result.msg, result.code);
            else
                printf("passing argument %s to the assembler", param);
            param_consumed = TRUE;
            continue;
        }

        if (strcmp(arg, "-xl") == 0)
        {
            printf("warning: disabling linker arguments\n");
            set_linker_arg_usage(FALSE);
            continue;
        }

        if (strcmp(arg, "-xa") == 0)
        {
            printf("warning: disabling assembler arguments\n");
            set_assembler_arg_usage(FALSE);
            continue;
        }

        if (strcmp(arg, "-v") == 0 || strcmp(arg, "-version") == 0)
        {
            printf("luacc ");
            char *version_file_path;

#ifdef _WIN32
            char buf[260];
            GetModuleFileNameA(NULL, buf, 260);
            
            version_file_path = dirname(buf);
#endif // _WIN32

            strcat(version_file_path, "\\version");

            FILE *version_file = fopen(version_file_path, "r");

            char ch;
            while ((ch = fgetc(version_file)) != EOF)
                putchar(ch);
            
            fclose(version_file);
            exit(EXIT_SUCCESS);
        }

        printf("warning: invalid argument %s\n", arg);
    }
}