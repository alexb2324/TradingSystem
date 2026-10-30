#pragma once
#include <cstdint>
#include <array>

#include "../Usings.h"


enum class PacketType : std::uint8_t
{
    Data,
    Heartbeat,
    EndOfSession
};

struct DecodedPacket
{
    std::array<char, 10> session;
    std::uint64_t sequenceNumber;
    std::uint16_t messageCount;
    PacketType packetType;
    itch::Bytes bytes;
};


