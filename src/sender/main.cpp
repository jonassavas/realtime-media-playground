#include <iostream>
#include <winsock2.h> // Socket API
#include <ws2tcpip.h> // Networking: inet_pton: IPV4 --> binary ip
#include <chrono>     // Generate timestamps
#include <thread>
#include <random>

#include "packet.hpp"
#include "sender/SenderState.hpp"

// MSVC pragma: Add the library to the linker inputs
#pragma comment(lib, "ws2_32.lib")


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

    SenderState state;

    // Random number generation to simulate packet loss
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    while (true)
    {
        Packet p;
        p.sequence_number = state.seq++;
        p.timestamp_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()
            ).count();
        // Print formatted text safetly into a fixed-size char buffer: 64 bytes
        snprintf(
            p.payload,
            sizeof(p.payload),
            "Hello from #%u",
            p.sequence_number
        );

        
        if (dist(gen) < 0.10) {
            state.packets_dropped++;

            std::cout << "Dropped packet "
                << p.sequence_number
                << "\n";
        }
        else {
            // 10% reorder (delay instead of sending immediately)
            if (dist(gen) < 0.10) {
                state.delay_queue.push({ p, 2 });
                std::cout << "Delayed packet: " << p.sequence_number << "\n";
            }
            else {
                sendto(
                    sock,                 // Which socket to use
                    (const char*)&p,      // Treat p as raw bytes (naive serialization)
                    sizeof(p),            // Number of bytes to send.
                    0,                    // Flags, currently unused.
                    (sockaddr*)&receiver, // Destination address: 127.0.0.1:5000
                    sizeof(receiver)      // Size of the destination structure
                );

                state.packets_sent++;

                std::cout << "Sent packet "
                    << p.sequence_number
                    << "\n";
            }
        }

        // Process delayed packets
        if (!state.delay_queue.empty())
        {
            auto& front = state.delay_queue.front();
            front.delay_slots--;

            if (front.delay_slots <= 0)
            {
                sendto(
                    sock,
                    (const char*)&front.packet,
                    sizeof(front.packet),
                    0,
                    (sockaddr*)&receiver,
                    sizeof(receiver)
                );

                state.packets_sent++;

                std::cout << "Released delayed packet "
                    << front.packet.sequence_number
                    << "\n";

                state.delay_queue.pop();
            }
        }

        // Packet sent & dropped stats
        if ((state.packets_sent + state.packets_dropped) % 10 == 0)
        {
            state.printStats();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    closesocket(sock); // Release socket resources
    WSACleanup();      // Shuts down Winsock
}