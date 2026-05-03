/* SPDX-License-Identifier: GPL-3.0
 * tiny
 *
 * math.c
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

int add(int a, int b){
	return a + b;
}

int power(int a){
	return a * a;
}

int main(){

	/* Add two numbers, get the power of one and then print the results 
	 * to the console.
	 */
	int a, b;
	scanf("%d", &a);
	scanf("%d", &b);

	printf("The sum of %d and %d is %d\n", a, b, add(a, b));
	printf("%d squared is %d\n", a, power(a));
	return 0;
}