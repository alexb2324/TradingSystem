#pragma once

#include <map>
#include <string>

#include "../Usings.h"
#include "Order.h"

class Orderbook
{

public:

    Orderbook();
    Orderbook(const Orderbook& other) = delete;
    void operator=(const Orderbook& other) = delete;
    Orderbook(Orderbook&& other) = delete;
    void operator=(Orderbook&& other) = delete;
    ~Orderbook();

    [[nodiscard]] std::size_t size() const noexcept { return bids_.size() + asks_.size(); }

private:

    std::map<orderbook::Price, Order, std::greater<>> bids_;
    std::map<orderbook::Price, Order, std::less<>> asks_;
    std::string symbol_;

};