#pragma once

#include <cstdint>
#include <string_view>
#include <span>
#include <filesystem>

class MessageSource
{
public:
    explicit MessageSource(const std::filesystem::path& filePath);
    MessageSource(const MessageSource& other) = delete;
    MessageSource& operator=(const MessageSource& other) = delete;
    MessageSource(MessageSource&& other) = delete;
    MessageSource& operator=(MessageSource&& other) = delete;
    ~MessageSource();

    [[nodiscard]] std::span<const std::uint8_t> allBytes() const noexcept;

    [[nodiscard]] std::span<const std::uint8_t> bytes() const noexcept;

private:
    void* mapping_;
    std::size_t mappingSize_;
    std::size_t readOffset_ {};
    std::size_t sequenceNumber_ {1};
};