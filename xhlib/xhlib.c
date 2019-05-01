#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <wchar.h>
#include <stdint.h>
#include <psapi.h>

#include "xhlib.h"

/**
 * Initialize a handle
 */
EXPORT HANDLE xhInitialize(char procName[])
{
    DWORD windowId;
    HWND window = FindWindowA(NULL, procName);
    GetWindowThreadProcessId(window, &windowId);

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, windowId);

    return handle;
}

/**
 * Read a string (truncated after X bytes)
 */
EXPORT void xhReadString(HANDLE proc, uintptr_t addr, int offsets[], char *buffer, int bufferSize)
{
    void *ptr = xhResolvePointer(proc, addr, offsets);

    ReadProcessMemory(proc, ptr, buffer, bufferSize, NULL);
}

/**
 * Read an integer that is 4byte (DWORD)
 */
EXPORT DWORD xhReadInteger4B(HANDLE proc, uintptr_t addr, int offsets[])
{
    DWORD value = 0;
    void *ptr = xhResolvePointer(proc, addr, offsets);

    ReadProcessMemory(proc, ptr, &value, sizeof(value), NULL);

    return value;
}

/**
 * Write an integer that is 4byte (DWORD)
 */
EXPORT void xhWriteInteger4B(HANDLE proc, uintptr_t addr, int offsets[], DWORD value) {
    void *ptr = xhResolvePointer(proc, addr, offsets);

    WriteProcessMemory(proc, ptr, &value, sizeof(value), NULL);
}

/**
 * Resolve a (possibly) multipointer chain
 */
EXPORT void *xhResolvePointer(HANDLE proc, uintptr_t addr, int offsets[])
{
    char *target = (char *)addr;

    for (int i = 0; i < sizeof offsets / sizeof(int); i++)
    {
        ReadProcessMemory(proc, target, &target, sizeof(target), 0);

        target += offsets[i];
    }

    return (void *)target;
}

/* Development helper functions */

/**
 * List all process names
 */ 
EXPORT BOOL xhListProcessNames()
{
    DWORD processes[1024], cbNeeded;
    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded))
    {
        return FALSE;
    }

    puts("Process List");
    puts("--------------------------------------------");
    for (int i = 0; i < sizeof processes / sizeof(DWORD); i++)
    {
        if (processes[i] > 0)
        {
            printProcessNameAndId(processes[i]);
        }
    }
    puts("--------------------------------------------");

    return TRUE;
}

/**
 * List all window names
 */ 
EXPORT BOOL xhListWindowNames()
{
    puts("Window List");
    puts("--------------------------------------------");
    EnumWindows(enumWindowCallback, NULL);
    puts("--------------------------------------------");

    return TRUE;
}

/* Not exported */
BOOL CALLBACK printProcessNameAndId(DWORD procId)
{
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

    if (handle == NULL)
    {
        return FALSE;
    }

    HMODULE mod;
    DWORD cbNeeded;
    char moduleName[1024] = "";
    char titleName[1024] = "";

    if (!EnumProcessModules(handle, &mod, sizeof(mod), &cbNeeded))
    {
        return FALSE;
    }

    GetModuleBaseNameA(handle, mod, moduleName, sizeof moduleName);

    printf("ProcessId: %d, ProcessName: %s\n", procId, moduleName);

    CloseHandle(handle);

    return TRUE;
}

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam)
{
    int length = GetWindowTextLength(hWnd);
    char buffer[1024] = "";
    GetWindowTextA(hWnd, buffer, sizeof(buffer));

    // Ignore windows if invisible or missing a title
    if (IsWindowVisible(hWnd) && length != 0)
    {
        printf("Window Name: %s\n", buffer);
    }

    return TRUE;
}