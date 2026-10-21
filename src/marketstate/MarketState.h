#pragma once

#include "../directory/StockDirectory.h"
#include "../tradebook/TradeBook.h"

class MarketState {

public:
	MarketState() = default;
	MarketState(const MarketState& other) = delete;
	MarketState& operator=(const MarketState& other) = delete;
	MarketState(MarketState&& other) = delete;
	MarketState& operator=(MarketState&& other) = delete;
	~MarketState() = default;

	void onStockDirectory();
	void onAddOrder();
	void onOrderExecuted();

private:

	StockDirectory stockDirectory_;
	// MarketDataParser marketDataParser_;
	TradeBook tradeBook_;
};



