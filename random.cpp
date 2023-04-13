#include <Windows.h>
#include <iostream>

int main()
{
    const char* dirX = "D:\\zuyd\\C++ecurity\\test.exe";
    SetFileAttributes(dirX,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);
    //SetFileAttributes(path,FILE_ATTRIBUTE_NORMAL);
 }