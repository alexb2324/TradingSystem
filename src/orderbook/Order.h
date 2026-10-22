#pragma once

#include <format>
#include <stdexcept>

#include "../Usings.h"

class Order
{
public:
    Order(orderbook::OrderId orderId, orderbook::Side side, orderbook::Price price, orderbook::Quantity quantity)
        : orderId_ { orderId }
        , side_ { side }
        , price_ { price }
        , initialQuantity_ { quantity }
        , remainingQuantity_ { quantity }
    { }

    [[nodiscard]] orderbook::OrderId getOrderId() const { return orderId_; }
    [[nodiscard]] orderbook::Side getSide() const { return side_; }
    [[nodiscard]] orderbook::Price getPrice() const { return price_; }
    [[nodiscard]] orderbook::Quantity getInitialQuantity() const { return initialQuantity_; }
    [[nodiscard]] orderbook::Quantity getRemainingQuantity() const { return remainingQuantity_; }
    [[nodiscard]] orderbook::Quantity getFilledQuantity() const { return getInitialQuantity() - getRemainingQuantity(); }

    [[nodiscard]] bool IsFilled() const { return getRemainingQuantity() == 0; }

    void Fill(const orderbook::Quantity quantity)
    {
        if (quantity > getRemainingQuantity())
        {
            throw std::logic_error(std::format("Order ({}) cannot be filled for more than its remaining quantity.", getOrderId()));
        }
        remainingQuantity_ -= quantity;
    }

private:
    orderbook::OrderId orderId_;
    orderbook::Side side_;
    orderbook::Price price_;
    orderbook::Quantity initialQuantity_;
    orderbook::Quantity remainingQuantity_;
};