#!/bin/sh

set -xe

CC="cc"
CFLAGS="-Wall -Wextra -Iinclude"
LFLAGS="-lelf"

if [ ! -d ./build ]; then
    mkdir ./build
fi

$CC $CFLAGS -o ./build/main ./src/main.c ./src/vasm.c ./src/vasm_x86_64.c $LFLAGS
