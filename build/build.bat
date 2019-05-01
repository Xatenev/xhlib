:: Build DLL, link user32.lib
cl /LD ../xhlib/xhlib.c /link user32.lib
:: Build .exe, link xhlib
cl ../main.c /link xhlib.lib
:: Execute main
main.exe