# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# Makefile
# This file seems pretty empty, well thats because it is by design. Check the
# includes in tools/build/make/includes.mk
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.



all: precheck help

NOCONFIG_TARGETS := menuconfig defconfig

precheck:
	@{ \
		if [ "$$(uname -s)" = "Darwin" ] && \
		   [ "$(CONFIG_MAKE_IGNORE_WARNING_DARWIN)" != "y" ]; then \
			echo "WARNING: You are using Darwin (MacOS) which is not usually supported by Nexus"; \
			echo "Set CONFIG_MAKE_IGNORE_WARNING_DARWIN=y to suppress this warning"; \
		fi; \
	} || true

	$(T)$(LOG) -e "PRECHECK\tGLOBPATH"
	$(T)pwd > .nexopath
	$(T)$(LOG) -e "PRECHECK\tCONFIG"
	@if [ -z "$(filter $(MAKECMDGOALS),$(NOCONFIG_TARGETS))" ]; then \
		if [ ! -f .config ]; then \
			cat doc/make/errors/confmissing.msg | less; \
			exit 1; \
		fi; \
		if which pandoc 2>/dev/null >/dev/null; then \
			echo "PASS" > /dev/null; \
		else \
			echo "ERROR: pandoc is required to show help messages and manuals. Please install pandoc to proceed."; \
			exit 1; \
		fi; \
	fi

-include tools/build/make/includes.mk

ifneq ($(filter $(MAKECMDGOALS),$(NOCONFIG_TARGETS)),)
  # This is only ran if they wanted to make the config without any config
else
  $(MAKECMDGOALS): precheck
endif

include Nbuild

build: $(NBUILD-T)
	$(T)$(LOG) -e "NBUILD\t COMPLETE"

$(NBUILD-T):
	$(T)$(LOG) -e "NBUILD\t $(NBUILD-T)"
	$(MAKE) -C $@

.PHONY: precheck build $(TARGETS) $(NBUILD-T)

