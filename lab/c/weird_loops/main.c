/* SPDX-License-Identifier: GPL-3.0
 * weird_loops
 *
 * main.c
 *
 * Looping in a weird way
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
#include <stdlib.h>

int jumps(){
	int i = 0;
loop: 
	if (i < 10) {
		printf("i = %d\n", i);
		i++;
		goto loop;
	}
	return 0;
}

int while_loop() {
	int i = 0;
	while (i < 10) {
		printf("i = %d\n", i);
		i++;
	}
	return 0;
}

int normal_loop() {
	for (int i = 0; i < 10; i++) {
		printf("i = %d\n", i);
	}
	return 0;
}

int do_while_loop() {
	int i = 0;
	do {
		printf("i = %d\n", i);
		i++;
	} while (i < 10);
	return 0;
}

int main() {

	printf("Normal loop\n");
	normal_loop();
	printf("\n");

	printf("While loop\n");
	while_loop();
	printf("\n");

	printf("Do while loop\n");
	do_while_loop();
	printf("\n");

	printf("Jump loop\n");
	jumps();
	printf("\n");
}