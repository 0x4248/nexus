# SPDX-License-Identifier: GPL-3.0
# tiny
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
cmake_minimum_required(VERSION 3.0)

project(tiny)

add_custom_target(build
	COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build.sh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Running build.sh"
	VERBATIM
)
