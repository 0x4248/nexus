# SPDX-License-Identifier: GPL-3.0
# tiny
#
# build.sh
#
# COPYRIGHT NOTICE
# Copyright (C) 2024-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
# 
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation


if [ "$1" == "clean" ]; then
	rm -rf build
	exit 0
fi

mkdir -p build

echo "[ BUILD.SH ] Building tiny"
gcc tiny.c -o build/tiny
gcc -S tiny.c -o build/tiny.s

echo "[ BUILD.SH ] Building hello"
gcc hello.c -o build/hello
gcc -S hello.c -o build/hello.s

echo "[ BUILD.SH ] Building math"
gcc math.c -o build/math
gcc -S math.c -o build/math.s