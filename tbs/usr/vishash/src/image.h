/* SPDX-License-Identifier: GPL-3.0
 * VisHash - An easy way to visualise hashes
 *
 * image.h
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2025-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include <vector>
#include <openssl/sha.h>
#include <cstring>
#include <cmath>

#include "const.h"

std::vector<std::vector<int>> heatmap(const std::vector<uint8_t> &digest);
std::vector<std::vector<int>> blur(const std::vector<std::vector<int>>& grid);
int apply_contrast(int v, int maxv, double gamma = 1.8);
char level_to_char(int level, int maxLevel);
