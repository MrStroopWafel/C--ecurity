#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <WinSock2.h>

using namespace std;

int main() {

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
    serverAddr.sin_addr.s_addr = inet_addr("172.27.64.1"); //IP van receiving machine

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
