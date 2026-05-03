# SPDX-License-Identifier: GPL-3.0
# V/UX
#
# Makefile
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

DIR := $(shell pwd)
BUSYBOX_DIR := $(DIR)/busybox
BUILD_DIR := $(DIR)/build
ROOTFS_TMP := /tmp/busybox

INITRAMFS := $(BUILD_DIR)/initramfs.cpio.gz
QCOW_IMG := $(BUILD_DIR)/busybox.qcow2

all: $(INITRAMFS) $(QCOW_IMG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

busybox: $(BUILD_DIR)
	cd $(BUSYBOX_DIR) && make && make install

$(ROOTFS_TMP): busybox
	rm -rf $(ROOTFS_TMP)
	mkdir -p $(ROOTFS_TMP)
	cp -r $(BUSYBOX_DIR)/_install/* $(ROOTFS_TMP)
	chmod +x fs/init
	cp fs/* $(ROOTFS_TMP)

	# device nodes
	cd $(ROOTFS_TMP) && \
		mkdir -p dev && \
		mknod dev/console c 5 1 && \
		mknod dev/ram b 1 0 && \
		mknod dev/tty c 5 0 && \
		mknod dev/tty1 c 4 1 && \
		mknod dev/fb0 c 29 0

$(INITRAMFS): $(ROOTFS_TMP)
	cd $(ROOTFS_TMP) && \
		find . -print0 | cpio -0oH newc | gzip -9 > $(INITRAMFS)

$(QCOW_IMG): busybox
	qemu-img create -f qcow2 $(QCOW_IMG) 1G
	@echo "# QCOW2 created. You must manually copy files into it."

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(ROOTFS_TMP)

.PHONY: all busybox clean
