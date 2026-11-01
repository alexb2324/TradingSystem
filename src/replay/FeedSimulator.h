#pragma once

#include <print>
#include <thread>
#include <chrono>

#include "MoldUDP64Encoder.h"
#include "MessageGenerator.h"
#include "UdpSender.h"
#include "Pacer.h"
#include "../datasource/filesource/FileSource.h"


using namespace std::chrono_literals;

class FeedSimulator
{

public:
    FeedSimulator(const std::filesystem::path& filePath, const std::string& address, const std::uint16_t port)
    :   fileSource_ { filePath },
        messageSource_ {fileSource_.bytes()},
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
            if (!pacer_.running()) [[unlikely]] { pacer_.startPacer(message->timestamp); }

            if (auto dueTime = pacer_.dueAt(message->timestamp); dueTime >= std::chrono::steady_clock::now())
            {
                if (!encoder_.tryAddMessage(message->bytes))
                {
                    const auto packet = encoder_.finish();
                    udpSender_.sendPacket(packet);
                    std::println("Sent a packet of bytes");
                }

                messageSource_.prepareNextMessage();
            }
            else
            {
                const auto packet = encoder_.finish();
                udpSender_.sendPacket(packet);
                std::this_thread::sleep_until(dueTime);
            }

        }

        if (!running_) { return; }

        if (!encoder_.empty())
        {
            const auto packet = encoder_.finish();
            udpSender_.sendPacket(packet);
        }

        const auto endSessionPacket = encoder_.endOfSession(encoder_.nextSequenceNumber());
        udpSender_.sendPacket(endSessionPacket);
    }

private:
    FileSource fileSource_;
    MessageGenerator messageSource_;
    MoldUDP64Encoder encoder_;
    Pacer pacer_ {};
    UdpSender udpSender_;

    bool running_ { true };
};
