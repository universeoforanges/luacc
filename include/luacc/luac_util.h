#ifndef _LUAC_UTIL_H
#define _LUAC_UTIL_H

#include <luacc/luac_chunk.h>
#include <luacc/util.h>

#include <stddef.h>
#include <stdint.h>

typedef struct
{
	size_t len;
	char *str;
} luacc_lua_str_t;

// all of these functions increment an index variable with the size.. idk how to explain it like wth

byte_t luacc_get_byte(const luacc_chunk_t *chunk, size_t *index);
char *luacc_get_str(const luacc_chunk_t *chunk, size_t *index, const size_t len); // MAKE SURE TO FREE THE STRING
luacc_lua_str_t *luacc_get_lua_str(const luacc_chunk_t *chunk, size_t *index); // ALSO MAKE SURE TO FREE

// more advanced stuff, ig

uint32_t luacc_get_u32(const luacc_chunk_t *chunk, size_t *index);
uint64_t luacc_get_u64(const luacc_chunk_t *chunk, size_t *index);
uint64_t luacc_get_uint_custom_size(const luacc_chunk_t *chunk, size_t *index, const size_t int_size);
uint64_t luacc_get_uint(const luacc_chunk_t *chunk, size_t *index);
size_t luacc_get_size_t(const luacc_chunk_t *chunk, size_t *index);
double luacc_get_double(const luacc_chunk_t *chunk, size_t *index);

// js even more boring memory management...

void luacc_lua_str_free(luacc_lua_str_t *str);

#endif // _LUAC_UTIL_H
