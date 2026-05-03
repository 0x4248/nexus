/* SPDX-License-Identifier: GPL-3.0
 * writing_raw
 *
 * write.h
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

int write_data(const uint8_t data[], 
				const char *filename, 
				const int buffer_size);