all: build

build:
	xz -9e --format=lzma main.sh
	cat pre-run.top.sh > out.sh
	base64 main.sh.lzma >> out.sh
	cat pre-run.bottom.sh >> out.sh