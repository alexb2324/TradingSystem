#pragma once

#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <system_error>
#include <print>

#include "../Usings.h"

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

        if (inet_pton(AF_INET, address.c_str(), &receiverAddr_.sin_addr) <= 0) // Bind to 127 exclusively
        {
            throw std::system_error(errno, std::system_category(), "Error occurred with address conversion");
        }

        receiverAddr_.sin_port = htons(port);

        if (bind(socketfd_, reinterpret_cast<sockaddr*>(&receiverAddr_), sizeof(receiverAddr_)) == -1)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Failed to bind to socket");
        }

        // Set timeout for socket
        const timeval timeoutConfiguration = { 5, 0 }; // 5 seconds timeout

        if (setsockopt(socketfd_, SOL_SOCKET, SO_RCVTIMEO, &timeoutConfiguration, sizeof(timeoutConfiguration)) == -1)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Failed to set socket settings");
        }
    
        std::println("UDP Server listening on port {} ...", port);
    }

    std::optional<itch::Bytes> receivePacket()
    {
        socklen_t len = sizeof(senderAddr_);
        
        // Receive packet
        const auto receivedBytes = recvfrom(socketfd_,
            networkBuffer_.data(),
            networkBuffer_.size(),
            0,
            reinterpret_cast<sockaddr*>(&senderAddr_),
            &len);

        if (receivedBytes == -1) { return std::nullopt; }

        std::println("Received {} bytes while listening", receivedBytes);

        return itch::Bytes { networkBuffer_.data(), static_cast<std::size_t>(receivedBytes) };
    }

    UdpReceiver(const UdpReceiver& other) = delete;
    UdpReceiver operator=(const UdpReceiver& other) = delete;
    UdpReceiver(UdpReceiver&& other) = delete;
    UdpReceiver operator=(UdpReceiver&& other) = delete;

    ~UdpReceiver()
    {
        close(socketfd_);
    }
    

private:
    int socketfd_ {};
    std::array<uint8_t, 1'500> networkBuffer_ {};
    sockaddr_in receiverAddr_ {};
    sockaddr_in senderAddr_ {};
};