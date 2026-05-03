# SPDX-License-Identifier: GPL-3.0
# Quick Floppy - Easy folder to floppy image converter
#
# Process:
# 1. Turn each file from unix to dos
# 2. Create a floppy image with the same name as the folder
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

#prechecks

echo "0x4248 QuickTools - QuickFloppy v1.0"
echo "Copyright (C) 2025-2026 0x4248 and contributors"
echo "==============================================="

find "$1" -type f -exec unix2dos {} \;
if [ -d "$1" ]; then
    imgname="$(basename "$1").img"
    echo "Creating floppy image: $imgname"
    dd if=/dev/zero of="$imgname" bs=512 count=2880
    echo "Formatting floppy image with FAT12 filesystem..."
    /sbin/mkfs.msdos -F 12 -n "$(basename "$1")" "$imgname"
    echo "Copying files to floppy image..."
    cd "$1"
    mcopy -i "../$imgname" -s * ::/
    cd ..
    echo "$1 -> $imgname"
    echo "Nothing else to complete."
else
    echo "Usage: $0 <folder>"
fi