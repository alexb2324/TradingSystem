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

    [[nodiscard]] itch::Bytes finish() noexcept
    {
        writePacketHeader();
        const itch::Bytes packet {packetBuffer_.data(), currentOffset_};

        // Cleanup
        packetSequenceNumber_ += messageCount_;
        messageCount_ = 0;
        currentOffset_ = 20;
        return packet;
    }

    [[nodiscard]] bool empty() const noexcept { return messageCount_ == 0; }

    [[nodiscard]] itch::Bytes heartBeat(const std::size_t nextSeqNumber)
    {
        packetSequenceNumber_ = nextSeqNumber;
        messageCount_ = 0;
        writePacketHeader();
        return {packetBuffer_.data(), currentOffset_};;
    }

    [[nodiscard]] itch::Bytes endOfSession()
    {
        messageCount_ = 0xFFFF;
        writePacketHeader();
        return {packetBuffer_.data(), currentOffset_};
    }


private:

    void writePacketHeader() noexcept
    {
        std::memcpy(packetBuffer_.data(), sessionId_.data(), sessionId_.size());
        storeBigEndian(packetBuffer_.data() + 10, packetSequenceNumber_);
        storeBigEndian(packetBuffer_.data() + 18, messageCount_);
    }

    std::array<uint8_t, 1500> packetBuffer_ {};
    std::size_t currentOffset_ {20};
    std::size_t packetSequenceNumber_ {1};
    std::uint16_t messageCount_ {};
    std::array<std::uint8_t, 10> sessionId_ {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
};
