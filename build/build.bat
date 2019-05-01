cl /LD ../xhlib/xhlib.c /link user32.lib
cl ../main.c /link xhlib.lib
main.exe