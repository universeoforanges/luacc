#include <luacc/luac_chunk.h>
#include <luacc/log.h>

#include <stdlib.h>

luacc_chunk_t *luacc_read_chunk(FILE *luac)
{
	if (!luac)
	{
		luacc_log(LUACC_LOG_LEVEL_ERROR, "luacc_read_chunk(): attempt to read a chunk with an invalid file");
		return NULL;
	}

	luacc_chunk_t *chunk = (luacc_chunk_t *) malloc(sizeof(luacc_chunk_t));
	chunk->bytes = NULL;
	chunk->src = luac;
	chunk->len = 0;

	char c = 0;
	int i = 0;

	while ((c = fgetc(luac)) != EOF)
	{
		// the length of the header should be 12 bytes in luac5.1
		if (i < sizeof(chunk->header_bytes))
		{
			chunk->header_bytes[i] = c;

			i++;
			continue;
		}

		chunk->bytes = chunk->bytes ? (byte_t *) realloc(chunk->bytes, sizeof(byte_t) * chunk->len + 1) : (byte_t *) malloc(sizeof(byte_t) * chunk->len + 1);
		chunk->bytes[chunk->len++] = c;

		i++;
	}
	
	return chunk;
}

void luacc_free_chunk(luacc_chunk_t *chunk)
{
	free(chunk->bytes);
	free(chunk);
}

luacc_chunk_header_t *luacc_chunk_parse_header(const luacc_chunk_t *chunk)
{
	if (!chunk)
	{
		luacc_log(LUACC_LOG_LEVEL_ERROR, "luacc_chunk_parse_header(): cannot parse the header of a null chunk");
		return NULL;
	}

	luacc_chunk_header_t *header = malloc(sizeof(luacc_chunk_header_t));
	int i = 0;
	
	// signature
	header->signature_byte = chunk->header_bytes[i++];
	header->signature[0] = chunk->header_bytes[i++];
	header->signature[1] = chunk->header_bytes[i++];
	header->signature[2] = chunk->header_bytes[i++];
	header->signature[3] = '\0';
	
	// version data
	header->lua_ver = chunk->header_bytes[i++];
	header->fmt_ver = chunk->header_bytes[i++];
	header->endianness = chunk->header_bytes[i++];

	// sizes
	header->int_size = chunk->header_bytes[i++];
	header->size_t_size = chunk->header_bytes[i++];
	header->instruction_size = chunk->header_bytes[i++];
	header->lua_number_size = chunk->header_bytes[i++];
	header->integral = chunk->header_bytes[i++];

	return header;
}

void luacc_free_chunk_header(luacc_chunk_header_t *header)
{
	free(header);
}
