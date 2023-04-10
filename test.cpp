#include <windows.h>
/*
meow-meow messagebox
author: @cocomelonc
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  ShowWindow(GetConsoleWindow(), SW_HIDE);
  MessageBoxA(NULL, "Meow-meow!","=^..^=", MB_OK);
  return 0;
}