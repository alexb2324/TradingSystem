#pragma once

#include "types/ItchTypes.h"
#include "types/MarketTypes.h"

enum class TradeType : char
{
	OrderExecuted			= 'E',
	OrderExecutedWithPrice	= 'C',
	Trade					= 'P',
	CrossTrade				= 'Q'
};

struct Trade
{
	itch::Timestamp		timestamp {};
	itch::MatchNumber	matchNumber {};
	itch::Shares		shares {};
	market::Price		price {};
	itch::StockLocate	stockLocate {};
	TradeType			tradeType {};
	char				side {};
	char				crossType {};
	bool				broken = false;
};
