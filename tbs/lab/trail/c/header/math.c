/* SPDX-License-Identifier: GPL-3.0
 * The Language Trail
 *
 * math.c
 *
 * This is going to be quite long so buckle up!
 *
 * https://en.cppreference.com/w/c/header/math.html
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2025-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include <math.h>
#include <stdio.h>
#include <float.h>

// ########## CHECKLIST ##########
// Contents:
// - Constants
// - Functions
//      - Basic
// ##### CONSTANTS #####
// HUGE_VAL         [X]
// INFINITY         [X]
// NAN              [TODO]
// FP_FAST          [TODO]
// FP_ILOGB0        [TODO]
// math_errhandling [TODO]
// ##### FUNCTIONS #####
// ## BASIC ##
// fabs             [TODO]
// fmod             [TODO]
// remainder        [TODO]

/*
 * Lets have a look at the math constants
 *
 * https://en.cppreference.com/w/c/numeric/math/HUGE_VALL.html
 * https://learn.microsoft.com/en-us/cpp/c-runtime-library/huge-val-huge
 */
void test_mathConsts(){
    /* HUGE_VAL - Its a big value, more specifically its the biggest float
     * value there is. Some people use it as an error e.g -HUGE_VAL.
     */
    printf("%f\n", HUGE_VAL); /* This just prints inf */
    printf("%Lf\n", HUGE_VALL); /* HUGE_VAL but double; so even huger value */

    /* Very similar to HUGE_VAL but can be used with isinf() */
    printf("%f\n", INFINITY);

    /* NAN - Not a number, but is a float; can be used with nan() and isnan() */
    printf("%f\n", NAN);
}

int main(){
    test_mathConsts();
    return 0;
}
