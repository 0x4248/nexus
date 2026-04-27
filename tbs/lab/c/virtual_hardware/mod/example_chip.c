/* SPDX-License-Identifier: GPL-3.0
 * Virtual hardware
 *
 * halfadder.c
 * 
 * COPYRIGHT NOTICE
 * Copyright (C) 2024-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 * 
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include "lib/libbit.h"
#include "mod/gates.h"
#include <stdio.h>

typedef struct {
    bit sum;
    bit carry;
} HalfAdder;

HalfAdder halfAdder(bit a, bit b){
    HalfAdder ha;
    ha.sum = XOR(a, b);
    ha.carry = AND(a, b);
    return ha;
}