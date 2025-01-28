#include <stdlib.h>
#include "input.h"
#include "cmdline.h"
#include "util.h"

file_t open_file(const char *file, const char *mode)
{
    file_t f;
    f.file = fopen(file, mode);
    f.file_path = file;
    return f;
}

input_files_t get_input_files(void)
{
    input_files_t input_files;
    input_files.files = NULL;
    input_files.len = 0;

    const char **args = get_args();

    size_t arg_len = get_arg_len();
    size_t array_size = 0;
    size_t next_index = 0;

    if (arg_len == 1)
    {
        fprintf(stderr, "get_input_files(): no input files provided\n");

        input_files.files = NULL;
        input_files.len = 0;
        return input_files;
    }

    for (size_t i = 1; i < arg_len; i++)
    {
        const char *file_path = args[i];

        file_t file = open_file(file_path, "r");
        if (!file.file) continue;
        
        array_size += sizeof(file);
        input_files.files = input_files.files ? realloc(input_files.files, array_size) : malloc(array_size);
        input_files.files[next_index++] = file;

        printf("get_input_files(): #%i: found input file %s\n", next_index - 1, file_path);
        input_files.len++;
    }

    return input_files;
}

void free_input(input_files_t *input_files)
{
    for (size_t i = 0; i < input_files->len; i++)
    {
        FILE *file = input_files->files[i].file;
        if (!file) continue;
        
        input_files->files[i].file = NULL;
        input_files->files[i].file_path = "";
        fclose(file);
    }
    
    free(input_files->files);
    input_files->files = NULL;
    input_files->len = 0;
}