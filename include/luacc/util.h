#ifndef _UTIL_H
#define _UTIL_H

#include <luacc/array.h>

// normal typedefs. what did you expect?
typedef unsigned char     byte_t;
typedef byte_t            bool_t;

// sketchy macros
#define LUACC_TRUE        ((bool_t)1)
#define LUACC_FALSE       ((bool_t)0)

int luacc_get_bits(const int n, const int idx, const int size);

void luacc_read_input_files(array_t *output_array, int argc, char **argv);
void luacc_parse_chunks(const array_t *chunks, array_t *func_chunks);

#endif // _UTIL_H
