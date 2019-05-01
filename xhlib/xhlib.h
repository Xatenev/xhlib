#include <windows.h>

#define EXPORT __declspec(dllexport)

int xhLastError = 0;

#define XH_ERR_WINDOW_NOT_FOUND 1
#define XH_ERR_PROCESS_NOT_FOUND 2

EXPORT HANDLE xhInitialize(char procName[]);

EXPORT unsigned int xhGetLastError();

EXPORT void xhReadString(HANDLE proc, uintptr_t addr, int offsets[], char* buffer, int bufferSize);
EXPORT void xhWriteString(HANDLE proc, uintptr_t addr, int offsets[], char *buffer);

EXPORT DWORD xhReadInteger4B(HANDLE proc, uintptr_t addr, int offsets[]);
EXPORT void xhWriteInteger4B(HANDLE proc, uintptr_t addr, int offsets[], DWORD value);

EXPORT void* xhResolvePointer(HANDLE proc, uintptr_t addr, int offsets[]);

EXPORT BOOL xhListWindowNames();
EXPORT BOOL xhListProcessNames();

BOOL CALLBACK printProcessNameAndId(DWORD procId);
BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam);

void setXhLastError(int errorId);