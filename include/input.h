#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdint.h>

typedef struct file
{
    FILE *file;
    char *file_path;
} file_t;
file_t open_file(const char *file, const char *mode);

typedef struct input_files
{
    file_t *files;
    size_t len;
} input_files_t;

input_files_t get_input_files(void);
void free_input(input_files_t *input_files);

#endif // INPUT_H