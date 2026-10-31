#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>

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
    [[nodiscard]] std::uint64_t nextSequenceNumber() const noexcept { return packetSequenceNumber_ + messageCount_; }

    [[nodiscard]] itch::Bytes heartBeat(const std::uint64_t nextSeqNumber) noexcept
    {
        packetSequenceNumber_ = nextSeqNumber;
        messageCount_ = 0;
        writePacketHeader();
        return {packetBuffer_.data(), Constants::kPacketHeaderLength};;
    }

    [[nodiscard]] itch::Bytes endOfSession(const std::uint64_t nextSeqNumber) noexcept
    {
        packetSequenceNumber_ = nextSeqNumber;
        messageCount_ = Constants::kHeartBeatMessageCount;
        writePacketHeader();
        return {packetBuffer_.data(), Constants::kPacketHeaderLength};
    }


private:

    void writePacketHeader() noexcept
    {
        std::memcpy(packetBuffer_.data(), sessionId_.data(), sessionId_.size());
        storeBigEndian(packetBuffer_.data() + 10, packetSequenceNumber_);
        storeBigEndian(packetBuffer_.data() + 18, messageCount_);
    }

    std::array<uint8_t, Constants::kPacketBufferSize> packetBuffer_ {};
    std::size_t currentOffset_ { Constants::kPacketHeaderLength };
    std::uint64_t packetSequenceNumber_ {1};
    std::uint16_t messageCount_ {};
    std::array<char, Constants::kMoldSessionIdSize> sessionId_ {'H', 'e', 'l', 'l', 'o', 'T', 'h', 'e', 'r', 'e'};
};
