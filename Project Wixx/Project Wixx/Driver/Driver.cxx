#include <string>
#include <cstring>
#include "driver.hxx"
#include "dyn_imports.hxx"
#include "../Settings/skStr.h"
#include <TlHelp32.h>

namespace {
    using pCreateToolhelp32Snapshot = HANDLE(WINAPI*)(DWORD, DWORD);
    using pProcess32FirstW = BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32W);
    using pProcess32NextW = BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32W);

    struct ToolhelpImports {
        pCreateToolhelp32Snapshot CreateToolhelp32Snapshot = nullptr;
        pProcess32FirstW          Process32FirstW = nullptr;
        pProcess32NextW           Process32NextW = nullptr;
    };

    ToolhelpImports& GetToolhelp()
    {
        static ToolhelpImports imp;
        static bool initialized = false;
        if (!initialized) {
            HMODULE k32 = GetModuleHandleW(L"kernel32.dll");
            if (!k32) k32 = LoadLibraryW(L"kernel32.dll");

            imp.CreateToolhelp32Snapshot =
                reinterpret_cast<pCreateToolhelp32Snapshot>(
                    GetProcAddress(k32, "CreateToolhelp32Snapshot"));
            imp.Process32FirstW =
                reinterpret_cast<pProcess32FirstW>(
                    GetProcAddress(k32, "Process32FirstW"));
            imp.Process32NextW =
                reinterpret_cast<pProcess32NextW>(
                    GetProcAddress(k32, "Process32NextW"));

            initialized = true;
        }
        return imp;
    }
}

bool DRIVER_CLASS::Init()
{
    std::string deviceName = skCrypt("\\\\.\\LenovoEACAccess").decrypt(); //LenovoECBridge

    const auto& imp = dyn::get();
    DriverHandle = imp.CreateFileA(deviceName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0x00000080, nullptr);

    //DriverHandle = CreateFileA(deviceName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0x00000080, nullptr);

    if (!DriverHandle || (DriverHandle == INVALID_HANDLE_VALUE))
        return false;

    return true;
}

void DRIVER_CLASS::readphys(PVOID address, PVOID buffer, DWORD size) {
    _RW_OPERATION arguments = { 0 };

    arguments.Address = (ULONGLONG)address;
    arguments.Buffer = (ULONGLONG)buffer;
    arguments.Size = size;
    arguments.PID = ProcessID;
    arguments.Write = false;
    arguments.Translate = false;
    //DeviceIoControl(DriverHandle, IOCTL_ACCESS_MEMORY, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
    const auto& imp = dyn::get();
    imp.DeviceIoControl(DriverHandle, IOCTL_ACCESS_MEMORY, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
}

static constexpr DWORD kDriverMaxReadSize = 0x1000;

void DRIVER_CLASS::ReadPhysicalMemory(PVOID address, PVOID buffer, DWORD size)
{
    if (!buffer || size == 0) return;
    if (size > kDriverMaxReadSize) size = kDriverMaxReadSize;
    _RW_OPERATION Arguments = { 0 };
    Arguments.Address = (ULONGLONG)address;
    Arguments.Buffer = (ULONGLONG)buffer;
    Arguments.Size = size;
    Arguments.PID = ProcessID;
    Arguments.Write = false;
    //DeviceIoControl(DriverHandle, IOCTL_ACCESS_MEMORY, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

    const auto& imp = dyn::get();
    imp.DeviceIoControl(DriverHandle, IOCTL_ACCESS_MEMORY, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void DRIVER_CLASS::WritePhysicalMemory(PVOID address, PVOID buffer, DWORD size)
{
    if (!buffer || size == 0) return;
    if (size > kDriverMaxReadSize) size = kDriverMaxReadSize;
    _RW_OPERATION Arguments = { 0 };
    Arguments.Address = (ULONGLONG)address;
    Arguments.Buffer = (ULONGLONG)buffer;
    Arguments.Size = size;
    Arguments.PID = ProcessID;
    Arguments.Write = true;
    //DeviceIoControl(DriverHandle, IOCTL_ACCESS_MEMORY, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

    const auto& imp = dyn::get();
    imp.DeviceIoControl(DriverHandle, IOCTL_ACCESS_MEMORY, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

uintptr_t DRIVER_CLASS::GetBase()
{
    uintptr_t image_address = { NULL };
    _BASE_ADDR_QUERY Arguments = { NULL };
    Arguments.PID = ProcessID;
    Arguments.Address = (ULONGLONG*)&image_address;
    //DeviceIoControl(DriverHandle, IOCTL_GET_BASE, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

    const auto& imp = dyn::get();
    imp.DeviceIoControl(DriverHandle, IOCTL_GET_BASE, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
    return image_address;
}

INT32 DRIVER_CLASS::FindProcess(LPCWSTR process_name)
{
    auto& imp = GetToolhelp();

    if (!imp.CreateToolhelp32Snapshot || !imp.Process32FirstW || !imp.Process32NextW)
        return ProcessID; // resolution failed, keep old PID

    HANDLE hsnap = imp.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hsnap == INVALID_HANDLE_VALUE)
        return ProcessID;

    PROCESSENTRY32W pt{};
    pt.dwSize = sizeof(pt);

    if (imp.Process32FirstW(hsnap, &pt)) {
        do {
            if (_wcsicmp(pt.szExeFile, process_name) == 0) {
                CloseHandle(hsnap);
                ProcessID = pt.th32ProcessID;
                return ProcessID;
            }
        } while (imp.Process32NextW(hsnap, &pt));
    }

    CloseHandle(hsnap);
    return ProcessID;
}

void DRIVER_CLASS::MoveMouse(long x, long y, unsigned short button_flags, ULONG extra_information)
{
    if (!DriverHandle || DriverHandle == INVALID_HANDLE_VALUE) return;
    MOVEMOUSE_REQUEST req = { x, y, button_flags, extra_information };
    //DeviceIoControl(DriverHandle, IOCTL_MOUSE_MOVEMENT, &req, sizeof(req), nullptr, 0, NULL, NULL);

    const auto& imp = dyn::get();
    imp.DeviceIoControl(DriverHandle, IOCTL_MOUSE_MOVEMENT, &req, sizeof(req), nullptr, 0, NULL, NULL);
}
