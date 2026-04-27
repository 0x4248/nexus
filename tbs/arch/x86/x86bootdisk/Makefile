# SPDX-License-Identifier: GPL-3.0
# x86bootdisk
#
# Makefile
#
# COPYRIGHT NOTICE
# Copyright (C) 2024-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

S=
NASM=nasm
BUILD=build
BOOTDISK = src/boot.asm
BIN = boot.bin

all: init compile

init:
	$(S)mkdir -p $(BUILD)

compile:
	$(S)$(NASM) -f bin $(BOOTDISK) -o $(BUILD)/$(BIN)

clean:
	$(S)rm -rf $(BUILD)
	$(S)rm -rf $(BUILD)/$(BIN)

test: all
	$(S)qemu-system-i386 -fda $(BUILD)/$(BIN)

.PHONY: all clean
