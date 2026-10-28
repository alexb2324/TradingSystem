#include "MessageSource.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <system_error>

MessageSource::MessageSource(const std::filesystem::path& filePath)
:   mapping_ { MAP_FAILED }, mappingSize_ {}
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
    void* mapping = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);

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

MessageSource::~MessageSource()
{
    if (mapping_ != MAP_FAILED) { munmap(mapping_, mappingSize_); }
}

std::span<const std::uint8_t> MessageSource::allBytes() const noexcept
{
    if (mapping_ == MAP_FAILED) { return {}; }
    return {static_cast<const std::uint8_t*>(mapping_), mappingSize_};
}
