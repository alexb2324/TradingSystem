#pragma once

#include "types/MarketTypes.h"

struct Order
{
    Order(market::Quantity quantity, market::Price price, market::Side side, itch::StockLocate stockLocate)
        : quantity { quantity }
        , price { price }
        , stockLocate { stockLocate }
        , side { side }
    { }

    market::Quantity quantity;
    market::Price price;
    itch::StockLocate stockLocate;
    market::Side side;
};