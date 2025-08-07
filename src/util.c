#include <luacc/luac_func_chunk.h>
#include <luacc/luac_chunk.h>
#include <luacc/util.h>
#include <luacc/log.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool_t _is_file(const char *fname)
{
	struct stat s = (struct stat) { 0 };
	stat(fname, &s);
	
	return S_ISREG(s.st_mode);
}

int luacc_get_bits(const int n, const int idx, const int size)
{
	const int SHIFTED = n >> idx;
	const int MASK = (1 << size) - 1;

	return SHIFTED & MASK;
}

void luacc_read_input_files(array_t *output_array, int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], argv[0]) == 0 || !_is_file(argv[i])) continue;
		
		FILE *input_file = fopen(argv[i], "r");

		if (!input_file)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "could not open input file %s", argv[i]);

			luacc_log(LUACC_LOG_LEVEL_ERROR, buf);
			continue;
		}

		luacc_array_append(output_array, input_file);
	}
}

void luacc_parse_chunks(const array_t *chunks, array_t *func_chunks)
{
	for (int i = 0; i < chunks->len; i++)
	{
		luacc_chunk_t *chunk = (luacc_chunk_t *) chunks->data[i];
		luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);

		if (header->signature_byte != 0x1b)
		{
			luacc_log(LUACC_LOG_LEVEL_ERROR, "in file header: invalid signature byte");
			continue;
		}

		if (strcmp((char *) header->signature, "Lua") != 0 || header->lua_ver != 0x51)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "unsupported luac version: this version of luacc supports Lua 51 only, but got %s %x", (char *) header->signature, header->lua_ver);

			luacc_log(LUACC_LOG_LEVEL_ERROR, buf);
			continue;
		}

		luacc_free_chunk_header(header);
		
		size_t index = 0;
		luacc_function_chunk_t *func_chunk = luacc_parse_function_chunk(chunk, &index);

		luacc_array_append(func_chunks, func_chunk);
	}
}

char *luacc_get_fname_from_handle(FILE *handle)
{
	int fd = fileno(handle);
	if (fd == -1) return NULL;

	char fd_path[256];
	snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);

	char *fname = (char *) malloc(256);
	if (!fname) return NULL;

	ssize_t n = readlink(fd_path, fname, 255);
	if (n == -1)
	{
		free(fname);
		return NULL;
	}

	fname[n] = '\0';

	return fname;
}
