#include "parser.h"
#include <sys/stat.h>

luac_unit_t read_luac(const file_t file)
{
    luac_unit_t luac;

    if (!file.file)
    {
        fprintf(stderr, "read_luac() error: invalid file\n");
        return luac;
    }

    struct stat st;
    if (stat(file.file_path, &st) == 0)
        printf("file size: %i bytes\n", st.st_size);
    else
    {
        fprintf(stderr, "read_luac() error: error reading file size\n");
        return luac;
    }
    printf("parsing luac unit %s\n", file.file_path);

    char entire_bytes[st.st_size];
    {
        int i = 0;
        char byte;
        while ((byte = fgetc(file.file)) != EOF)
        {
            if (i >= st.st_size)
            {
                fprintf(stderr, "read_luac() parse error: buffer overflow at index %i of %i bytes\n", i, st.st_size);
                return luac;
            }

            entire_bytes[i] = byte;
            i++;
        }
    }

    char bytes[st.st_size - 12]; // 12 bytes for the header

    // read the header bytes first
    printf("header:\n");
    luac_header_t header;
    header.signature_byte = entire_bytes[0];

    header.signature[0] = entire_bytes[1];
    header.signature[1] = entire_bytes[2];
    header.signature[2] = entire_bytes[3];
    printf("%c%c%c ", header.signature[0], header.signature[1], header.signature[2]);

    header.lua_version = entire_bytes[4];
    header.fmt_version = entire_bytes[5];
    header.endian = entire_bytes[6];
    printf("%x\nformat version %i, endian %i\n", header.lua_version, header.fmt_version, header.endian);

    header.int_type_size = entire_bytes[7];
    header.size_type_size = entire_bytes[8];
    header.instruction_size = entire_bytes[9];
    header.l_number_size = entire_bytes[10];
    printf("size type sizes in bytes: int: %i, size_t: %i, Instruction: %i, lua_Number: %i\n", header.int_type_size, header.size_type_size, header.instruction_size, header.l_number_size);

    header.integral = entire_bytes[11];

    printf("entire header: ");
    for (size_t i = 0; i < 12; i++)
        printf("0x%x ", entire_bytes[i]);
    putchar('\n');

    luac.header = header;

    for (size_t i = 12; i < st.st_size; i++)
    {
        
    }

    return luac;
}