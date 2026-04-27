# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# help.mk
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.


HELPLIST := doc/make/README.txt doc/make/blank.msg doc/make/git.txt

ifeq ($(CONFIG_DOCS_MAN_MODE),y)
	HELP-BEGIN = $(T)$(PANDOC) -s
	HELP-END = -t man | $(GROFF) -man -Tutf8 | less -R
else
	HELP-BEGIN = $(T)cat
	HELP-END = | less
endif

help:
	$(HELP-BEGIN) $(HELPLIST) $(HELP-END)
	$(T)exit 0

help-targets:
	$(T)echo $(TARGETS)

TARGETS += help-targets
