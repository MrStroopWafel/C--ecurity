#include <Windows.h>
#include <iostream>

int main()
{
    char system32[MAX_PATH];
    GetSystemDirectoryA(system32, sizeof(system32));

    std::cout << system32 << std::endl;

    return 0;
 }