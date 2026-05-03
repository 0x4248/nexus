/* SPDX-License-Identifier: GPL-3.0
 * writing_raw
 *
 * basic.c
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
#include <stdint.h>

#include "write.h"

int main() {
	int buffer_size = 2;
	const uint8_t data[] = {
	    0xFF, 0xAA
	};
	
	write_data(data, "data.bin", buffer_size);
	return 0;
}