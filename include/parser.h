#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "util.h"
#include "input.h"

typedef struct luac_header
{
    // signature
    byte_t signature_byte;
    byte_t signature[3];

    // versions
    byte_t lua_version;
    byte_t fmt_version; // format version
    byte_t endian; // default = 1 (little endian)

    // type sizes
    byte_t int_type_size; // default = 4
    byte_t size_type_size; // default = 4
    byte_t instruction_size; // default = 4
    byte_t l_number_size; // default = 8

    byte_t integral; // default = 0 (FALSE)
} luac_header_t;

typedef struct luac_unit
{
    luac_header_t header;
    const char *bytes; // all bytes after the header
} luac_unit_t;
//void free_luac_unit(luac_unit_t* luac_unit);

luac_unit_t read_luac(const file_t file);
//const char *parse_luac(const luac_unit_t luac);

#endif // PARSER_H