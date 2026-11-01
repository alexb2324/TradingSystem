#pragma once

#include <print>
#include <thread>
#include <chrono>
#include <cmath>

#include "MoldUDP64Encoder.h"
#include "MessageGenerator.h"
#include "UdpSender.h"
#include "Pacer.h"
#include "../datasource/filesource/FileSource.h"
#include "../console/Logger.h"

using namespace std::chrono_literals;

class FeedSimulator
{

public:
    FeedSimulator(const std::filesystem::path& filePath, const std::string& address, std::uint16_t port, std::uint64_t replayStartTime)
    :   fileSource_ { filePath },
        messageSource_ {fileSource_.bytes() },
        udpSender_ {address, port},
        pacer_ { replayStartTime }
    {
        std::println("Mapped file successfully.");
    }

    void run()
    {
        std::println("Starting message parsing...");
        std::this_thread::sleep_for(1s);

        while (const auto message = messageSource_.message())
        {
            if (!pacer_.isDue(message->timestamp, std::chrono::steady_clock::now()))
            {
                if (!encoder_.empty()) { udpSender_.sendPacket(encoder_.finish()); }
                std::this_thread::sleep_until(pacer_.dueAt(message->timestamp));
                continue;
            }

            if (!encoder_.tryAddMessage(message->bytes))
            {
                udpSender_.sendPacket(encoder_.finish());
                continue;
            }

            messageSource_.prepareNextMessage();
        }

        //if (!running_) { return; }

        if (!encoder_.empty())
        {
            const auto packet = encoder_.finish();
            udpSender_.sendPacket(packet);
        }

        const auto endSessionPacket = encoder_.endOfSession(encoder_.nextSequenceNumber());
        udpSender_.sendPacket(endSessionPacket);
        std::println("Shutdown");
    }

private:
    FileSource fileSource_;
    MessageGenerator messageSource_;
    MoldUDP64Encoder encoder_;
    Pacer pacer_;
    UdpSender udpSender_;
    Logger logger_;
    bool running_ { true };
};
