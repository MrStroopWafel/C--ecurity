#include <iostream>
#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <string>


// copy in binary mode
bool copyFile(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

int main(int argv, char* args[])
{
    std::string current_File = std::filesystem::current_path().string() + "\\test.exe";
    const char* exe = current_File.c_str(); //dit is vgm niet nodig, ook regel hierboven

    // get system32 path
    WCHAR system32Path[MAX_PATH];
    SHGetSpecialFolderPathW(NULL, system32Path, CSIDL_SYSTEM, FALSE);

    // convert wstring to string
    std::wstring wsystem32Path(system32Path);
    std::string system32PathStr(wsystem32Path.begin(), wsystem32Path.end());

    // append to sys32 path
    std::string destStr = system32PathStr + "\\test.exe";
    const char* dest = destStr.c_str();

    //TODO: change this to the path of system 32, read this out dynamically
    //const char* dest = "C:\\Users\\Public\\test.exe";
    
    std::cout << exe << std::endl;
    return copyFile(exe, dest) ? 0 : 1;
}
