# SPDX-License-Identifier: GPL-3.0
# V/UX
#
# build.sh
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

DIR=$(pwd)
mkdir build
cd busybox
make
make install
cd ..
mkdir /tmp/busybox
cp -r busybox/_install/* /tmp/busybox
chmod +x fs/init
cp fs/* /tmp/busybox
cd /tmp/busybox
mkdir -p dev
mknod dev/console c 5 1
mknod dev/ram b 1 0
mknod dev/tty c 5 0
mknod dev/tty1 c 4 1
mknod dev/fb0 c 29 0
find -print0 | cpio -0oH newc | gzip -9 > $DIR/build/initramfs.cpio.gz
# make a .qcow2 image of the _install so we can test the second stage
qemu-img create -f qcow2 $DIR/build/busybox.qcow2 1G
# put _/install in the qcow2 image
cd $DIR
