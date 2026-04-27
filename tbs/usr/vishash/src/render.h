 /* SPDX-License-Identifier: GPL-3.0
  * VisHash - An easy way to visualise hashes
  *
  * render.h
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
#include <iostream>

void draw(const std::vector<std::vector<int>> &grid, const std::string &label);
void print_values(const std::vector<std::vector<int>>& grid);
void savePPM(const std::vector<std::vector<int>>& grid, const std::string& filename);
