/* SPDX-License-Identifier: GPL-3.0
 * XORenc
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

char *encode(char *str, int *key)
{
	/**
	 * Loop over each item in input string (str) and XOR it with the key using
	 * the modulo operator to get the correct index of the key.
	 *
	 * If we hit \0 we are at the end of the string and we can stop.
	 */
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = str[i] ^ key[i % 4];
	}
	return str;
}

int main()
{
	char str[100];
	int key[100];

	/* input */
	printf("Enter a string: ");	
	scanf("%s", str);
	printf("Enter a key: ");
	scanf("%d", key);

	/* Encrypt */
	encode(str, key);
	printf("%s\n", str);

	/* Decrypt */
	encode(str, key);
	printf("%s\n", str);
	return 0;
}