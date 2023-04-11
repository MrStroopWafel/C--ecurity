#include <iostream>
#include <filesystem>
#include <fstream>
#include <Windows.h>


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
    const char* src = current_File.c_str(); //dit is vgm niet nodig, ook regel hierboven

    // get system32 path
    char system32[MAX_PATH];
    GetSystemDirectoryA(system32, sizeof(system32));

    // convert to string and add file extension 
    std::string tempString = system32;
    tempString.append("\\test.exe");

    //convert final string to const char*
    const char* dest = tempString.c_str();
    //const char* dest = "C:\\Users\\Public\\test.exe";
    std::cout << tempString << std::endl;
    return copyFile(src, dest) ? 0 : 1;
}
