#include <windows.h>

#define EXPORT __declspec(dllexport)

EXPORT HANDLE xhInitialize(char procName[]);
EXPORT DWORD xhReadInteger4B(HANDLE proc, uintptr_t addr, int offsets[]);
EXPORT void xhReadString(HANDLE proc, uintptr_t addr, int offsets[], char* buffer, int bufSize);
EXPORT void xhWriteInteger4B(HANDLE proc, uintptr_t addr, int offsets[], DWORD value);
EXPORT void* xhResolvePointer(HANDLE proc, uintptr_t addr, int offsets[]);

EXPORT BOOL xhListWindowNames();
EXPORT BOOL xhListProcessNames();

BOOL CALLBACK printProcessNameAndId(DWORD procId);
BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam);