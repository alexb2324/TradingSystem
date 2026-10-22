#include <cstdint>
#include <cstddef>
#include <fcntl.h>
#include <stdexcept>
#include <filesystem>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <print>

#include "parser/Messages.h"
#include "parser/Decode.h"

#include "tradebook/Tradebook.h"

#include "directory/StockDirectory.h"
#include "parser/Dispatch.h"

namespace fs = std::filesystem;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{
    const int fd = open("/home/hehexd2324/CLionProjects/TradingSystem/res/01302019.NASDAQ_ITCH50", O_RDONLY);

    if (fd == -1) { throw std::runtime_error("Couldn't open file"); }

    struct stat fileInfo {};

    if (fstat(fd, &fileInfo) == -1)
    {
        close(fd);
        throw std::runtime_error("Couldn't get file stats");
    }

    if (fileInfo.st_size == 0)
    {
        close(fd);
        std::println("File is empty");
        return 0;
    }

    const auto fileSize = static_cast<std::size_t>(fileInfo.st_size);

    void* fileMapping = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);

    if (fileMapping == MAP_FAILED)
    {
        close(fd);
        throw std::runtime_error("Failed to map file");
    }

    close(fd);

    std::span<const std::uint8_t> data {static_cast<const std::uint8_t*>(fileMapping), fileSize};
    MarketState marketStateHandler {};
    const auto parsedBytes = parseData(data, marketStateHandler);

    if (parsedBytes == fileSize)
    {
        std::println("Parsing finished");
    }

    if (munmap(fileMapping, fileSize) == -1)
    {
        throw std::runtime_error("Failed to unmap file");
    }

    return 0;
}