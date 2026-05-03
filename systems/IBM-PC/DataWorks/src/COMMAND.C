/* C89 Standard only */

#include <stdio.h>
#include <stdlib.h>

#include "DW.H"

void dw_print_help(void)
{
	printf("DataWorks (simple) commands:\n");
	printf("  DW <file.dw> PRINT\n");
	printf("  DW <file.dw> INSERT <value> $row,col\n");
	printf("  DW <file.dw> DEL_COL [col_index]\n");
	printf("  DW <file.dw> SET_COL_NAME <name> [col_index]\n");
	printf("  DW <file.dw> INTERACT\n");
	printf("Limits: %d rows, %d cols, cell size %d chars\n", DW_MAX_ROWS, DW_MAX_COLS, DW_MAX_CELL - 1);
}

int dw_run_command(const char *path, int argc, char **argv)
{
	DWTable table;
	int row;
	int col;
	int col_index;
	int ok;

	if (argc < 1) {
		return 0;
	}

	dw_init_table(&table);

	if (!dw_load_file(path, &table)) {
		printf("Error: failed to load %s\n", path);
		dw_free_table(&table);
		return 0;
	}
	ok = 1;

	if (dw_stricmp(argv[0], "PRINT") == 0) {
		dw_print_table(&table);
	} else if (dw_stricmp(argv[0], "INSERT") == 0) {
		if (argc < 3) {
			printf("Usage: DW <file.dw> INSERT <value> $row,col\n");
			ok = 0;
			goto cleanup;
		}
		if (!dw_parse_cell_ref(argv[2], &row, &col)) {
			printf("Error: invalid cell ref '%s'. Use $row,col\n", argv[2]);
			ok = 0;
			goto cleanup;
		}
		if (!dw_insert(&table, argv[1], row, col)) {
			printf("Error: table limit reached\n");
			ok = 0;
			goto cleanup;
		}
		if (!dw_save_file(path, &table)) {
			printf("Error: failed to save %s\n", path);
			ok = 0;
			goto cleanup;
		}
	} else if (dw_stricmp(argv[0], "DEL_COL") == 0) {
		col_index = (argc >= 2) ? atoi(argv[1]) : (table.cols - 1);
		if (!dw_del_col(&table, col_index)) {
			printf("Error: invalid column index\n");
			ok = 0;
			goto cleanup;
		}
		if (!dw_save_file(path, &table)) {
			printf("Error: failed to save %s\n", path);
			ok = 0;
			goto cleanup;
		}
	} else if (dw_stricmp(argv[0], "SET_COL_NAME") == 0) {
		if (argc < 2) {
			printf("Usage: DW <file.dw> SET_COL_NAME <name> [col_index]\n");
			ok = 0;
			goto cleanup;
		}
		col_index = (argc >= 3) ? atoi(argv[2]) : (table.cols - 1);
		if (!dw_set_col_name(&table, col_index, argv[1])) {
			printf("Error: invalid column index\n");
			ok = 0;
			goto cleanup;
		}
		if (!dw_save_file(path, &table)) {
			printf("Error: failed to save %s\n", path);
			ok = 0;
			goto cleanup;
		}
	} else {
		printf("Error: unknown command '%s'\n", argv[0]);
		dw_print_help();
		ok = 0;
	}

cleanup:
	dw_free_table(&table);
	return ok;
}
