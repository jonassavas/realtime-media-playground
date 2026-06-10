#pragma once

#include <cstdint>
#include <iostream>

struct ReceiverState
{
    uint64_t packets_received = 0;
    uint64_t packets_lost = 0;
    uint64_t out_of_order = 0;

    uint64_t total_latency_ms = 0;

    bool first_packet = true;
    uint32_t last_seq = 0;

    void printStats()
    {
        double avg_latency =
            packets_received > 0
            ? static_cast<double>(total_latency_ms) / packets_received
            : 0.0;

        std::cout
            << "\nReceiver Stats:\n"
            << "Received:       " << packets_received << "\n"
            << "Lost:           " << packets_lost << "\n"
            << "Out-of-order:   " << out_of_order << "\n"
            << "Avg latency:    " << avg_latency << " ms\n"
            << "\n";
    }
};