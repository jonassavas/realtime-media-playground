#pragma once

#include <cstdint>
#include <array>

struct Packet
{
    uint32_t sequence_number;
    uint64_t timestamp_ms;
    uint16_t payload_size;
};

// Convert packet --> bytes
inline std::array<uint8_t, 14> serialize(const Packet& p)
{
    std::array<uint8_t, 14> data{};

    std::memcpy(&data[0], &p.sequence_number, 4);
    std::memcpy(&data[4], &p.timestamp_ms, 8);
    std::memcpy(&data[12], &p.payload_size, 2);

    return data;
}

// bytes --> packet
inline Packet deserialize(const uint8_t* data)
{
    Packet p{};

    std::memcpy(&p.sequence_number, data, 4);
    std::memcpy(&p.timestamp_ms, data + 4, 8);
    std::memcpy(&p.payload_size, data + 12, 2);

    return p;
}