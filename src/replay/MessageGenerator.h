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
    { }

    std::optional<SourceMessage> nextMessage()
    {
        if (!decodeNextMessage()) { return std::nullopt; }
        return nextMessage_;
    }


private:

    bool decodeNextMessage()
    {
        if (currentOffset_ + Constants::kItchMessageOffset > fileData_.size()) { return false; }

        const auto msgLength = convertBLEndian<itch::MessageLength>(fileData_.data() + currentOffset_);

        if (currentOffset_ + Constants::kItchMessageOffset + msgLength > fileData_.size()) { return false; }
        if (msgLength == 0) { return false; }

        nextMessage_ = {.bytes = fileData_.subspan(currentOffset_,  + msgLength),
            .timestamp = convertBLEndian48(fileData_.data() + currentOffset_ + Constants::kItchMessageOffset + 5),
            .sequenceNumber = sequenceNumber_};

        currentOffset_ += Constants::kItchMessageOffset + msgLength;
        ++sequenceNumber_;

        return true;
    }

private:
    std::optional<SourceMessage> nextMessage_ {};
    itch::Bytes fileData_ {};
    std::size_t currentOffset_ {};
    std::size_t sequenceNumber_ {1};
};