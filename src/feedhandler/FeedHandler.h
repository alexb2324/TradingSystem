#pragma once

#include <thread>
#include <print>
#include <array>

#include "UdpReceiver.h"
#include "MoldUDP64Decoder.h"
#include "../parser/Dispatch.h"
#include "../marketstate/MarketState.h"

using namespace std::chrono_literals;

class FeedHandler 
{

public:

    FeedHandler(const std::string& address, const std::uint16_t port)
    :   receiver_ {address, port},
        marketState_ { std::make_unique<MarketState>() }
    { }

    void run()
    {
        while (running_)
        {
            if (auto packet = receiver_.receivePacket(); packet)
            {
                onDatagram(*packet);
            }
            else
            {
                std::println("Waiting 1 sec to see if packet has arrived.");
                std::this_thread::sleep_for(1s);
                running_ = false;
            }

        }
    }

    void onDatagram(itch::Bytes packet)
    {
        const auto decodedPacket = decodePacket(packet);

        if (!decodedPacket) { return; }

        if (!decodedPacket || decodedPacket->session != session_) { ++invalidSessionPackets_; return; }
        if (decodedPacket->packetType == PacketType::EndOfSession) { running_ = false; return; }

        if (expectedSequenceNumber_ < decodedPacket->sequenceNumber)
        {
            ++gaps_;
            missedMessages_ += decodedPacket->sequenceNumber - expectedSequenceNumber_;
            expectedSequenceNumber_ = decodedPacket->sequenceNumber; // Skip ahead for now
        }

        if (decodedPacket->packetType == PacketType::Heartbeat) { return; }

        std::size_t readOffset {};
        while (readOffset + Constants::kItchMessageOffset <= decodedPacket->bytes.size())
        {
            const auto msgLength = convertBLEndian<itch::MessageLength>(decodedPacket->bytes.data() + readOffset);

            if (readOffset + Constants::kItchMessageOffset + msgLength > decodedPacket->bytes.size()) { break; }
            if (msgLength == 0) { break; }

            dispatch(decodedPacket->bytes.subspan(readOffset + Constants::kItchMessageOffset, msgLength), *marketState_);
            readOffset += Constants::kItchMessageOffset + msgLength;

            ++expectedSequenceNumber_;
        }
        processedBytes_ += readOffset;
    }

private:
    UdpReceiver receiver_;
    std::unique_ptr<MarketState> marketState_;
    std::uint64_t expectedSequenceNumber_ {1};
    std::array<char, Constants::kMoldSessionIdSize> session_ {'H', 'e', 'l', 'l', 'o', 'T', 'h', 'e', 'r', 'e'};
    std::size_t invalidSessionPackets_ {};
    std::size_t gaps_ {};
    std::size_t missedMessages_ {};
    std::size_t processedBytes_ {};
    bool running_ { true };
};