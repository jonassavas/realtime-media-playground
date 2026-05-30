#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "packet.hpp"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in receiver{};
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &receiver.sin_addr);

    int counter = 0;

    while (true)
    {
        std::string msg = "packet " + std::to_string(counter++);

        sendto(sock, msg.c_str(), msg.size(), 0,
            (sockaddr*)&receiver, sizeof(receiver));

        std::cout << "Sent: " << msg << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    closesocket(sock);
    WSACleanup();
}