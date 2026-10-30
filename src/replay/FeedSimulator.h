#pragma once

#include <print>
#include <string_view>

#include "MoldUDP64Encoder.h"
#include "MessageGenerator.h"
#include "UdpSender.h"
#include "../datasource/filesource/FileSource.h"

class FeedSimulator
{

public:
    explicit FeedSimulator(const std::filesystem::path& filePath, const std::string& address, const std::uint16_t port)
    :   fileSource_ { filePath },
        messageSource_ {fileSource_.bytes()},
        udpSender_ {address, port}
    {
        std::println("Mapped file successfully.");
    }

    void run()
    {
        std::println("Starting message parsing...");

        while (const auto message = messageSource_.message())
        {
            if (!encoder_.tryAddMessage(message->bytes))
            {
                const auto packet = encoder_.finish();
                udpSender_.sendPacket(packet);
            }
            else { messageSource_.prepareNextMessage(); }
        }

        if (!encoder_.empty())
        {
            const auto packet = encoder_.finish();
            udpSender_.sendPacket(packet);
        }

        const auto endSessionPacket = encoder_.endOfSession(123);
        udpSender_.sendPacket(endSessionPacket);
    }

private:

    FileSource fileSource_;
    MessageGenerator messageSource_;
    MoldUDP64Encoder encoder_;
    UdpSender udpSender_;
};
