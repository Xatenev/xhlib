#include <windows.h>

#define EXPORT __declspec(dllexport)

EXPORT HANDLE xhInitialize(char procName[]);
EXPORT BOOL xhCleanup(HANDLE proc);

EXPORT BOOL xhReadString(HANDLE proc, uintptr_t addr, int offsets[], char* buffer, int bufferSize);
EXPORT BOOL xhWriteString(HANDLE proc, uintptr_t addr, int offsets[], char *buffer);

EXPORT DWORD xhReadInteger4B(HANDLE proc, uintptr_t addr, int offsets[]);
EXPORT BOOL xhWriteInteger4B(HANDLE proc, uintptr_t addr, int offsets[], DWORD value);

EXPORT BOOL xhInjectDLL(HANDLE proc, char* dllPath);

EXPORT void* xhResolvePointer(HANDLE proc, uintptr_t addr, int offsets[]);

EXPORT BOOL xhListWindowNames();
EXPORT BOOL xhListProcessNames();

BOOL CALLBACK printProcessNameAndId(DWORD procId);
BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam);
