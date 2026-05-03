/* SPDX-License-Identifier: GPL-3.0
 * VisHash - An easy way to visualise hashes
 *
 * render.cpp
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
#include <iostream>
#include <fstream>
#include "image.h"

/**
 * Draw to TTY
 */
void draw(const std::vector<std::vector<int>> &grid, const std::string &label) {

    int maxv = 0;
    for (auto &row : grid)
        for (int v : row)
            maxv = std::max(maxv, v);


    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            std::cout << level_to_char(grid[y][x], maxv);
        }
        std::cout << "\n";
    }

}

/**
 * Basically prints out the values for debugging
 */
void print_values(const std::vector<std::vector<int>>& grid) {
    int H = grid.size();
    int W = grid[0].size();
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            std::cout << grid[y][x];
        }
        std::cout << "\n";
    }
}

void savePPM(const std::vector<std::vector<int>>& grid, const std::string& filename)
{
    int H = grid.size();
    int W = grid[0].size();

    int maxv = 0;
    for (auto& row : grid)
        for (int v : row)
            maxv = std::max(maxv, v);

    std::ofstream out(filename);
    out << "P3\n" << W << " " << H << "\n255\n";

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            float t = (float)grid[y][x] / (float)maxv;

            int v = (int)(t * 255); /* Can be changed for intensity */

            out << v << " " << v << " " << v << "  ";
        }
        out << "\n";
    }
}
