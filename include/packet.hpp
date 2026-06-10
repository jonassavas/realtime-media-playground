#pragma once

#include <cstdint>

struct Packet
{
    uint32_t sequence_number;
    uint64_t timestamp_ms;

    // Fixed-size avoids complexity
    // Enough for the text message
    char payload[64];
};