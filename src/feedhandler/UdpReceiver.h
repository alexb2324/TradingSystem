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
        std::memset(&serverAddr_, 0, sizeof(serverAddr_));
        serverAddr_.sin_family = AF_INET; // IPv4

        if (inet_pton(AF_INET, address.c_str(), &serverAddr_.sin_addr) <= 0) // Bind to 127 exclusively
        {
            throw std::system_error(errno, std::system_category(), "Error occurred with address conversion");
        }

        serverAddr_.sin_port = htons(port);

        if (bind(socketfd_, reinterpret_cast<sockaddr*>(&serverAddr_), sizeof(serverAddr_)) == -1)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Failed to bind to socket");
        }
    
        std::println("UDP Server listening on port {} ...", port);
    }

    itch::Bytes receivePackets()
    {
        socklen_t len = sizeof(clientAddr_);
        
        // 4. Receive data
        const auto receivedBytes = recvfrom(socketfd_,
            networkBuffer_.data(),
            networkBuffer_.size(),
            0, 
            reinterpret_cast<sockaddr*>(&clientAddr_),
            &len);
        
        if (receivedBytes == -1) 
        {
            std::println("Error occurred while receiving data");
            return {};
        }
        return {networkBuffer_.data(), static_cast<std::size_t>(receivedBytes)};
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
    std::array<uint8_t, 65'536> networkBuffer_ {};
    sockaddr_in serverAddr_ {};
    sockaddr_in clientAddr_ {};
};