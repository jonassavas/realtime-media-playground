#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "packet.hpp"

#pragma comment(lib, "ws2_32.lib")

struct ReceiverStats
{
    uint64_t packets_received = 0;
    uint64_t packets_lost = 0;
    uint64_t out_of_order = 0;
};

int main()
{
    WSADATA wsaData;
    // Winsock version 2.2
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Adress Family Internet/IPv4, Datagram socket/UDP, Default protocol
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Define where we want to listen for packets - Structure: IP Address + port
    sockaddr_in addr{};
    addr.sin_family = AF_INET; // IPv4
    // PC (little-endian), Network (big-endian). 
    // htons: host to network short, 5000 -> network byte order
    addr.sin_port = htons(5000);
    // Listen on every local interface
    addr.sin_addr.s_addr = INADDR_ANY;

    // Attatch this socket to port 5000
    bind(sock, (sockaddr*)&addr, sizeof(addr));

    std::cout << "Receiver listening on port 5000...\n";

    Packet p;

    ReceiverStats stats;

    bool first_packet = true;
    uint32_t last_seq = 0;

    while (true)
    {   
        // recv blocks until data arrives
        // then stores the data directly into the packet object
        recv(sock, (char*)&p, sizeof(p), 0);

        stats.packets_received++;

        uint32_t expected = last_seq + 1;

        // Detect packet loss
        if (!first_packet && p.sequence_number != expected)
        {
            std::cout
                << "Packet loss detected! Expected "
                << expected
                << " but got "
                << p.sequence_number
                << "\n";

            uint32_t missing = p.sequence_number - expected;
            stats.packets_lost += missing;
        }

        std::cout << "Received packet " << p.sequence_number << "\n";

        // Packet loss statistics
        if (stats.packets_received % 10 == 0)
        {
            std::cout
                << "\nStats:\n"
                << "Received: " << stats.packets_received << "\n"
                << "Lost:     " << stats.packets_lost << "\n"
                << "\n";
        }

        last_seq = p.sequence_number;
        first_packet = false;
    }

    closesocket(sock); // Release socket resources
    WSACleanup();      // Shuts down Winsock
}