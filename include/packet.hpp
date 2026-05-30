#pragma once

#include <cstdint>

struct Packet
{
    uint32_t sequence_number;
    uint64_t timestamp_ms;
};