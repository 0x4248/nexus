/* SPDX-License-Identifier: GPL-3.0
 * VisHash - An easy way to visualise hashes
 *
 * crypto.h
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2025-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include <openssl/sha.h>
#include <vector>
#include <string>

std::vector<uint8_t> sha256(const std::string &input);
