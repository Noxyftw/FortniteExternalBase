#pragma once
#include <Windows.h>
#include <TlHelp32.h>  // <-- add this

namespace dyn {

    using pCreateFileA = HANDLE(WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
    using pDeviceIoControl = BOOL(WINAPI*)(HANDLE, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
    using pCreateToolhelp32Snapshot = HANDLE(WINAPI*)(DWORD, DWORD);
    using pProcess32FirstA = BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32);
    using pProcess32NextA = BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32);

    struct Imports {
        pCreateFileA              CreateFileA;
        pDeviceIoControl          DeviceIoControl;
        pCreateToolhelp32Snapshot CreateToolhelp32Snapshot;
        pProcess32FirstA          Process32FirstA;
        pProcess32NextA           Process32NextA;
    };

    const Imports& get(); // resolved once, cached
}