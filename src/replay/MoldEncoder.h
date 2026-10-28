#pragma once

#include <algorithm>

#include "../parser/Decode.h"
#include "../Usings.h"

class MoldEncoder {

public:

    MoldEncoder() = default;

    [[nodiscard]] bool tryAddMessage();





private:

    void writePacketHeader() noexcept
    {
        std::memcpy(packetBuffer_.data(), sessionId_.data(), sessionId_.size());
        storeBigEndian(packetBuffer_.data() + 10, sequenceNumber_);
        storeBigEndian(packetBuffer_.data() + 18, messageCount_);
    }

    std::array<uint8_t, 1500> packetBuffer_ {};
    std::size_t offset_ {20};
    std::size_t sequenceNumber_ {1};
    std::uint16_t messageCount_ {};
    std::array<std::uint8_t, 10> sessionId_ {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
};
