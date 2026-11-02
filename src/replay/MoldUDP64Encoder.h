#pragma once

#include <algorithm>
#include <cassert>

#include "../parser/Decode.h"
#include "../Usings.h"
#include "../Constants.h"

class MoldUDP64Encoder
{

public:
    MoldUDP64Encoder() = default;

    [[nodiscard]] bool tryAddMessage(const itch::Bytes message)
    {
        if (currentOffset_ + message.size() > Constants::kMaxPacketSize) { return false; }

        std::memcpy(packetBuffer_.data() + currentOffset_, message.data(), message.size());
        currentOffset_ += message.size();
        ++messageCount_;

        return true;
    }

    // Create completed data packet
    [[nodiscard]] itch::Bytes finish() noexcept
    {
        assert(messageCount_ > 0);
        writePacketHeader();
        const itch::Bytes packet {packetBuffer_.data(), currentOffset_};

        // Cleanup
        packetSequenceNumber_ += messageCount_;
        messageCount_ = 0;
        currentOffset_ = Constants::kPacketHeaderLength;
        return packet;
    }

    [[nodiscard]] bool empty() const noexcept { return messageCount_ == 0; }

    [[nodiscard]] itch::Bytes heartBeat() noexcept
    {
        assert(messageCount_ == 0);
        // Pass values to writePacketHeader() so you keep the class's state in case still building packet.
        writePacketHeader(packetSequenceNumber_, 0);
        // Return and send only the header
        return { packetBuffer_.data(), Constants::kPacketHeaderLength };
    }

    [[nodiscard]] itch::Bytes endOfSession() noexcept
    {
        assert(messageCount_ == 0);
        // Pass values to writePacketHeader() so you keep the class's state in case still building packet.
        writePacketHeader(packetSequenceNumber_, Constants::kEOSMessageCount);
        // Return and send only the header
        return { packetBuffer_.data(), Constants::kPacketHeaderLength };
    }

private:

    // Use Encoder state for header creation, for data packets
    void writePacketHeader() noexcept
    {
        std::memcpy(packetBuffer_.data(), sessionId_.data(), sessionId_.size());
        storeBigEndian<std::uint64_t>(packetBuffer_.data() + 10, packetSequenceNumber_);
        storeBigEndian<std::uint16_t>(packetBuffer_.data() + 18, messageCount_);
    }

    // For heartbeat and EOS packets
    void writePacketHeader(const std::uint64_t sequenceNumber, const std::uint16_t messageCount) noexcept
    {
        std::memcpy(packetBuffer_.data(), sessionId_.data(), sessionId_.size());
        storeBigEndian<std::uint64_t>(packetBuffer_.data() + 10, sequenceNumber);
        storeBigEndian<std::uint16_t>(packetBuffer_.data() + 18, messageCount);
    }

    std::array<uint8_t, Constants::kPacketBufferSize> packetBuffer_ {};
    std::size_t currentOffset_ { Constants::kPacketHeaderLength };
    std::uint64_t packetSequenceNumber_ { 1 };
    std::uint16_t messageCount_ {};
    std::array<char, Constants::kMoldSessionIdSize> sessionId_ {'H', 'e', 'l', 'l', 'o', 'T', 'h', 'e', 'r', 'e'};
};
