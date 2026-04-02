/* C89 Standard only */

#include <stdio.h>
#include <string.h>

#include "DW.H"

static int dw_interact(const char *path)
{
	char line[DW_MAX_LINE];
	char *tokens[8];
	char *p;
	int count;

	printf("DataWorks interactive mode. Type HELP or EXIT.\n");
	for (;;) {
		printf("%s> ", path);
		if (fgets(line, sizeof(line), stdin) == NULL) {
			return 1;
		}
		dw_trim(line);
		if (line[0] == '\0') {
			continue;
		}

		count = 0;
		p = strtok(line, " \t");
		while (p != NULL && count < 8) {
			tokens[count++] = p;
			p = strtok(NULL, " \t");
		}
		if (count == 0) {
			continue;
		}

		if (dw_stricmp(tokens[0], "EXIT") == 0 || dw_stricmp(tokens[0], "QUIT") == 0) {
			return 1;
		}
		if (dw_stricmp(tokens[0], "HELP") == 0) {
			dw_print_help();
			continue;
		}

		if (!dw_run_command(path, count, tokens)) {
			printf("Command failed.\n");
		}
	}
}

int dw_cli_main(int argc, char **argv)
{
	if (argc < 3) {
		dw_print_help();
		return 1;
	}

	if (dw_stricmp(argv[2], "INTERACT") == 0) {
		return dw_interact(argv[1]) ? 0 : 1;
	}

	return dw_run_command(argv[1], argc - 2, argv + 2) ? 0 : 1;
}
