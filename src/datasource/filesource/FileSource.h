#pragma once

#include <cstdint>
#include <string_view>
#include <span>

class FileSource
{
public:
    explicit FileSource(const std::string_view filePath);
    FileSource(const FileSource& other) = delete;
    FileSource& operator=(const FileSource& other) = delete;
    FileSource(FileSource&& other) = delete;
    FileSource& operator=(FileSource&& other) = delete;
    ~FileSource();

    [[nodiscard]] std::span<const std::uint8_t> bytes() const noexcept;

private:
    void* mapping_;
    std::size_t mappingSize_;
};