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
    :   socketdfd_ { socket(AF_INET, SOCK_DGRAM, 0) }
    {

        if (socketdfd_ == -1)
        {
            throw std::system_error(errno, std::system_category(), "Failed to open socket");
        }

        // Setup server address struct
        std::memset(&destAddr_, 0, sizeof(destAddr_));
        destAddr_.sin_family = AF_INET;
        destAddr_.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &destAddr_.sin_addr);
    }


    std::size_t sendPacket(itch::Bytes packet)
    {
        const auto sentBytes = sendto(socketdfd_,
            packet.data(),
            packet.size(),
            0,
            reinterpret_cast<sockaddr*>(&destAddr_),
            sizeof(destAddr_)
        );

        if (sentBytes < packet.size())
        {
            std::println("Something went wrong while sending packets.");
            return 0uz;
        }

        return static_cast<std::size_t>(sentBytes);
    }

    UdpSender(const UdpSender& other) = delete;
    UdpSender operator=(const UdpSender& other) = delete;
    UdpSender(UdpSender&& other) = delete;
    UdpSender operator=(UdpSender&& other) = delete;

    ~UdpSender()
    {
        close(socketdfd_);
    }


private:
    int socketdfd_;
    sockaddr_in destAddr_;
};

