#include <stdio.h>

#include "xhlib/xhlib.h"

int main(void) {

    HANDLE handle = xhInitialize("AssaultCube");

    // Ammo
	uintptr_t address = 0x50F4F4, offsets[1] = {0x150};

    printf("%d\n",xhReadInteger4B(handle, address, offsets));

    // Health
	uintptr_t address2 = 0x50F4F4, offsets2[1] = {0xF8};

    printf("%d",xhReadInteger4B(handle, address2, offsets2));
    DWORD value = 96;
    xhWriteInteger4B(handle, address2, offsets2, value);
    printf("%d",xhReadInteger4B(handle, address2, offsets2));


    return 0;
}