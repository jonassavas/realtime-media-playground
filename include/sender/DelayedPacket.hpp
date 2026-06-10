#pragma once

#include "packet.hpp"

struct DelayedPacket
{
    Packet packet;
    int delay_slots;
};