#pragma once

#include "../Usings.h"

struct Order
{
    Order(orderbook::Side side, orderbook::Price price, orderbook::Quantity quantity)
        : quantity { quantity }
        , side { side }
        , price { price }
    { }

    orderbook::Quantity quantity;
    orderbook::Side side;
    orderbook::Price price;
};