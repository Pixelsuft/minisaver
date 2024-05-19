if [ -z $CC ]; then
    export CC=cc
fi

$CC main.c $(sdl2-config --cflags --libs) $CFLAGS $LDFLAGS -Wextra -Wpedantic -lSDL2main -o minisaver
