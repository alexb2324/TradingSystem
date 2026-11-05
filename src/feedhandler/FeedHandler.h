#pragma once

#include <thread>
#include <print>
#include <array>
#include <memory>

#include "UdpReceiver.h"
#include "MoldUDP64Decoder.h"
#include "../parser/Dispatch.h"
#include "../marketstate/MarketState.h"
#include "../BookEngine/BookEngine.h"

using namespace std::chrono_literals;

class FeedHandler
{
public:

    FeedHandler(const std::string& address, const std::uint16_t port, BookEngine& engine)
    :   receiver_ {address, port},
        engine_ { engine }
    { }

    void run(const std::stop_token& stopToken)
    {
        while (running_ && !stopToken.stop_requested())
        {
            if (auto packet = receiver_.receivePacket())
            {
                onDatagram(*packet);
            }
            else if ((packet.error() == ReceiverError::Timeout && --allowableTimeouts_ == 0) || packet.error() == ReceiverError::Error)
            {
                running_ = false;
            }
            engine_.tryPublishSnapshot();
        }

        engine_.tryPublishSnapshot(true);
        printSummary();
    }

    void onDatagram(const itch::Bytes packet)
    {
        const auto decodedPacket = decodePacket(packet);

        if (!decodedPacket) { ++badPackets_; return; }

        if (decodedPacket->session != session_) { ++invalidSessionPackets_; return; }

        if (expectedSequenceNumber_ < decodedPacket->sequenceNumber)
        {
            ++gaps_;
            missedMessages_ += decodedPacket->sequenceNumber - expectedSequenceNumber_;
            expectedSequenceNumber_ = decodedPacket->sequenceNumber; // Skip ahead for now
        }

        if (decodedPacket->packetType == PacketType::Heartbeat) { return; }
        if (decodedPacket->packetType == PacketType::EndOfSession) { running_ = false; return; }

        std::size_t readOffset {};
        while (readOffset + Constants::kItchMessageOffset <= decodedPacket->bytes.size())
        {
            const auto msgLength = convertBLEndian<itch::MessageLength>(decodedPacket->bytes.data() + readOffset);

            if (readOffset + Constants::kItchMessageOffset + msgLength > decodedPacket->bytes.size()) { break; }
            if (msgLength == 0) { break; }

            // Dispatch to engine for decoding, processing, publishing
            engine_.onMessage(decodedPacket->bytes.subspan(readOffset + Constants::kItchMessageOffset, msgLength));
            readOffset += Constants::kItchMessageOffset + msgLength;

            ++expectedSequenceNumber_;
        }
        processedBytes_ += readOffset;
    }

    void printSummary() const noexcept
    {
        std::println("Finished handling feed!");
        std::println("Total bytes processed - {}", formatBytes(processedBytes_));
        std::println("Total bad packets detected - {}", badPackets_);
        std::println("Total invalid session packets detected - {}", invalidSessionPackets_);
        std::println("Total gaps periods - {}", gaps_);
        std::println("Total messages missed - {}", missedMessages_);
        std::println("Total timeouts triggered - {} / 3", 3 - allowableTimeouts_);
    }

    static std::string formatBytes(const std::size_t bytes)
    {
        std::string byteString = std::to_string(bytes);

        for (std::size_t i = byteString.length() - 3; i > 0; i -= 3)
        {
            byteString.insert(i, ",");
            if (i <= 3) { break; }
        }

        return byteString;
    }

private:
    UdpReceiver receiver_;
    BookEngine& engine_;
    std::uint64_t expectedSequenceNumber_ { 1 };
    std::array<char, Constants::kMoldSessionIdSize> session_ {'H', 'e', 'l', 'l', 'o', 'T', 'h', 'e', 'r', 'e'};
    std::size_t badPackets_ {};
    std::size_t invalidSessionPackets_ {};
    std::size_t gaps_ {};
    std::size_t missedMessages_ {};
    std::size_t processedBytes_ {};
    std::size_t allowableTimeouts_ { 3 };
    bool running_ { true };
};