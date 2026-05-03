# Key detect (Makefile)
# A simple program to detect keypresses.
# Github: https://www.github.com/0x4248/key_detect

CC = gcc
CFLAGS =

SRC = src
MAIN = main.c

BIN = bin
OUTPUT = key_detect

all: init compile

init:
	mkdir -p $(BIN)

compile:
	$(CC) $(CFLAGS) $(SRC)/$(MAIN) -o $(BIN)/$(OUTPUT)

clean:
	rm -rf $(BIN)