# For Win32: cc main.c -lSDL3 -ldwmapi $CFLAGS $LDFLAGS -Wextra -Wpedantic -mwindows -o minisaver.exe
cc main.c -lSDL3 $CFLAGS $LDFLAGS -Wextra -Wpedantic -o minisaver
