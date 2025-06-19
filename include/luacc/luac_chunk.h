#ifndef _LUAC_CHUNK_H
#define _LUAC_CHUNK_H

#include <luacc/array.h>
#include <luacc/util.h>

#include <stdio.h>

#define LUACC_BIG_ENDIAN    0
#define LUACC_LITTLE_ENDIAN 1

typedef struct
{
	// should always be 12 bytes in size in Lua 5.1 bytecode, unless it isn't Lua 5.1 bytecode
	byte_t header_bytes[12];

	size_t len;
	byte_t *bytes;

	FILE *src;
} luacc_chunk_t;

typedef struct
{
	// signature
	byte_t signature_byte;

	// 4 bytes not because it includes the signature byte, but for the null byte
	byte_t signature[4];     // SHOULD ALWAYS be "Lua", or else it isn't a valid Lua bytecode file
	
	// versions
	byte_t lua_ver;          // version in hex (e.g. 0x51 for Lua 5.1)
	byte_t fmt_ver;          // 0 OFFICIAL VER
	byte_t endianness;       // 0 big endian, 1 little endian (default is 1)
	
	// sizes
	byte_t int_size;         // default: 4
	byte_t size_t_size;      // default: 4
	byte_t instruction_size; // default: 4
	byte_t lua_number_size;  // default: 8
	byte_t integral;         // default: 0
} luacc_chunk_header_t;

luacc_chunk_t *luacc_read_chunk(FILE *luac);
void luacc_free_chunk(luacc_chunk_t *chunk);

luacc_chunk_header_t *luacc_chunk_parse_header(const luacc_chunk_t *chunk);
void luacc_free_chunk_header(luacc_chunk_header_t *header);

#endif // _LUAC_CHUNK_H
