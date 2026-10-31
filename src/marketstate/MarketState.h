#pragma once

#include "../stockdirectory/StockDirectory.h"
#include "../tradebook/Tradebook.h"
#include "Market.h"

#include "../parser/Messages.h"

class MarketState {

public:
	MarketState() = default;
	MarketState(const MarketState& other) = delete;
	MarketState& operator=(const MarketState& other) = delete;
	MarketState(MarketState&& other) = delete;
	MarketState& operator=(MarketState&& other) = delete;
	~MarketState() = default;

	void onSystemEvent(const SystemEventMessage& message);

	void onStockDirectory(const StockDirectoryMessage& message);
	void onStockTradingAction(const StockTradingActionMessage& message);
	void onRegSHORestriction(const RegSHORestrictionMessage& message);
	void onMarketParticipantPosition(const MarketParticipantPositionMessage& message);
	void onMWCBDeclineLevel(const MWCBDeclineLevelMessage& message);
	void onMWCBStatus(const MWCBStatusMessage& message);
	void onIPOQuotingPeriodUpdate(const IPOQuotingPeriodUpdateMessage& message);
	void onLULDAuctionCollar(const LULDAuctionCollarMessage& message);
	void onOperationalHalt(const OperationalHaltMessage& message);

	void onAddOrder(const AddOrderMessage& message);
	void onAddOrderMPID(const AddOrderMPIDMessage& message);

	void onOrderExecuted(const OrderExecutedMessage& message);
	void onOrderExecutedWithPrice(const OrderExecutedWithPriceMessage& message);
	void onOrderCancelled(const OrderCancelMessage& message);
	void onOrderDelete(const OrderDeleteMessage& message);
	void onOrderReplace(const OrderReplaceMessage& message);

	void onTrade(const TradeMessage& message);
	void onCrossTrade(const CrossTradeMessage& message);
	void onBrokenTrade(const BrokenTradeMessage& message);

	void onNOII(const NOIIMessage& message);
	void onRetailPriceImprovement(const RetailPriceImprovementMessage& message);
	void onDirectListingCapitalRaise(const DirectListingCapitalRaiseMessage& message);

	void noteMessage(itch::Timestamp ts) noexcept { ++messages_; lastTimestamp_ = ts; }
	void badMessage() noexcept { ++badMessages_; }
	[[nodiscard]] std::uint64_t   messageCount()  const noexcept { return messages_; }
	[[nodiscard]] itch::Timestamp lastTimestamp() const noexcept { return lastTimestamp_; }

	[[nodiscard]] const StockDirectory& stockDirectory() const noexcept { return stockDirectory_; }
	[[nodiscard]] const Market& market() const noexcept { return market_; }
	[[nodiscard]] const Tradebook& tradeBook() const noexcept { return tradeBook_; }

private:
	StockDirectory stockDirectory_ {};
	Tradebook tradeBook_ {};
	Market market_ {};
	std::uint64_t messages_ {};
	std::size_t badMessages_ {};
	itch::Timestamp lastTimestamp_ {};
};



