#include <iostream>
#include <fstream>
#include <string>
#include <WinSock2.h>

using namespace std;

int main() {
    // Initialize Winsock
    //cerr << "Passed 0" << endl;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error: Failed to initialize Winsock" << endl;
        return -1;
    }
    cerr << "Initialized Winsock" << endl;

    // Create a socket and bind it to a local address
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
        cerr << "Error: Failed to create socket" << endl;
        WSACleanup();
        return -1;
    }
    cerr << "Created socket" << endl;
    struct sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(5000);
    localAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSock, (SOCKADDR*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        cerr << "Error: Failed to bind socket" << endl;
        closesocket(serverSock);
        WSACleanup();
        return -1;
    }
    cerr << "Binded socket" << endl;

    // Listen for incoming connections
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error: Failed to listen on socket" << endl;
        closesocket(serverSock);
        WSACleanup();
        return -1;
    }
    cerr << "Listening on socket" << endl;

    // Accept the first incoming connection
    SOCKET clientSock = accept(serverSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) {
        cerr << "Error: Failed to accept incoming connection" << endl;
        closesocket(serverSock);
        WSACleanup();
        return -1;
    }
    cerr << "Accepted incoming connection" << endl;

    // Open the file for writing
    ofstream file("myfile.txt", ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Failed to open file for writing" << endl;
        closesocket(clientSock);
        closesocket(serverSock);
        WSACleanup();
        return -1;
    }
    cerr << "Opened file for reading" << endl;

    // Receive the file data over the socket
    const int bufferSize = 1024;
    char buffer[bufferSize];
    int bytesReceived;
    do {
        bytesReceived = recv(clientSock, buffer, bufferSize, 0);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Error: Failed to receive data over socket" << endl;
            file.close();
            closesocket(clientSock);
            closesocket(serverSock);
            WSACleanup();
            return -1;
        }
    cerr << "Received data over socket" << endl;
        file.write(buffer, bytesReceived);
    } while (bytesReceived > 0);

    // Close the file and the sockets
    file.close();
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}