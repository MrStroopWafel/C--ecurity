#include <iostream>
#include <filesystem>
#include <fstream>

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
    const char* exe = current_File.c_str();

    //TODO: change this to the path of system 32, read this out dynamically
    const char* dest = "C:\\Users\\Public\\test.exe";
    


    std::cout << exe << std::endl;
    return copyFile(exe, dest) ? 0 : 1;
}