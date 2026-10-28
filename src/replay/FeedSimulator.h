#pragma once

#include "MoldUDP64Encoder.h"
#include "MessageGenerator.h"
#include "Pacer.h"
#include "UdpSender.h"
#include "../datasource/filesource/FileSource.h"

class FeedSimulator
{
public:
    explicit FeedSimulator(const std::filesystem::path& filePath)
    :   fileSource_ { filePath },
        messageSource_ {fileSource_.bytes()}
    {}

    void run()
    {
        auto message = messageSource_.peekNextMessage();



        encoder_.updateSequenceNumber();
        if (encoder_.tryAddMessage(message))
        {
        }
        else
        {
            auto packet = encoder_.finish();
            udpSender_.sendPacket(packet);
            encoder_.cleanup();

        }
    }

private:

    FileSource fileSource_;
    MessageGenerator messageSource_;
    MoldUDP64Encoder encoder_;
    Pacer pacer_ {};
    UdpSender udpSender_ {};
};
