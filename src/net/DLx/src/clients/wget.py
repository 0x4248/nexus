# SPDX-License-Identifier: GPL-3.0
# DLX
#
# Bulk download tool
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

import subprocess

class Client:
    def download(self, url, local_path):
        try:
            subprocess.run(['wget', '-O', local_path, url], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Failed to download {url} using wget. Error: {e}")