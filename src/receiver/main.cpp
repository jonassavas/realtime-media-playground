#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "packet.hpp"

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

    Packet p;

    uint32_t last_seq = 0;
    bool first = true;

    while (true)
    {
        recv(sock, (char*)&p, sizeof(p), 0);

        if (!first && p.sequence_number != last_seq + 1)
        {
            std::cout
                << "Packet loss detected! Expected "
                << last_seq + 1
                << " but got "
                << p.sequence_number
                << "\n";
        }

        std::cout << "Received packet " << p.sequence_number << "\n";

        last_seq = p.sequence_number;
        first = false;
    }

    closesocket(sock);
    WSACleanup();
}