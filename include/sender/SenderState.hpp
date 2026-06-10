#pragma once

#include <queue>
#include <cstdint>
#include <iostream>

#include "DelayedPacket.hpp"

struct SenderState
{
    std::queue<DelayedPacket> delay_queue;

    uint32_t seq = 0;
    uint64_t packets_sent = 0;
    uint64_t packets_dropped = 0;

    void printStats()
    {
        std::cout
            << "\nSender Stats:\n"
            << "Sent:    " << packets_sent << "\n"
            << "Dropped: " << packets_dropped << "\n"
            << "\n";
    }
};