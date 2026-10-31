#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <system_error>

#include "../Usings.h"

class UdpSender
{
public:
    UdpSender(const std::string& address, const std::uint16_t port)
    :   socketfd_ { socket(AF_INET, SOCK_DGRAM, 0) }
    {

        if (socketfd_ == -1)
        {
            throw std::system_error(errno, std::system_category(), "Failed to open socket");
        }

        // Setup server address struct
        std::memset(&destAddr_, 0, sizeof(destAddr_));
        destAddr_.sin_family = AF_INET;
        destAddr_.sin_port = htons(port);
        if (inet_pton(AF_INET, address.c_str(), &destAddr_.sin_addr) <= 0)
        {
            close(socketfd_);
            throw std::system_error(errno, std::system_category(), "Error occurred with address conversion");
        }
        std::println("Created UDP socket to address {}, on port {}", address, port);
    }


    void sendPacket(itch::Bytes packet)
    {
        const auto sentBytes = sendto(socketfd_,
            packet.data(),
            packet.size(),
            0,
            reinterpret_cast<sockaddr*>(&destAddr_),
            sizeof(destAddr_)
        );

        if (sentBytes == -1)
        {
            throw std::system_error(errno, std::system_category(), "Something went wrong while sending packet.");
        }

        sentBytes_ += static_cast<std::size_t>(sentBytes);
    }

    UdpSender(const UdpSender& other) = delete;
    UdpSender operator=(const UdpSender& other) = delete;
    UdpSender(UdpSender&& other) = delete;
    UdpSender operator=(UdpSender&& other) = delete;

    ~UdpSender()
    {
        close(socketfd_);
    }

private:
    int socketfd_;
    sockaddr_in destAddr_;
    std::size_t sentBytes_ {};
};

