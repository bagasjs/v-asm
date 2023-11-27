CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Iinclude

all: ./build ./build/v-asm

./build/v-asm: ./src/main.c ./src/vasm.c ./src/vasm_elf.c ./src/vasm_x86_64.c
	$(CC) $(CFLAGS) -o $@ $^


./build:
	mkdir $@
