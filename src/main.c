#include "luacc/luac_func_chunk.h"
#include <luacc/luac_chunk.h>
#include <luacc/array.h>
#include <luacc/log.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

void print_usage(void)
{
	printf("usage: luacc [options] input file(s)...\n");
	printf("command line options:\n");
	printf("\t-o <output>        outputs the compiled assembly to <output>. (default is out.asm)\n");
	printf("\t-a <nasm|masm>     sets the assembly syntax to the provided assembler. (default is nasm)\n");
	printf("\t-V                 makes the program provide more verbose output. great for debugging ig\n");
	printf("\t-v                 prints the current version of luacc that you're using.\n");
	printf("\t-h                 prints this help message\n");
	printf("\nluacc is currently in early development, and your contribution helps. you can contribute at: https://github.com/universeoforanges/luacc\n");
}

bool_t is_file(const char *fname)
{
	struct stat s = (struct stat) { 0 };
	stat(fname, &s);
	
	return S_ISREG(s.st_mode);
}

int main(int argc, char **argv)
{
	int opt = 0;

	char *output = "out.asm";
	char *asm_syntax = "nasm";

	while ((opt = getopt(argc, argv, "vho:a:V")) != -1)
	{
		switch (opt)
		{
		case 'v':
			printf("%s\n", LUACC_VERSION);
			return 0;
		case 'h':
			print_usage();
			return 0;
		case 'o':
			output = optarg;
			break;
		case 'a':
			asm_syntax = optarg;
			break;
		case 'V':
			luacc_enable_verbose_logging(LUACC_TRUE);
			break;
		default:
			char buf[32];
			snprintf(buf, sizeof(buf), "invalid command line option `%c`", (char) opt);

			luacc_log(LUACC_LOG_LEVEL_WARNING, buf);
		}
	}

	array_t *input_files = luacc_alloc_array();

	if (!input_files)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "memory allocation failure during input file array allocation");
	
	// file reading

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], argv[0]) == 0 || !is_file(argv[i])) continue;
		
		FILE *input_file = fopen(argv[i], "r");

		if (!input_file)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "could not open input file %s", argv[i]);

			luacc_log(LUACC_LOG_LEVEL_ERROR, buf);

			continue;
		}

		luacc_array_append(input_files, input_file);
	}

	if (input_files->len == 0)
	{
		luacc_free_array(input_files);
		luacc_log(LUACC_LOG_LEVEL_FATAL, "what input files are there to even compile??");
	}
	
	array_t *chunks = luacc_alloc_array();
	array_t *function_chunks = luacc_alloc_array();
	
	for (int i = 0; i < input_files->len; i++)
		luacc_array_append(chunks, luacc_read_chunk((FILE *) input_files->data[i]));

	// parsing

	for (int i = 0; i < chunks->len; i++)
	{
		luacc_chunk_t *chunk = (luacc_chunk_t *) chunks->data[i];
		luacc_chunk_header_t *header = luacc_chunk_parse_header(chunk);

		if (strcmp((char *) header->signature, "Lua") != 0 || header->lua_ver != 0x51)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "unsupported luac version: this version of luacc supports Lua 51 only, but got %s %x", (char *) header->signature, header->lua_ver);

			luacc_log(LUACC_LOG_LEVEL_WARNING, buf);
		}

		luacc_free_chunk_header(header);
		
		size_t index = 0;
		luacc_function_chunk_t *func_chunk = luacc_parse_function_chunk(chunk, &index);

		luacc_array_append(function_chunks, func_chunk);
	}
	
	// cleanup

	for (int i = 0; i < input_files->len; i++)
		fclose(input_files->data[i]);

	luacc_free_array(input_files);

	for (int i = 0; i < chunks->len; i++)
		luacc_free_chunk((luacc_chunk_t *) chunks->data[i]);
	
	luacc_free_array(chunks);

	for (int i = 0; i < function_chunks->len; i++)
		luacc_free_function_chunk((luacc_function_chunk_t *) function_chunks->data[i]);
	
	luacc_free_array(function_chunks);
}
