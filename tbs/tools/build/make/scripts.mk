# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# scripts.mk
# This points to all the scripts
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.



### CLANG ###
clangd-conf:
	$(T)$(PYTHON) tools/build/clConfGen/generatecland.py

### WWW ###

## Atlas ##
build-atlas:
	$(T)$(PYTHON) tools/www/buildpages.py

clean-scapexplorer:
	$(T)rm -rf pages/

### MACOS ###
macos-imhex:
	$(T)read -p "Enter path: " P; \
	$(T)/Applications/ImHex.app/Contents/MacOS/imhex -o "$$P"

TARGETS += clangd-conf build-scapexplorer clean-scapexplorer macos-imhex
