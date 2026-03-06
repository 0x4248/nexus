# SPDX-License-Identifier: GPL-3.0

CC ?= gcc
CFLAGS ?= -O2 -Wall

PKG_NAME = hello-world
PKG_VERSION = 0.1.0

DESTDIR ?=
INSTALL_PATH ?= /usr/bin/

SRC = src/hello_world.c
BUILD_DIR = build
BIN = hello-world

.PHONY: all clean install

all: $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/$(BIN)

install: all
	install -d $(DESTDIR)$(INSTALL_PATH)
	install -m 0755 $(BUILD_DIR)/$(BIN) $(DESTDIR)$(INSTALL_PATH)$(BIN)

clean:
	rm -rf $(BUILD_DIR)
