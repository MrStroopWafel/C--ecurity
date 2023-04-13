#include <Shlobj.h>
#include <string>
#include <comdef.h>


#include <iostream>

// Handmatige fix voor FOLDERID_Documents
#ifndef FOLDERID_Documents
const GUID FOLDERID_Documents = {0xFDD39AD0, 0x238F, 0x46AF, {0xAD, 0xB4, 0x6C, 0x85, 0x48, 0x03, 0x69, 0xC7}};
#endif

int hiddenFolder()
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
    //CreateDirectoryW(hiddenFolderPath.c_str(), nullptr) && SetFileAttributesW(hiddenFolderPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    std::cout << "dirX: " << hiddenFolderPath.c_str() << std::endl << std::endl << std::endl;
    return 0;
}

int main() {
    hiddenFolder();
    return 0;
}