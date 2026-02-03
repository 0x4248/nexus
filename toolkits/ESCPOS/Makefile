all: build run

build:
	mkdir -p build
	gcc src/escpos.c src/escpos_commands.h -o build/escpos

run: 
	python3 src/server.py