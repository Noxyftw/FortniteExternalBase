// dyn_imports.cpp
#include "dyn_imports.hxx"

namespace {

    FARPROC resolve(const wchar_t* mod, const char* name) {
        HMODULE h = GetModuleHandleW(mod);
        if (!h) h = LoadLibraryW(mod);
        return h ? GetProcAddress(h, name) : nullptr;
    }

}

namespace dyn {

    const Imports& get() {
        static Imports imp{};
        static bool init = false;
        if (!init) {
            imp.CreateFileA = reinterpret_cast<pCreateFileA>(resolve(L"kernel32.dll", "CreateFileA"));
            imp.DeviceIoControl = reinterpret_cast<pDeviceIoControl>(resolve(L"kernel32.dll", "DeviceIoControl"));
            imp.CreateToolhelp32Snapshot = reinterpret_cast<pCreateToolhelp32Snapshot>(resolve(L"kernel32.dll", "CreateToolhelp32Snapshot"));
            imp.Process32FirstA = reinterpret_cast<pProcess32FirstA>(resolve(L"kernel32.dll", "Process32First"));
            imp.Process32NextA = reinterpret_cast<pProcess32NextA>(resolve(L"kernel32.dll", "Process32Next"));
            // You can add more as needed.
            init = true;
        }
        return imp;
    }

}