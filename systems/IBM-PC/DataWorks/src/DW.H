#ifndef DW_H
#define DW_H

#define DW_SEP 31
#define DW_MAX_LINE 4096
#define DW_MAX_ROWS 256
#define DW_MAX_COLS 64
#define DW_MAX_CELL 128

typedef struct {
	int rows;
	int cols;
	char ***cells;
} DWTable;

void dw_trim(char *s);
int dw_stricmp(const char *a, const char *b);
void dw_copy_cell(char *dst, const char *src);
int dw_parse_cell_ref(const char *text, int *row, int *col);

void dw_init_table(DWTable *table);
void dw_free_table(DWTable *table);
int dw_load_file(const char *path, DWTable *table);
int dw_save_file(const char *path, const DWTable *table);
void dw_print_table(const DWTable *table);
int dw_insert(DWTable *table, const char *value, int data_row, int col);
int dw_del_col(DWTable *table, int col_index);
int dw_set_col_name(DWTable *table, int col_index, const char *name);

void dw_print_help(void);
int dw_run_command(const char *path, int argc, char **argv);

int dw_cli_main(int argc, char **argv);

#endif

