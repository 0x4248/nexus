/* SPDX-License-Identifier: GPL-3.0
 * VisHash - An easy way to visualise hashes
 *
 * image.cpp
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

std::vector<std::vector<int>> heatmap(const std::vector<uint8_t> &digest) {
    std::vector<std::vector<int>> grid(H, std::vector<int>(W, 0));

    uint8_t buf[SHA256_DIGEST_LENGTH];
    memcpy(buf, digest.data(), digest.size());

    int samples = H * W * 3;
    for (int i = 0; i < samples; i++) {
        uint8_t tmp[SHA256_DIGEST_LENGTH];
        SHA256(buf, SHA256_DIGEST_LENGTH, tmp);
        memcpy(buf, tmp, SHA256_DIGEST_LENGTH);


        int y = buf[0] % H;
        int x = buf[1] % W;

        grid[y][x]++;
    }

    return grid;
}

std::vector<std::vector<int>> blur(const std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> out = grid;

    static int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

    int H = grid.size();
    int W = grid[0].size();

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {

            int sum = 0, weight = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int ny = y + dy;
                    int nx = x + dx;
                    if (ny >= 0 && ny < H && nx >= 0 && nx < W) {
                        int w = kernel[dy+1][dx+1];
                        sum += grid[ny][nx] * w;
                        weight += w;
                    }
                }
            }

            out[y][x] = sum / weight;
        }
    }

    return out;
}

int apply_contrast(int v, int maxv, double gamma = 1.8) {
    if (maxv == 0) return 0;
    double norm = (double)v / maxv;
    norm = pow(norm, gamma);
    return (int)(norm * maxv);
}

char level_to_char(int level, int maxLevel)
{
    if (maxLevel == 0) return ' ';

    int adjusted = apply_contrast(level, maxLevel, 1.8);

    int idx = (adjusted * (RAMP.size() - 1)) / maxLevel;
    return RAMP[idx];
}
