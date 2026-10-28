#pragma once

#include <algorithm>

#include "../parser/Decode.h"
#include "../Usings.h"

class MoldUDP64Encoder
{

public:
     MoldUDP64Encoder() = default;

    [[nodiscard]] bool tryAddMessage(const itch::Bytes message)
    {
        if (currentOffset_ + message.size() > packetBuffer_.size()) { return false; }
        std::memcpy(packetBuffer_.data() + currentOffset_, message.data(), message.size());
        currentOffset_ += message.size();
        ++messageCount_;

        return true;
    }

    void cleanup() noexcept
    {
        std::fill(packetBuffer_.begin(), packetBuffer_.end(), 0);
        messageCount_ = 0;
        currentOffset_ = 20;
    }

    [[nodiscard]] itch::Bytes finish()
    {
        writePacketHeader();
        return packetBuffer_;
    }

    [[nodiscard]] itch::Bytes heartBeat(const std::size_t nextSeqNumber)
    {
        sequenceNumber_ = nextSeqNumber;
        return finish();
    }

    void updateSequenceNumber(const std::size_t seqNumber) { sequenceNumber_ = seqNumber; }

    [[nodiscard]] itch::Bytes endOfSession();


private:

    void writePacketHeader() noexcept
    {
        std::memcpy(packetBuffer_.data(), sessionId_.data(), sessionId_.size());
        storeBigEndian(packetBuffer_.data() + 10, sequenceNumber_);
        storeBigEndian(packetBuffer_.data() + 18, messageCount_);
    }

    std::array<uint8_t, 1500> packetBuffer_ {};
    std::size_t currentOffset_ {20};
    std::size_t sequenceNumber_ {};
    std::uint16_t messageCount_ {};
    std::array<std::uint8_t, 10> sessionId_ {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
};
