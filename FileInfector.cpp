#include <iostream>
#include <filesystem>
#include <Windows.h>

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
    std::cout << dest << std::endl;

    CopyFile( src, dest, 0);

    return 0;
}