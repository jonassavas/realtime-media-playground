#include <iostream>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "packet.hpp"
#include "receiver/ReceiverState.hpp"

#pragma comment(lib, "ws2_32.lib")


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

    ReceiverState state;

    while (true)
    {   
        // recv blocks until data arrives
        // then stores the data directly into the packet object
        recv(sock, (char*)&p, sizeof(p), 0);

        state.packets_received++;

        if (state.first_packet)
        {
            state.last_seq = p.sequence_number;
            state.first_packet = false;
            continue;
        }

        auto now_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()
            ).count();

        uint64_t latency_ms = now_ms - p.timestamp_ms;

        state.total_latency_ms += latency_ms;

        uint32_t expected = state.last_seq + 1;

        // Detect packet loss
        if (p.sequence_number > expected)
        {
            uint32_t missing = p.sequence_number - expected;
            state.packets_lost += missing;

            std::cout << "Packet loss detected!\n";
        }
        else if (p.sequence_number < expected)
        {
            state.out_of_order++;

            std::cout << "Out-of-order packet detected: "
                << p.sequence_number << "\n";
        }

        std::cout 
            << "Received packet: " 
            << p.sequence_number 
            << " | payload: ["
            << p.payload
            << "] | (latency: "
            << latency_ms
            << " ms)\n";

        // Packet loss/latency statistics
        if (state.packets_received % 10 == 0)
        {
            state.printStats();
        }

        state.last_seq = p.sequence_number;
    }

    closesocket(sock); // Release socket resources
    WSACleanup();      // Shuts down Winsock
}