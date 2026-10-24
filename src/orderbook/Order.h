#pragma once

#include "../Usings.h"

struct Order
{
    Order(orderbook::Quantity quantity, orderbook::Price price, orderbook::Side side, itch::StockLocate stockLocate)
        : quantity { quantity }
        , price { price }
        , stockLocate { stockLocate }
        , side { side }
    { }

    orderbook::Quantity quantity;
    orderbook::Price price;
    itch::StockLocate stockLocate;
    orderbook::Side side;
};