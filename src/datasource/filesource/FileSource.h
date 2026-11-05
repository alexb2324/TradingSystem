#pragma once

#include <cstdint>
#include <span>
#include <filesystem>
#include <print>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <system_error>

class FileSource
{
public:

    explicit FileSource(const std::filesystem::path& filePath, bool pagingFlag = false)
    :   mapping_ { MAP_FAILED },
        mappingSize_ {}
    {
        const int fd = open(filePath.c_str(), O_RDONLY);

        if (fd == -1)
        {
            throw std::system_error(errno, std::system_category(), "Failed to open file");
        }

        struct stat fileInfo {};

        if (fstat(fd, &fileInfo) == -1)
        {
            const int error = errno;
            close(fd);
            throw std::system_error(error, std::system_category(), "Failed to get file stats");
        }

        if (fileInfo.st_mode <= 0)
        {
            close(fd);
            throw std::runtime_error("Invalid file size");
        }

        const auto fileSize = static_cast<std::size_t>(fileInfo.st_size);

        void* mapping = nullptr;

        if (pagingFlag)
        {
            mapping = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
            std::println("Faulted in all pages!");
        }
        else
        {
            mapping = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
        }

        if (mapping == MAP_FAILED)
        {
            const int error = errno;
            close(fd);
            throw std::system_error(error, std::system_category(), "Failed to map file");
        }

        close(fd);

        mapping_ = mapping;
        mappingSize_ = fileSize;
    }

    FileSource(const FileSource& other) = delete;
    FileSource& operator=(const FileSource& other) = delete;
    FileSource(FileSource&& other) = delete;
    FileSource& operator=(FileSource&& other) = delete;

    ~FileSource()
    {
        if (mapping_ != MAP_FAILED) { munmap(mapping_, mappingSize_); }
    }

    [[nodiscard]] std::span<const std::uint8_t> bytes() const noexcept
    {
        if (mapping_ == MAP_FAILED) { return {}; }
        return {static_cast<const std::uint8_t*>(mapping_), mappingSize_};
    }

private:
    void* mapping_;
    std::size_t mappingSize_;
};