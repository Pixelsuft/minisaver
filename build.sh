# For Win32: cc main.c -SDL3 -ldwmapi $CFLAGS $LDFLAGS -Wextra -Wpedantic -o minisaver.exe
cc main.c -SDL3 $CFLAGS $LDFLAGS -Wextra -Wpedantic -o minisaver
