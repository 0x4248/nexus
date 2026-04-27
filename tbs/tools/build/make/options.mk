# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# options.mk
# Kconfig utilities, targets should not be put here
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

ifeq ($(CONFIG_MAKEFILE_TRACE),y)
T :=
endif

ifeq ($(CONFIG_MAKEFILE_LOGS),y)
LOG := echo
endif
