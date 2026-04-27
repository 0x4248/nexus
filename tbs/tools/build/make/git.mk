# SPDX-License-Identifier: GPL-3.0
# Nexus Makefile System
#
# git.mk
# GIT based targets
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

g-log:
	$(T)$(LOG) -e "GIT\tLOG"
	$(T)git log

g-acp:
	$(T)$(LOG) -e "GIT\t ADD > COMMIT > PUSH"
	$(T)git add *
	$(T)git commit --signoff
	$(T)git push

g-cs:
	$(T)$(LOG) -e "GIT\t COMMIT (SPECIAL)"
	$(T)printf "\n\n" > COMMIT_MSG.txt

	$(T)name=$$(git config user.name); \
	email=$$(git config user.email); \
	printf "Signed-off-by: %s <%s>" "$$name" "$$email" >> COMMIT_MSG.txt

	$(T)echo "A file named COMMIT_MSG.txt, has been created for you"
	$(T)echo "Once you are done press enter or crtl c to cancel:"
	$(T)read

	$(T)git commit -F COMMIT_MSG.txt
	$(T)mv COMMIT_MSG.txt COMMIT_MSG.old



g-mod:
	$(T)git submodule update --recursive --remote

TARGETS += g-log g-acp
