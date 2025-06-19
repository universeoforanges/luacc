#include "luacc/log.h"
#include <luacc/luac_chunk.h>
#include <luacc/luac_util.h>
#include <luacc/array.h>

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

byte_t luacc_get_byte(const luacc_chunk_t *chunk, size_t *index)
{
	if (*index + 1 >= chunk->len)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "expected a byte, but got EOF");

	return chunk->bytes[(*index)++];
}

char *luacc_get_str(const luacc_chunk_t *chunk, size_t *index, const size_t len)
{
	if ((*index) + len >= chunk->len)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "expected a byte array with length %u, but got EOF", (uint32_t) len);

		luacc_log(LUACC_LOG_LEVEL_FATAL, buf);
	}

	char* str = malloc(sizeof(char) * len + 1); // add space for the null byte
	size_t str_len = 0;

	for (size_t i = 0; i < len; i++)
		str[str_len++] = chunk->bytes[(*index) + i];

	*index += len;
	
	str[str_len++] = '\0';

	return str;
}

luacc_lua_str_t *luacc_get_lua_str(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_lua_str_t *str = malloc(sizeof(luacc_lua_str_t));
	str->len = luacc_get_size_t(chunk, index);
	str->str = str->len > 0 ? luacc_get_str(chunk, index, str->len) : NULL;

	return str;
}

uint32_t luacc_get_u32(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);

	// size of a unsigned int should be 32 bits (4 bytes)
	byte_t *bytes = (byte_t *) luacc_get_str(chunk, index, 4);
	uint32_t res = 0;

	if (header->endianness == LUACC_BIG_ENDIAN)
	{
		for (size_t i = 0; i < 4; i++)
			res = (res << 8) | bytes[i];
	}
	else
	{
		for (size_t i = 0; i < 4; i++)
			res |= (bytes[i] << (i * 8));
	}

	free(bytes);
	luacc_free_chunk_header(header);

	return res;
}

uint64_t luacc_get_u64(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);
	
	// size of a u64 should be 64 bits (8 bytes)
	byte_t *bytes = (byte_t *) luacc_get_str(chunk, index, 8);
	uint64_t res = 0;

	if (header->endianness == LUACC_BIG_ENDIAN)
	{
		for (size_t i = 0; i < 8; i++)
			res = (res << 8) | bytes[i];
	}
	else
	{
		for (size_t i = 0; i < 8; i++)
			res |= (bytes[i] << (i * 8));
	}

	free(bytes);
	luacc_free_chunk_header(header);

	return res;
}

uint64_t luacc_get_uint_custom_size(const luacc_chunk_t *chunk, size_t *index, const size_t int_size)
{
	luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);

	// normally int_size should be 4
	byte_t *raw_bytes = (byte_t *) luacc_get_str(chunk, index, int_size);

	array_t *bytes = luacc_alloc_array();
	luacc_array_resize(bytes, sizeof(uint64_t));

	uint64_t res = 0;

	if (int_size > sizeof(res))
	{
		luacc_log(LUACC_LOG_LEVEL_ERROR, "luacc_get_uint(): unsupported int size (compile again with the -V flag to see the file's header information)");
		return 0;
	}

	if (header->endianness == LUACC_BIG_ENDIAN)
	{
		for (size_t i = 0; i < sizeof(res) - int_size; i++)
			luacc_array_append(bytes, 0);

		luacc_array_raw_insert(bytes, raw_bytes, int_size);

		for (size_t i = 0; i < int_size; i++)
			res = (res << 8) | *((byte_t *) bytes->data[i]);
	}
	else
	{
		luacc_array_raw_insert(bytes, raw_bytes, int_size);

		for (size_t i = 0; i < bytes->size - int_size; i++)
			luacc_array_append(bytes, 0);

		for (size_t i = 0; i < int_size; i++)
			res |= (*((byte_t *) bytes->data[i]) << (i * 8));
	}

	free(raw_bytes);
	luacc_free_array(bytes);
	luacc_free_chunk_header(header);

	return res;
}

uint64_t luacc_get_uint(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);
	uint64_t res = luacc_get_uint_custom_size(chunk, index, header->int_size);

	luacc_free_chunk_header(header);

	return res;
}

size_t luacc_get_size_t(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);
	size_t res = (size_t) luacc_get_uint_custom_size(chunk, index, header->size_t_size);

	luacc_free_chunk_header(header);

	return res;
}

double luacc_get_double(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);
	
	// size of lua_Number (which is basically just a double) should usually be 8 bytes
	uint64_t uint = luacc_get_uint_custom_size(chunk, index, header->lua_number_size);
	double res = 0.0;

	memcpy(&res, &uint, sizeof(res));

	luacc_free_chunk_header(header);

	return res;
}

void luacc_lua_str_free(luacc_lua_str_t *str)
{
	if (str->str) free(str->str);
	free(str);
}
