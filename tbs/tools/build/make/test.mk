# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# test.mk
# Base test file just to test if things are working ok e.g precheck system.
# This can also be used as a template.
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

test:
	$(T)$(LOG) -e "TEST\tYUP WORKS"
	$(T)echo "Yeah works"

TARGETS += test
