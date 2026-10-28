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

    [[nodiscard]] const SourceMessage& peekNextMessage() const noexcept { return nextMessage_; }
    void advance()
    {

    }


private:

    void decodeNextMessage()
    {
        if (currentOffset_ + Constants::kItchMessageOffset > fileData_.size()) { throw; }

        const auto msgLength = convertBLEndian<itch::MessageLength>(fileData_.data() + currentOffset_);

        if (currentOffset_ + Constants::kItchMessageOffset + msgLength > fileData_.size()) { throw; }
        if (msgLength == 0) { throw;}

        nextMessage_ = {.bytes = fileData_.subspan(currentOffset_, Constants::kItchMessageOffset + msgLength),
            .timestamp = convertBLEndian48(fileData_.data() + 5),
            .sequenceNumber = sequenceNumber_};

        currentOffset_ += Constants::kItchMessageOffset + msgLength;
        ++sequenceNumber_;
    }

private:
    SourceMessage nextMessage_ {};
    itch::Bytes fileData_ {};
    std::size_t currentOffset_ {};
    std::size_t sequenceNumber_ {1};
};