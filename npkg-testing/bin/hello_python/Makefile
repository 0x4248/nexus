# SPDX-License-Identifier: GPL-3.0

BIN = hello-python
DESTDIR ?=
INSTALL_PATH ?= /usr/bin/

.PHONY: all install clean

all:
	chmod +x $(BIN)

install: all
	install -d $(DESTDIR)$(INSTALL_PATH)
	install -m 0755 $(BIN) $(DESTDIR)$(INSTALL_PATH)$(BIN)

clean:
	true
