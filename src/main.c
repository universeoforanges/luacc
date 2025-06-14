#include <luacc/array.h>
#include <luacc/log.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
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
		}
	}

	array_t *input_files = luacc_alloc_array();

	if (!input_files)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "memory allocation failure during input file array allocation");

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], argv[0]) == 0) continue;

		char *fname = argv[i];
		if (!is_file(fname)) continue;

		luacc_array_append(input_files, fname);
	}

	if (input_files->len == 0)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "what input files are there to even compile??");
	
	for (int i = 0; i < input_files->len; i++)
		luacc_log(LUACC_LOG_LEVEL_VERBOSE, (const char *) input_files->data[i]);

	luacc_free_array(input_files);
}
