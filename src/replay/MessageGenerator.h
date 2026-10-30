#pragma once

#include <cstdint>
#include <span>
#include <filesystem>

#include "../parser/Decode.h"

#include "../Constants.h"
#include "../Usings.h"


struct SourceMessage
{
    itch::Bytes bytes;
    std::uint64_t timestamp;
    std::uint64_t sequenceNumber;
};


class MessageGenerator
{
public:
    explicit MessageGenerator(itch::Bytes data)
        : fileData_ { data }
    {
        decodeNextMessage();
        if (!nextMessage_) { throw std::runtime_error("Not able to decode messages from source"); }
    }

    void prepareNextMessage() noexcept
    {
        decodeNextMessage();
    }

    [[nodiscard]] std::optional<SourceMessage> message() const
    {
        return nextMessage_;
    }


private:

    void decodeNextMessage() noexcept
    {
        if (currentOffset_ + Constants::kItchMessageOffset > fileData_.size()) { return; }

        const auto msgLength = convertBLEndian<itch::MessageLength>(fileData_.data() + currentOffset_);

        if (currentOffset_ + Constants::kItchMessageOffset + msgLength > fileData_.size()) { return; }
        if (msgLength == 0) { return; }

        nextMessage_ = {.bytes = fileData_.subspan(currentOffset_,  + msgLength),
            .timestamp = convertBLEndian48(fileData_.data() + currentOffset_ + Constants::kItchMessageOffset + 5),
            .sequenceNumber = sequenceNumber_};

        currentOffset_ += Constants::kItchMessageOffset + msgLength;
        ++sequenceNumber_;
    }


private:
    std::optional<SourceMessage> nextMessage_ {};
    itch::Bytes fileData_ {};
    std::size_t currentOffset_ {};
    std::uint64_t sequenceNumber_ {1};
};