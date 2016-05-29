#include <stdio.h>

#include "bf.h"
#include "debug.h"
#include "file_reader.h"

void print_usage(void)
{
	debug("Brainfuck Executer.\n"
		"Usage: bf <FILE_PATH>\n"
		"		FILE_PATH: Path to a brainfuck code file\n");
}

int main(int argc, char *argv[])
{
	char *prog;
	size_t prog_size;
	struct bf_context ctx;

	if (argc != 2) {
		print_usage();
		return -1;
	}

	debug("Program file: %s\n", argv[1]);
	prog_size = read_file(argv[1], &prog);
	debug("Read program of size %zu\n", prog_size);
	bf_init(&ctx, prog, prog_size);
	return bf_run(&ctx);
}
