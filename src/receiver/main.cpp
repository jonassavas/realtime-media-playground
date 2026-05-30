#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "packet.hpp"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (sockaddr*)&addr, sizeof(addr));

    std::cout << "Receiver listening on port 5000...\n";

    uint8_t buffer[1024];

    while (true)
    {
        sockaddr_in senderAddr{};
        int addrLen = sizeof(senderAddr);

        int bytes = recvfrom(sock, (char*)buffer, sizeof(buffer), 0,
            (sockaddr*)&senderAddr, &addrLen);

        if (bytes > 0)
        {
            std::cout << "Received packet of size: " << bytes << "\n";
        }
    }

    closesocket(sock);
    WSACleanup();
}