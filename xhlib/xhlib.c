#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <wchar.h>
#include <stdint.h>
#include <psapi.h>

#include "xhlib.h"

/**
 * Initialize a handle
 * 
 * Returns HANDLE on success
 * Returns NULL on failure
 */
EXPORT HANDLE xhInitialize(char procName[])
{
    DWORD windowId;
    HWND window = FindWindowA(NULL, procName);

    if (window == NULL)
    {
        return NULL;
    }

    GetWindowThreadProcessId(window, &windowId);

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, windowId);

    if (handle == NULL)
    {
        return NULL;
    }

    return handle;
}

/**
 * Read a string (truncated after X bytes)
 * 
 * Returns TRUE on success
 * Returns FALSE on failure
 */
EXPORT BOOL xhReadString(HANDLE proc, uintptr_t addr, int offsets[], char *buffer, int bufferSize)
{
    void *ptr = xhResolvePointer(proc, addr, offsets);

    return ReadProcessMemory(proc, ptr, buffer, bufferSize, NULL);
}

/**
 * Write a string that is 4byte (DWORD)
 * 
 * Returns TRUE on success
 * Returns FALSE on failure
 */
EXPORT BOOL xhWriteString(HANDLE proc, uintptr_t addr, int offsets[], char *buffer)
{
    void *ptr = xhResolvePointer(proc, addr, offsets);

    return WriteProcessMemory(proc, ptr, buffer, strlen(buffer) + 1, NULL); // add 1 for null terminator
}

/**
 * Read an integer that is 4byte (DWORD)
 */
EXPORT DWORD xhReadInteger4B(HANDLE proc, uintptr_t addr, int offsets[])
{
    DWORD value = 0;
    void *ptr = xhResolvePointer(proc, addr, offsets);

    BOOL success = ReadProcessMemory(proc, ptr, &value, sizeof(value), NULL);

    if(!success) {
        return FALSE;
    }

    return value;
}

/**
 * Write an integer that is 4byte (DWORD)
 */
EXPORT BOOL xhWriteInteger4B(HANDLE proc, uintptr_t addr, int offsets[], DWORD value)
{
    void *ptr = xhResolvePointer(proc, addr, offsets);

    return WriteProcessMemory(proc, ptr, &value, sizeof(value), NULL);
}

/** 
 * Export a DLL into a remote process
 */
EXPORT BOOL xhInjectDLL(HANDLE proc, char *dllPath)
{
    LPVOID dllPathAddressInRemoteMemory = VirtualAllocEx(proc, NULL, strlen(dllPath), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    BOOL successWrite = WriteProcessMemory(proc, dllPathAddressInRemoteMemory, dllPath, strlen(dllPath), NULL);

    if (!successWrite)
    {
        return FALSE;
    }

    // Returns a pointer to the LoadLibrary address.
    // This will be the same on the remote process as in our current process.
    LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    if (loadLibraryAddress == NULL)
    {
        return FALSE;
    }

    HANDLE remoteThread = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddressInRemoteMemory, 0, NULL);

    if(remoteThread == NULL) {
        return FALSE;
    }

    return TRUE;
}

/**
 * Resolve a (possibly) multipointer chain
 * 
 * Example: pointer => address + offset => address + offset => address + offset ...
 */
EXPORT void *xhResolvePointer(HANDLE proc, uintptr_t addr, int offsets[])
{
    char *target = (char *)addr;

    for (int i = 0; i < sizeof offsets / sizeof(int); i++)
    {
        BOOL success = ReadProcessMemory(proc, target, &target, sizeof(target), 0);

        if(!success) {
            return FALSE;
        }

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
    EnumWindows(enumWindowCallback, 0);
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