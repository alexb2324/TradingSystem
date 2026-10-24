#pragma once

#include <unordered_map>
#include <array>

#include "../orderbook/Order.h"
#include "../orderbook/Orderbook.h"

#include "../Usings.h"
#include "../Constants.h"

class Market {
public:
    Market() { orderTable_.reserve(10'000'000u); }
    Market(const Market& other) = delete;
    Market& operator=(const Market& other) = delete;
    Market(Market&& other) = delete;
    Market& operator=(Market&& other) = delete;
    ~Market() = default;

    // Add new order to orderbook
    void addOrder(orderbook::OrderId orderId, const Order& order);

    const Orderbook& orderbook(const itch::StockLocate& stockLocate) const { return books_[stockLocate]; };

private:
    std::unordered_map<orderbook::OrderId, Order> orderTable_;
    std::array<Orderbook, Constants::kMaxStockDirectorySize> books_;

};
