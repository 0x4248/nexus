/* SPDX-License-Identifier: GPL-3.0
 * factorial
 *
 * main.c
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2024-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
 */

#include <stdio.h>

__uint128_t factorial(int n)
{
	if (n == 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else
	{
		return n * factorial(n - 1);
	}
}

void print_uint128_decimal(__uint128_t value)
{
	if (value == 0)
	{
		printf("0");
		return;
	}

	char buffer[40];
	int pos = sizeof(buffer) - 1;

	buffer[pos] = '\0';
	while (value > 0)
	{
		buffer[--pos] = '0' + (value % 10);
		value /= 10;
	}
	printf("%s", &buffer[pos]);
}

int main()
{
	printf("128-bit factorial\n");
	printf("Copyright (C) 2025-2026 0x4248\n");
	printf("GNU General Public License v3.0\n");

	__uint128_t result;
	for (int i = 0; i < 130; i++)
	{
		result = factorial(i);
		printf("%d = ", i);
		print_uint128_decimal(result);
		printf("\n");
	}
	return 0;
}