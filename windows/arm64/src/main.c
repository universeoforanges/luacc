#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "cmdline.h"
#include "config.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    set_args(argv, argc);
    init_config_data();

    if (argc == 1)
    {
        print_usage();
        return EXIT_FAILURE;
    }

    parse_args();

    input_files_t input_files = get_input_files();
    if (!input_files.files)
    {
        fprintf(stderr, "fatal error: no input files provided\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < input_files.len; i++)
    {
        read_luac(input_files.files[i]);
    }

    free_input(&input_files);
    free_config_data();
    return EXIT_SUCCESS;
}