
include mod/modules.mk

GCC = gcc
BUILD = build
OUT = -o build/out
INCLUDE = -I include/

all: init build

init:
	mkdir -p $(BUILD)

build:
	$(GCC) $(MK-M) $(INCLUDE) $(OUT)
	$(GCC) $(MK-M) $(INCLUDE) -S 

.PHONY: all build
