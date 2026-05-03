/* SPDX-License-Identifier: GPL-3.0
 * VisHash - An easy way to visualise hashes
 *
 * main.cpp
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
#include <string>

#include "image.h"
#include "crypto.h"
#include "render.h"

int main(int argc, char *argv[]){

    auto digest = sha256(argv[1]);
    auto matrix = heatmap(digest);
    matrix = blur(matrix);
    draw(matrix, "SHA256");
    print_values(matrix);
    savePPM(matrix, "heatmap.ppm");
}
