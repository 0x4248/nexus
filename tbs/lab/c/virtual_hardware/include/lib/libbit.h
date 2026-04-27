/* SPDX-License-Identifier: GPL-3.0
 * Virtual hardware
 *
 * libbit.h
 * 
 * COPYRIGHT NOTICE
 * Copyright (C) 2024-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 * 
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include<stdbool.h>

/**
 * Ok so I know many people say this is not one bit its really 8 bits in 
 * memory and I KNOW OK! This isn't designed to be efficient at all TwT, im sorry
 * C god's, please spare me...
 * 
 * You cant really do "one" bit in C unless im dumb (most likely) and not
 * implementing it here. Anyways, this **one** line is used in 99% of the code
 * in this project, crazy!
 */
typedef bool bit;