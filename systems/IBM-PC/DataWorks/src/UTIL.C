/* C89 Standard only */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "DW.H"

void dw_trim(char *s)
{
	int len;
	int i;
	int start;

	len = (int)strlen(s);
	while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || isspace((unsigned char)s[len - 1]))) {
		s[len - 1] = '\0';
		--len;
	}

	start = 0;
	while (s[start] != '\0' && isspace((unsigned char)s[start])) {
		++start;
	}

	if (start > 0) {
		i = 0;
		while (s[start + i] != '\0') {
			s[i] = s[start + i];
			++i;
		}
		s[i] = '\0';
	}
}

int dw_stricmp(const char *a, const char *b)
{
	unsigned char ca;
	unsigned char cb;

	while (*a != '\0' && *b != '\0') {
		ca = (unsigned char)tolower((unsigned char)*a);
		cb = (unsigned char)tolower((unsigned char)*b);
		if (ca != cb) {
			return (int)ca - (int)cb;
		}
		++a;
		++b;
	}
	return (int)(unsigned char)*a - (int)(unsigned char)*b;
}

void dw_copy_cell(char *dst, const char *src)
{
	if (src == NULL) {
		dst[0] = '\0';
		return;
	}
	strncpy(dst, src, DW_MAX_CELL - 1);
	dst[DW_MAX_CELL - 1] = '\0';
}

int dw_parse_cell_ref(const char *text, int *row, int *col)
{
	char *endptr;
	long r;
	long c;

	if (text == NULL || text[0] != '$') {
		return 0;
	}

	r = strtol(text + 1, &endptr, 10);
	if (*endptr != ',' || r < 0) {
		return 0;
	}
	c = strtol(endptr + 1, &endptr, 10);
	if (*endptr != '\0' || c < 0) {
		return 0;
	}

	*row = (int)r;
	*col = (int)c;
	return 1;
}
