#include <windows.h>

#define EXPORT __declspec(dllexport)

#define XH_ERR_WINDOW_NOT_FOUND 1
#define XH_ERR_PROCESS_NOT_FOUND 2
#define XH_ERR_DLL_WPM_FAIL 3
#define XH_ERR_DLL_LOAD_LIBRARY_FAIL 4
#define XH_ERR_CREATE_THREAD_FAIL 5

EXPORT HANDLE xhInitialize(char procName[]);

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
