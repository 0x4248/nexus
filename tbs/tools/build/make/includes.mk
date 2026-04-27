# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# includes.mk
#
# This file is the skeleton of the makefile system, it includes all the
# makefiles and includes the config.
#
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

include tools/build/make/glob.mk
-include .config
include tools/build/make/options.mk
include tools/build/make/kconfig.mk
include tools/build/make/git.mk
include tools/build/make/help.mk
include tools/build/make/test.mk
include tools/build/make/misc.mk
include tools/build/make/scripts.mk