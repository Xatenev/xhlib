#include <stdio.h>

#include "xhlib/xhlib.h"

int main(void) {

    HANDLE handle = xhInitialize("AssaultCube");

    BOOL success = xhInjectDLL(handle, "test.dll");

    printf("%d", success);
/*

    // Ammo
	uintptr_t address = 0x50F4F4, offsets[1] = {0x150};

    // Health
	uintptr_t address2 = 0x50F4F4, offsets2[1] = {0xF8};

    DWORD value = 96;
    xhWriteInteger4B(handle, address2, offsets2, value);

    // Name*/
/*
    uintptr_t address3 = 0x50F4F4, offsets3[1] = {0x225};

    char buffer[128] = "";
    xhReadString(handle, address3, offsets3, buffer, 128);
    int oldSize = strlen(buffer);

    
    char buffer2[] = "meh";
    xhWriteString(handle, address3, offsets3, buffer2);
    printf("%s", buffer);*/

    xhCleanup(handle);
    return 0;
}