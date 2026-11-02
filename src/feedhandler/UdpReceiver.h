#pragma once

#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <system_error>
#include <expected>
#include <cerrno>
#include <array>

#include "../Usings.h"


enum class ReceiverError
{
    Timeout,
    Error
};

class UdpReceiver
{

public:
    UdpReceiver(const std::string& address, std::uint16_t port)
    :   socketfd_ { socket(AF_INET, SOCK_DGRAM, 0) }
    {
        if (socketfd_ == -1)
        {
            throw std::system_error(errno, std::system_category(), "Failed to open socket");
        }

        // Setup server address struct
        std::memset(&receiverAddr_, 0, sizeof(receiverAddr_));
        receiverAddr_.sin_family = AF_INET; // IPv4
        receiverAddr_.sin_port = htons(port);

        if (inet_pton(AF_INET, address.c_str(), &receiverAddr_.sin_addr) <= 0) // Bind to 127.0.0.1 exclusively
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Error occurred with address conversion");
        }

        if (bind(socketfd_, reinterpret_cast<sockaddr*>(&receiverAddr_), sizeof(receiverAddr_)) == -1)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Failed to bind to socket");
        }

        // Set timeout for socket
        if (setsockopt(socketfd_, SOL_SOCKET, SO_RCVTIMEO, &receiverTimeoutConfig_, sizeof(receiverTimeoutConfig_)) == -1)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Failed to set socket settings");
        }

        // Set socket buffer size
        if (setsockopt(socketfd_, SOL_SOCKET, SO_RCVBUF, &receiverBufferSize_, sizeof(receiverBufferSize_)) == -1)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Failed to set socket buffer size");
        }

        std::println("UDP Server listening on port {} ...", port);
    }

    std::expected<itch::Bytes, ReceiverError> receivePacket()
    {
        while (true)
        {
            // Receive packet
            const auto receivedBytes = recvfrom(socketfd_,
                packetBuffer_.data(),
                packetBuffer_.size(),
                0,
                nullptr,
                nullptr);

            if (receivedBytes >= 0)
            {
                return itch::Bytes { packetBuffer_.data(), static_cast<std::size_t>(receivedBytes) };
            }

            if (errno == EINTR) { continue; }
            if (errno == EAGAIN || errno == EWOULDBLOCK) { return std::unexpected { ReceiverError::Timeout }; }
            return std::unexpected { ReceiverError::Error };
        }
    }

    UdpReceiver(const UdpReceiver& other) = delete;
    UdpReceiver& operator=(const UdpReceiver& other) = delete;
    UdpReceiver(UdpReceiver&& other) = delete;
    UdpReceiver& operator=(UdpReceiver&& other) = delete;

    ~UdpReceiver()
    {
        close(socketfd_);
    }

private:
    int socketfd_ {};
    std::array<uint8_t, Constants::kPacketBufferSize> packetBuffer_ {};
    sockaddr_in receiverAddr_ {};
    std::size_t receiverBufferSize_ { 8'388'608 }; // Request 8 MB buffer to handle traffic
    timeval receiverTimeoutConfig_ = { 5, 0 }; // 5 seconds timeout
};