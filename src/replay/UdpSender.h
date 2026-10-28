#pragma once

#include <string>

#include "MoldUDP64Encoder.h"


class UdpSender
{
public:
    UdpSender() = default;
    std::size_t sendPacket(itch::Bytes packet);

private:
    std::string address_ {};
    std::uint16_t port_ {8888};
};
