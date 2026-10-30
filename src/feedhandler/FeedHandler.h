#pragma once

#include "UdpReceiver.h"

class FeedHandler 
{

public:

    FeedHandler(const std::string& address, const std::uint16_t port)
    :   receiver_ {address, port}
    { }

private:
    UdpReceiver receiver_;
};