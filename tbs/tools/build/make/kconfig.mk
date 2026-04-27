# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# kconfig.mk
# Kconfig based targets
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

defconfig:
	$(T)$(LOG) -e "KCONFIG\tDEFCONFIG"
	$(T)$(PYTHON) ext/kconfiglib/defconfig.py

menuconfig:
	$(T)$(LOG) -e "KCONFIG\tMENUCONFIG"
	$(T)$(PYTHON) ext/kconfiglib/menuconfig.py

genconfig:
	$(T)$(LOG) -e "KCONFIG\tGENCONFIG"
	$(T)$(PYTHON) ext/kconfiglib/genconfig.py

guiconfig:
	$(T)$(LOG) -e "KCONFIG\tGUICONFIG"
	$(T)$(PYTHON) ext/kconfiglib/guiconfig.py

config-allyes:
	$(T)$(LOG) -e "KCONFIG\tALLYESCONFIG"
	$(T)$(PYTHON) ext/kconfiglib/allyesconfig.py

config-allno:
	$(T)$(LOG) -e "KCONFIG\tALLNOCONFIG"
	$(T)$(PYTHON) ext/kconfiglib/allnoconfig.py

TARGETS += defconfig menuconfig genconfig guiconfig config-allyes config-allno
