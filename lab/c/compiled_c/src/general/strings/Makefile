# Compiled C
# A collection of C programs that are compiled to assembly 
# code for understanding how C and assembly work.
# GitHub: https://wwww.github.com/0x4248/Compiled_C
# Licence: GNU General Public Licence v3.0
# By: 0x4248

include ../../../config.mk

MAIN = main.c
BUILD = build

all: clean init compile

init:
	@mkdir -p $(BUILD)

compile:
	@$(GCC) $(CFLAGS) $(MAIN) -o $(BUILD)/compiled
	@$(OBJ_DUMP) -d -s -t -r -w $(BUILD)/compiled > $(BUILD)/objdump.txt
	@$(GCC) $(CFLAGS) -S $(MAIN) -o $(BUILD)/assembly.s

clean:
	@rm -rf $(BUILD)