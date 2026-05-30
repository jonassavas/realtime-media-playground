#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <thread>

#include "packet.hpp"

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

    uint32_t seq = 0;

    while (true)
    {
        Packet p;
        p.sequence_number = seq++;
        p.timestamp_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()
            ).count();

        sendto(
            sock,
            (const char*)&p,
            sizeof(p),
            0,
            (sockaddr*)&receiver,
            sizeof(receiver)
        );

        std::cout << "Sent packet " << p.sequence_number << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    closesocket(sock);
    WSACleanup();
}