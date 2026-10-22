#pragma once

#include "../Usings.h"
#include "../marketstate/MarketState.h"

[[nodiscard]] std::size_t parseData(itch::Bytes data, MarketState& handler);

void dispatch(itch::Bytes message, MarketState& handler);

