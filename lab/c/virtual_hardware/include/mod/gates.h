/* SPDX-License-Identifier: GPL-3.0
 * Virtual hardware
 *
 * gates.h
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

bit NOT(bit a);
bit NAND(bit a, bit b);
bit AND(bit a, bit b);
bit OR(bit a, bit b);
bit NOR(bit a, bit b);
bit XOR(bit a, bit b);
bit XNOR(bit a, bit b);