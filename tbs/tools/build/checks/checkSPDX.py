# SPDX-License-Identifier: GPL-3.0
#
# Check SPDX
# Checks for SPDX licence in file
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

import os

IGNOREP = ["./ext", "./doc", "./.git", "./systems/linux/kernel/vega/busybox", "./ext"]
IGNOREA = ["/build/"]
IGNOREF = [".txt", ".md", "README", ".ref", ".json"]


def list_dir_tree(start_path):
    for root, dirs, files in os.walk(start_path):
        for file in files:
            ignore = False
            fp = os.path.join(root, file)
            for IGNORE in IGNOREF:
                if file.endswith(IGNORE):
                    ignore = True
            for IGNORE in IGNOREP:
                if root.startswith(IGNORE):
                    ignore = True
            for IGNORE in IGNOREA:
                if IGNORE in root:
                    ignore = True
            f = open(fp, "r")
            try:
                if "SPDX" not in f.readlines()[0] and not ignore:
                    print("FAIL:", fp)
            except IndexError:
                print("FAIL:", fp)
            except UnicodeDecodeError:
                continue


list_dir_tree(".")
