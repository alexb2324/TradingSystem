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

using namespace std::chrono_literals;

class FeedSimulator
{

public:
    FeedSimulator(const std::filesystem::path& filePath, const std::string& address, std::uint16_t port, std::uint32_t playbackSpeed, std::uint64_t replayStartTime)
    :   fileSource_ { filePath },
        messageSource_ {fileSource_.bytes() },
        pacer_ { playbackSpeed, replayStartTime },
        udpSender_ {address, port}
    {
        std::println("Mapped file successfully.");
    }

    void run()
    {
        std::println("Starting message parsing...");
        std::this_thread::sleep_for(1s);

        while (const auto message = messageSource_.message())
        {
            auto timeNow = std::chrono::steady_clock::now();

            if (!pacer_.isDue(message->timestamp, timeNow))
            {
                if (!encoder_.empty())
                {
                    udpSender_.sendPacket(encoder_.finish());
                }

                const auto wakeTime = pacer_.dueAt(message->timestamp);

                while (timeNow + std::chrono::seconds(1) < wakeTime)
                {
                    std::this_thread::sleep_until(timeNow + std::chrono::seconds(1));
                    udpSender_.sendPacket(encoder_.heartBeat());
                    timeNow += std::chrono::seconds(1);
                }

                std::this_thread::sleep_until(wakeTime);
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

        if (!encoder_.empty()) { udpSender_.sendPacket(encoder_.finish()); }

        const auto endSessionPacket = encoder_.endOfSession();
        udpSender_.sendPacket(endSessionPacket);
        std::println("Shutdown");
    }

private:
    FileSource fileSource_;
    MessageGenerator messageSource_;
    MoldUDP64Encoder encoder_;
    Pacer pacer_;
    UdpSender udpSender_;
    bool running_ { true };
};
