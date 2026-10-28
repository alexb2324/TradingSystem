#pragma once

#include <filesystem>


struct SimulatorConfigs
{
    std::filesystem::path filePath;
    std::string address;
    std::uint16_t port;
    std::uint64_t timestamp;
};
