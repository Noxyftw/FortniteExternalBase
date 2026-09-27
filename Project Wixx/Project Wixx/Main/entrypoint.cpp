#include "../Bytes/NToskrnl.h"
#include "../Bytes/RT_CoreVuln64.h"
#include "../Driver/Driver.hxx"
#include "../Settings/Settings.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include <urlmon.h>
#include <wininet.h>
#include <utility>
#include <limits>
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>
#include <cstdlib>
#include <thread>

#include <conio.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <wincrypt.h>
#include <tlhelp32.h>
#include <chrono>
#include <vector>

#include <algorithm>
#include <shlwapi.h>
#include "../Overlay/Overlay.h"



#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "bcrypt.lib")

#include <fstream>
#include <string>


bool CreateFileFromMemory(const std::string& desired_file_path, const char* address, size_t size)
{
    std::ofstream file_ofstream(desired_file_path.c_str(), std::ios_base::out | std::ios_base::binary);

    if (!file_ofstream.write(address, size))
    {
        file_ofstream.close();
        return false;
    }

    file_ofstream.close();
    return true;
}

void Driver() {

    Sleep(1500);
    std::cout << "\n\n   [ * ] Loading Driver!";
    Sleep(1500);
    system("cls");

    auto Mapper = "C:\\Windows\\Tasks\\RT_Core64.exe";
    auto Driver = "C:\\Windows\\Tasks\\Asus_Driver_help.sys";

    CreateFileFromMemory(Mapper, reinterpret_cast<const char*>(RT_CoreVuln64), sizeof(RT_CoreVuln64));
    CreateFileFromMemory(Driver, reinterpret_cast<const char*>(NToskrnl), sizeof(NToskrnl));

    Sleep(3000);

    system("C:\\Windows\\Tasks\\RT_Core64.exe C:\\Windows\\Tasks\\Asus_Driver_help.sys");
    remove("C:\\Windows\\Tasks\\RT_Core64.exe");
    remove("C:\\Windows\\Tasks\\Asus_Driver_help.sys");

    if (driver.Init()) {
        std::cout << "[ + ] Driver Loaded!";
        Sleep(1500);
        system("cls");
    }

    if (!driver.Init()) {
        std::cout << "[ - ] Driver Not Loaded!";
        Sleep(1500);
        system("cls");
    }
  
}

int main() {

    globals.ScreenX = GetSystemMetrics(SM_CXSCREEN);
    globals.ScreenY = GetSystemMetrics(SM_CYSCREEN);
    globals.ScreenXHALF = globals.ScreenX / 2;
    globals.ScreenYHALF = globals.ScreenY / 2;

    Driver();


    std::cout << "Welcome, User!";
    Sleep(1500);

    system("cls");
    Sleep(500);

    DWORD procID = 0;
    bool game = false;

    while (!game) {

        procID = driver.FindProcess(L"FortniteClient-Win64-Shipping.exe");

        if (procID == 0) {

            system("cls");

           

            std::cout << "      [ * ] Open Fortnite... \n\n" << std::endl;

            Sleep(2000);

        }
        else {

            game = true;

           

            system("cls");

            std::cout << "      [ + ] Fortnite Found!\n" << std::endl;

            Sleep(500);

           
        }
    }

    memory.BaseAddress = 0;

    while (!memory.BaseAddress) {

        memory.BaseAddress = driver.GetBase();

        if (!memory.BaseAddress) {


            system("cls");

            
        }
        else {
          

        }
    }


    
        Beep(500, 500);
        Sleep(500);

	


  

    overlay::start();

}