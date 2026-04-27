/* SPDX-License-Identifier: GPL-3.0
 * Virtual hardware
 *
 * gates.c
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

bit NOT(bit a){
    return !(a);
}

bit NAND(bit a, bit b){
    return NOT(a & b);
}

bit AND(bit a, bit b){
    bit ret = NAND(a,b);
    return NAND(ret, ret);
}

bit OR(bit a, bit b){
    return NAND(NAND(a,a), NAND(b,b));
}

bit NOR(bit a, bit b){
    return NOT(OR(a, b));
}

bit XOR(bit a, bit b){
    bit nand1 = NAND(a, b);
    bit nand2 = NAND(a, nand1);
    bit nand3 = NAND(b, nand1);
    return NAND(nand2, nand3);
}

bit XNOR(bit a, bit b){
    return NOT(XOR(a, b));
}
