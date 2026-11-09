#pragma once

#include <cstdint>
#include <array>
#include <optional>

#include "../../parser/Decode.h"
#include "configs/MoldUDP64Constants.h"


enum class PacketType : std::uint8_t
{
    Data,
    Heartbeat,
    EndOfSession
};

struct DecodedPacket
{
    std::array<char, moldudp64::constants::kMoldSessionIdSize> session;
    std::uint64_t sequenceNumber;
    std::uint16_t messageCount;
    PacketType packetType;
    itch::Bytes bytes;
};


[[nodiscard]] inline std::optional<DecodedPacket> decodePacket(const itch::Bytes datagram)
{
    if (datagram.size() < moldudp64::constants::kPacketHeaderLength) { return std::nullopt; }

    DecodedPacket packet {};

    std::memcpy(packet.session.data(), datagram.data(), 10);
    packet.sequenceNumber = convertBLEndian<std::uint64_t>(datagram.data() + 10);
    packet.messageCount = convertBLEndian<std::uint16_t>(datagram.data() + 18);

    if (packet.messageCount == 0 && datagram.size() == moldudp64::constants::kPacketHeaderLength)
    {
        packet.packetType = PacketType::Heartbeat;
        return packet;
    }

    if (packet.messageCount == moldudp64::constants::kEOSMessageCount && datagram.size() == moldudp64::constants::kPacketHeaderLength)
    {
        packet.packetType = PacketType::EndOfSession;
        return packet;
    }

    packet.packetType = PacketType::Data;
    packet.bytes = datagram.subspan(moldudp64::constants::kPacketHeaderLength, datagram.size() - moldudp64::constants::kPacketHeaderLength);

    return packet;
}