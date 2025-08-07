#include <luacc/luac_func_chunk.h>
#include <luacc/luac_chunk.h>
#include <luacc/codegen.h>
#include <luacc/array.h>
#include <luacc/log.h>

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>

void print_usage(void)
{
	printf("usage: luacc [options] input file(s)...\n");
	printf("command line options:\n");
	printf("\t-a | --asm <nasm|masm>    sets the assembly syntax to the provided assembler. (default is nasm)\n");
	printf("\t-V | --verbose            makes the program provide more verbose output. great for debugging ig\n");
	printf("\t-v | --version            prints the current version of luacc that you're using.\n");
	printf("\t-h | --help               prints this help message\n");
	printf("\nluacc is currently in early development, and your contribution helps. you can contribute at: https://github.com/universeoforanges/luacc\n");
}

int main(int argc, char **argv)
{
	char *asm_syntax = "nasm";

	static struct option options[] = {
		{ "asm", required_argument, NULL, 'a' },
		{ "verbose", no_argument, NULL, 'V' },
		{ "version", no_argument, NULL, 'v' },
		{ "help", no_argument, NULL, 'h' },
		{ 0, 0, 0, 0 }
	};
	
	int opt_idx = 0;
	int opt = 0;

	while ((opt = getopt_long(argc, argv, "a:Vvh", options, &opt_idx)) != -1)
	{
		switch (opt)
		{
		case 'a':
			asm_syntax = optarg;
			break;
		case 'V':
			luacc_enable_verbose_logging(LUACC_TRUE);
			break;
		case 'v':
			printf("%s\n", LUACC_VERSION);
			return 0;
		case 'h':
			print_usage();
			return 0;
		case '?':
			char buf[256];
			snprintf(buf, sizeof(buf), "invalid command line option `%c`", (char) optopt);

			luacc_log(LUACC_LOG_LEVEL_WARNING, buf);
			break;
		default:
			break;
		}
	}

	array_t *input_files = luacc_alloc_array();

	if (!input_files)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "memory allocation failure during input file array allocation");
	
	// file reading
	luacc_read_input_files(input_files, argc, argv);
	
	if (input_files->len == 0)
	{
		luacc_free_array(input_files);
		luacc_log(LUACC_LOG_LEVEL_FATAL, "what input files are there to even compile??");
	}
	
	array_t *chunks = luacc_alloc_array();
	array_t *function_chunks = luacc_alloc_array();
	
	for (int i = 0; i < input_files->len; i++)
	{
		luacc_chunk_t *chunk = luacc_read_chunk((FILE *) input_files->data[i]);
		if (!chunk)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "could not read the chunk at index %d", i);

			luacc_log(LUACC_LOG_LEVEL_ERROR, buf);
			continue;
		}

		luacc_array_append(chunks, chunk);
	}

	// parsing
	luacc_parse_chunks(chunks, function_chunks);

	// finally, the code generation part
	for (size_t i = 0; i < function_chunks->len; i++)
	{
		const luacc_function_chunk_t *func = function_chunks->data[i];
		char fname_buf[256];

		snprintf(fname_buf, sizeof(fname_buf), "out%d.asm", (int) i);
		luacc_func_chunk_generate(func, fname_buf);
	}

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
