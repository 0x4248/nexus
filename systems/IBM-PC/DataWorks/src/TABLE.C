/* C89 Standard only */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DW.H"

static int dw_alloc_cell(char **out_cell)
{
	*out_cell = (char *)malloc((size_t)DW_MAX_CELL);
	if (*out_cell == NULL) {
		return 0;
	}
	(*out_cell)[0] = '\0';
	return 1;
}

static void dw_fill_header_names(DWTable *table, int start_col)
{
	int c;
	char temp[32];

	for (c = start_col; c < table->cols; ++c) {
		if (table->cells[0][c] != NULL && table->cells[0][c][0] == '\0') {
			sprintf(temp, "COL_%d", c);
			dw_copy_cell(table->cells[0][c], temp);
		}
	}
}

static int dw_ensure_size(DWTable *table, int rows, int cols)
{
	int r;
	int c;
	int old_rows;
	char ***new_rows;
	char **new_row;

	if (rows > DW_MAX_ROWS || cols > DW_MAX_COLS || rows < 1 || cols < 1) {
		return 0;
	}

	old_rows = table->rows;

	if (rows > table->rows) {
		new_rows = (char ***)realloc(table->cells, (size_t)rows * sizeof(char **));
		if (new_rows == NULL) {
			return 0;
		}
		table->cells = new_rows;
		for (r = table->rows; r < rows; ++r) {
			table->cells[r] = NULL;
		}
		table->rows = rows;
	}

	if (table->cols > 0 && table->rows > old_rows) {
		for (r = old_rows; r < table->rows; ++r) {
			table->cells[r] = (char **)malloc((size_t)table->cols * sizeof(char *));
			if (table->cells[r] == NULL) {
				return 0;
			}
			for (c = 0; c < table->cols; ++c) {
				table->cells[r][c] = NULL;
				if (!dw_alloc_cell(&table->cells[r][c])) {
					return 0;
				}
			}
		}
	}

	if (cols > table->cols) {
		for (r = 0; r < table->rows; ++r) {
			new_row = (char **)realloc(table->cells[r], (size_t)cols * sizeof(char *));
			if (new_row == NULL) {
				return 0;
			}
			table->cells[r] = new_row;
			for (c = table->cols; c < cols; ++c) {
				table->cells[r][c] = NULL;
				if (!dw_alloc_cell(&table->cells[r][c])) {
					return 0;
				}
			}
		}
		table->cols = cols;
		dw_fill_header_names(table, 0);
	}

	return 1;
}

void dw_init_table(DWTable *table)
{
	table->rows = 0;
	table->cols = 0;
	table->cells = NULL;

	if (!dw_ensure_size(table, 1, 1)) {
		return;
	}

	dw_copy_cell(table->cells[0][0], "COL_0");
}

void dw_free_table(DWTable *table)
{
	int r;
	int c;

	if (table->cells != NULL) {
		for (r = 0; r < table->rows; ++r) {
			if (table->cells[r] != NULL) {
				for (c = 0; c < table->cols; ++c) {
					free(table->cells[r][c]);
				}
				free(table->cells[r]);
			}
		}
		free(table->cells);
	}

	table->rows = 0;
	table->cols = 0;
	table->cells = NULL;
}

int dw_load_file(const char *path, DWTable *table)
{
	FILE *fp;
	char line[DW_MAX_LINE];
	int row;
	int col;
	int pos;
	int i;
	char delim;
	char cell[DW_MAX_CELL];

	dw_init_table(table);
	fp = fopen(path, "rb");
	if (fp == NULL) {
		return 1;
	}

	row = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		dw_trim(line);
		delim = (strchr(line, DW_SEP) != NULL) ? (char)DW_SEP : ',';

		col = 0;
		pos = 0;
		cell[0] = '\0';

		for (i = 0;; ++i) {
			if (line[i] == delim || line[i] == '\0') {
				cell[pos] = '\0';
				dw_trim(cell);

				if (!dw_ensure_size(table, row + 1, col + 1)) {
					fclose(fp);
					dw_free_table(table);
					return 0;
				}
				dw_copy_cell(table->cells[row][col], cell);

				++col;
				pos = 0;
				cell[0] = '\0';

				if (line[i] == '\0' || col >= DW_MAX_COLS) {
					break;
				}
			} else if (pos < DW_MAX_CELL - 1) {
				cell[pos++] = line[i];
			}
		}

		++row;
		if (row >= DW_MAX_ROWS) {
			break;
		}
	}

	if (row > 0) {
		table->rows = row;
	}
	dw_fill_header_names(table, 0);
	fclose(fp);
	return 1;
}

int dw_save_file(const char *path, const DWTable *table)
{
	FILE *fp;
	int r;
	int c;

	fp = fopen(path, "wb");
	if (fp == NULL) {
		return 0;
	}

	for (r = 0; r < table->rows; ++r) {
		for (c = 0; c < table->cols; ++c) {
			if (table->cells[r][c] != NULL) {
				fputs(table->cells[r][c], fp);
			}
			if (c < table->cols - 1) {
				fputc(DW_SEP, fp);
			}
		}
		fputc('\n', fp);
	}

	fclose(fp);
	return 1;
}

void dw_print_table(const DWTable *table)
{
	int r;
	int c;
	int i;
	int len;
	int pad;
	int widths[DW_MAX_COLS];

	for (c = 0; c < table->cols; ++c) {
		widths[c] = 0;
	}

	for (r = 0; r < table->rows; ++r) {
		for (c = 0; c < table->cols; ++c) {
			len = 0;
			if (table->cells[r][c] != NULL) {
				len = (int)strlen(table->cells[r][c]);
			}
			if (len > widths[c]) {
				widths[c] = len;
			}
		}
	}

	for (r = 0; r < table->rows; ++r) {
		for (c = 0; c < table->cols; ++c) {
			if (table->cells[r][c] == NULL || table->cells[r][c][0] == '\0') {
				printf("~");
			} else {
				printf("%s", table->cells[r][c]);
			}
			if (c < table->cols - 1) {
				len = 0;
				if (table->cells[r][c] != NULL) {
					len = (int)strlen(table->cells[r][c]);
				}
				if (len == 0) {
					len = 1;
				}
				pad = (widths[c] - len) + 4;
				for (i = 0; i < pad; ++i) {
					putchar(' ');
				}
			}
		}
		printf("\n");
	}
}

int dw_insert(DWTable *table, const char *value, int data_row, int col)
{
	int real_row;

	real_row = data_row + 1;
	if (!dw_ensure_size(table, real_row + 1, col + 1)) {
		return 0;
	}
	dw_copy_cell(table->cells[real_row][col], value);
	return 1;
}

int dw_del_col(DWTable *table, int col_index)
{
	int r;
	int c;

	if (col_index < 0 || col_index >= table->cols || table->cols <= 0) {
		return 0;
	}

	for (r = 0; r < table->rows; ++r) {
		for (c = col_index; c < table->cols - 1; ++c) {
			dw_copy_cell(table->cells[r][c], table->cells[r][c + 1]);
		}
		if (table->cells[r][table->cols - 1] != NULL) {
			table->cells[r][table->cols - 1][0] = '\0';
		}
	}
	table->cols -= 1;
	return 1;
}

int dw_set_col_name(DWTable *table, int col_index, const char *name)
{
	if (col_index < 0 || col_index >= table->cols) {
		return 0;
	}
	dw_copy_cell(table->cells[0][col_index], name);
	return 1;
}
