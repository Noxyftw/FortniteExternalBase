#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
) 
#define IOCTL_ACCESS_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0xB1, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0xB2, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_MOUSE_MOVEMENT CTL_CODE(FILE_DEVICE_UNKNOWN, 0xA1, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// extra_information = ulExtraInfo from real mouse input (Fortnite requires it)
typedef struct _MOVEMOUSE_REQUEST {
    long x;
    long y;
    unsigned short button_flags;
    ULONG extra_information;
} MOVEMOUSE_REQUEST, * PMOVEMOUSE_REQUEST;

typedef struct _RW_OPERATION
{
    INT32 PID;
    ULONGLONG Address, Buffer, Size;
    BOOLEAN Write;
    BOOLEAN Translate;
} RWOperation, * pRWOperation;

typedef struct _BASE_ADDR_QUERY
{
    INT32 PID;
    ULONGLONG* Address;
} BaseAddrQuery, * pBaseAddrQuery;


class DRIVER_CLASS
{
public:
    HANDLE DriverHandle;
    INT32 ProcessID;

    bool Init();
    void ReadPhysicalMemory(PVOID address, PVOID buffer, DWORD size);
    void readphys(PVOID address, PVOID buffer, DWORD size);
    void WritePhysicalMemory(PVOID address, PVOID buffer, DWORD size);
    uintptr_t GetBase();
    INT32 FindProcess(LPCTSTR process_name);
    void MoveMouse(long x, long y, unsigned short button_flags, ULONG extra_information);
}; inline DRIVER_CLASS driver;

class MEMORY_CLASS
{
public:
    ULONGLONG BaseAddress;

    template <typename T>
    T read(uint64_t address)
    {
        T buffer{ };
        driver.ReadPhysicalMemory((PVOID)address, &buffer, sizeof(T));
        return buffer;
    }

    template <typename T>
    T write(uint64_t address, T buffer)
    {
        driver.WritePhysicalMemory((PVOID)address, &buffer, sizeof(T));
        return buffer;
    }
}; inline MEMORY_CLASS memory;