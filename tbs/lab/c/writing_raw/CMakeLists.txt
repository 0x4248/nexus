# SPDX-License-Identifier: GPL-3.0
# writing_raw
#
# CMakeLists.txt
#
# COPYRIGHT NOTICE
# Copyright (C) 2024-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.
cmake_minimum_required(VERSION 3.5)

project(writing_raw)

include_directories(include)
add_executable(basic basic.c write.c)
add_executable(hello hello.c write.c)