#include <iostream>
#include <winsock2.h> // Socket API
#include <ws2tcpip.h> // Networking: inet_pton: IPV4 --> binary ip
#include <chrono>     // Generate timestamps
#include <thread>

#include "packet.hpp"

#pragma comment(lib, "ws2_32.lib") // Link against the winsock library

int main()
{
    WSADATA wsaData;
    // Winsock version 2.2
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Adress Family Internet/IPv4, Datagram socket/UDP, Default protocol
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Define where we should send packets - Structure: IP Address + port
    sockaddr_in receiver{};
    receiver.sin_family = AF_INET; // IPv4
    // PC (little-endian), Network (big-endian). 
    // htons: host to network short, 5000 -> network byte order
    receiver.sin_port = htons(5000); 
    // Converts IPv4 address into binary.
    // pton: presentation to network: presentation(human-readable) --> binary
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
            sock,                 // Which socket to use
            (const char*)&p,      // Treat p as raw bytes (naive serialization)
            sizeof(p),            // Number of bytes to send.
            0,                    // Flags, currently unused.
            (sockaddr*)&receiver, // Destination address: 127.0.0.1:5000
            sizeof(receiver)      // Size of the destination structure
        );

        std::cout << "Sent packet " << p.sequence_number << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    closesocket(sock); // Release socket resources
    WSACleanup();      // Shuts down Winsock
}