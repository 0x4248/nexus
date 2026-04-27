/* SPDX-License-Identifier: GPL-3.0
 * The Language Trail
 *
 * ctype.c
 *
 * Exploration of the <ctype> standard header. This header is pretty self
 * explanatory, but useful.
 *
 * https://en.cppreference.com/w/c/header/ctype.html
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2025-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include <ctype.h>
#include <stdio.h>

// ########## CHECKLIST ##########
// isalnum  [TODO]
// isalpha  [X]
// islower  [X]
// isupper  [TODO]
// isdigit  [TODO]
// isxdigit [TODO]
// iscntrl  [TODO]
// isgraph  [TODO]
// isspace  [TODO]
// isblank  [TODO]
// isprint  [TODO]
// ispunct  [TODO]
// tolower  [TODO]
// toupper  [TODO]

/** isLower(chr)
 * checks if a char is "abcdefghijklmnopqrstuvwxyz"
 *
 * Return: 0 on lowercase, non zero on uppercase (int: 512)
 */
void test_isLower(){
    char str[12] = "aBcDEfg123!#";
   	for (int i = 0; i < sizeof(str); i++) {
        if(islower(str[i])){
            printf("%c\tis lowercase\n",str[i]);
        } else {
            /* This is not always true, see in the testing you can see the
             * numbers printed with the uppercase, but yet again its only
             * checking for "abcdefghijklmnopqrstuvwxyz"
             */
            printf("%c\tis UPPERCASE\n",str[i]);
        }

	}
}

/** isalnum(chr)
 * checks if a char is a number or a letter in latin.
 *
 * Return: 0 on alphanumeric, non zero on non alphanumeric
 */
void test_isalnum(){
    char str[12] = "aBcDEfg123!#";
   	for (int i = 0; i < sizeof(str); i++) {
        if(isalnum(str[i])){
            printf("%c\tTRUE\n",str[i]);
        } else {
            printf("%c\tFALSE\n",str[i]);
        }

	}
}



int main(void){
    printf("---[isLower]---\n");
    test_isLower();
    printf("---[isalnum]---\n");
    test_isalnum();
    return 0;
}
