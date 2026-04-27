# SPDX-License-Identifier: GPL-3.0
# Nexus
#
# Github install dependencies script
#
# Since github actions doesn't always have the required dependencies installed
# we need to install them manually. This script will install the required
# dependencies for the CI to work.
# COPYRIGHT NOTICE
# Copyright (C) 2024-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    gfortran \
    nasm
