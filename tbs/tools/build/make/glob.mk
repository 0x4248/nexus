# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# glob.mk
# Global variables
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

# Trace mode
T := @
LOG := :
# All targets for phony and help
TARGETS =

SHELL ?= /bin/bash
PYTHON ?= python3
PIP ?= pip3
PANDOC ?= pandoc
GROFF ?= groff
