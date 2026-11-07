#pragma once

#include <fstream>
#include "Decode.h"
#include "Messages.h"
#include "../Constants.h"

#include "../marketstate/MarketState.h"

#include <cstdio>


inline void dispatch(itch::Bytes message, MarketState& marketStateHandler)
{
	if (!Constants::kItchMessageLengthsMap.contains(static_cast<char>(message[0])))
	{
		marketStateHandler.badMessage();
	}

    if constexpr (requires (itch::Timestamp timestamp) { marketStateHandler.noteMessage(timestamp); })
    {
        marketStateHandler.noteMessage(convertBLEndian48(message.data() + 5));
    }

	switch (static_cast<MessageType>(message[0]))
    {
        case MessageType::SystemEventMessage:
            if constexpr (requires (const SystemEventMessage& msg) { marketStateHandler.onSystemEvent(msg); })
            {
                marketStateHandler.onSystemEvent(decodeSystemEventMessage(message));
            }
            break;
        case MessageType::StockDirectoryMessage:
	        if constexpr (requires (const StockDirectoryMessage& msg) { marketStateHandler.onStockDirectory(msg); })
            {
                marketStateHandler.onStockDirectory(decodeStockDirectoryMessage(message));
            }
	        break;
        case MessageType::StockTradingActionMessage:
            if constexpr (requires (const StockTradingActionMessage& msg) { marketStateHandler.onStockTradingAction(msg); })
            {
                marketStateHandler.onStockTradingAction(decodeStockTradingActionMessage(message));
            }
            break;
        case MessageType::RegSHORestrictionMessage:
	        if constexpr (requires (const RegSHORestrictionMessage& msg) { marketStateHandler.onRegSHORestriction(msg); })
	        {
	            marketStateHandler.onRegSHORestriction(decodeRegSHORestrictionMessage(message));
	        }
	        break;
	    case MessageType::MarketParticipantPositionMessage:
			if constexpr (requires (const MarketParticipantPositionMessage& msg) { marketStateHandler.onMarketParticipantPosition(msg); })
			{
				marketStateHandler.onMarketParticipantPosition(decodeMarketParticipantPositionMessage(message));
			}
	        break;
        case MessageType::MWCBDeclineLevelMessage:
	        if constexpr (requires (const MWCBDeclineLevelMessage& msg) { marketStateHandler.onMWCBDeclineLevel(msg); })
	        {
	            marketStateHandler.onMWCBDeclineLevel(decodeMWCBDeclineLevelMessage(message));
	        }
	        break;
        case MessageType::MWCBStatusMessage:
	        if constexpr (requires (const MWCBStatusMessage& msg) { marketStateHandler.onMWCBStatus(msg); })
	        {
	            marketStateHandler.onMWCBStatus(decodeMWCBStatusMessage(message));
	        }
	        break;
        case MessageType::IPOQuotingPeriodUpdateMessage:
	        if constexpr (requires (const IPOQuotingPeriodUpdateMessage& msg) { marketStateHandler.onIPOQuotingPeriodUpdate(msg); })
	        {
	            marketStateHandler.onIPOQuotingPeriodUpdate(decodeIPOQuotingPeriodUpdateMessage(message));
	        }
	        break;
        case MessageType::LULDAuctionCollarMessage:
	        if constexpr (requires (const LULDAuctionCollarMessage& msg) { marketStateHandler.onLULDAuctionCollar(msg); })
	        {
	            marketStateHandler.onLULDAuctionCollar(decodeLULDAuctionCollarMessage(message));
	        }
	        break;
        case MessageType::OperationalHaltMessage:
			if constexpr (requires (const OperationalHaltMessage& msg) { marketStateHandler.onOperationalHalt(msg); })
			{
				marketStateHandler.onOperationalHalt(decodeOperationalHaltMessage(message));
			}
			break;
        case MessageType::AddOrderMessage:
	        if constexpr (requires (const AddOrderMessage& msg) { marketStateHandler.onAddOrder(msg); })
            {
                marketStateHandler.onAddOrder(decodeAddOrderMessage(message));
            }
	        break;
        case MessageType::AddOrderMPIDMessage:
	        if constexpr (requires (const AddOrderMPIDMessage& msg) { marketStateHandler.onAddOrderMPID(msg); })
            {
                marketStateHandler.onAddOrderMPID(decodeAddOrderMPIDMessage(message));
            }
	        break;
        case MessageType::OrderExecutedMessage:
	        if constexpr (requires (const OrderExecutedMessage& msg) { marketStateHandler.onOrderExecuted(msg); })
            {
                marketStateHandler.onOrderExecuted(decodeOrderExecutedMessage(message));
            }
	        break;
        case MessageType::OrderExecutedWithPriceMessage:
	        if constexpr (requires (const OrderExecutedWithPriceMessage& msg) { marketStateHandler.onOrderExecutedWithPrice(msg); })
            {
                marketStateHandler.onOrderExecutedWithPrice(decodeOrderExecutedWithPriceMessage(message));
            }
	        break;
        case MessageType::OrderCancelMessage:
	        if constexpr (requires (const OrderCancelMessage& msg) { marketStateHandler.onOrderCancelled(msg); })
            {
                marketStateHandler.onOrderCancelled(decodeOrderCancelMessage(message));
            }
	        break;
        case MessageType::OrderDeleteMessage:
	        if constexpr (requires (const OrderDeleteMessage& msg) { marketStateHandler.onOrderDelete(msg); })
            {
                marketStateHandler.onOrderDelete(decodeOrderDeleteMessage(message));
            }
	        break;
        case MessageType::OrderReplaceMessage:
	        if constexpr (requires (const OrderReplaceMessage& msg) { marketStateHandler.onOrderReplace(msg); })
            {
                marketStateHandler.onOrderReplace(decodeOrderReplaceMessage(message));
            }
	        break;
        case MessageType::TradeMessage:
	        if constexpr (requires (const TradeMessage& msg) { marketStateHandler.onTrade(msg); })
            {
                marketStateHandler.onTrade(decodeTradeMessage(message));
            }
	        break;
        case MessageType::CrossTradeMessage:
	        if constexpr (requires (const CrossTradeMessage& msg) { marketStateHandler.onCrossTrade(msg); })
            {
                marketStateHandler.onCrossTrade(decodeCrossTradeMessage(message));
            }
	        break;
        case MessageType::BrokenTradeMessage:
	        if constexpr (requires (const BrokenTradeMessage& msg) { marketStateHandler.onBrokenTrade(msg); })
            {
                marketStateHandler.onBrokenTrade(decodeBrokenTradeMessage(message));
            }
	        break;
        case MessageType::NOIIMessage:
			if constexpr (requires (const NOIIMessage& msg) { marketStateHandler.onNOII(msg); })
			{
				marketStateHandler.onNOII(decodeNOIIMessage(message));
			}
			break;
        case MessageType::RetailPriceImprovementMessage:
			if constexpr (requires (const RetailPriceImprovementMessage& msg) { marketStateHandler.onRetailPriceImprovement(msg); })
			{
				marketStateHandler.onRetailPriceImprovement(decodeRetailPriceImprovementMessage(message));
			}
			break;
        case MessageType::DirectListingCapitalRaiseMessage:
			if constexpr (requires (const DirectListingCapitalRaiseMessage& msg) { marketStateHandler.onDirectListingCapitalRaise(msg); })
			{
				marketStateHandler.onDirectListingCapitalRaise(decodeDirectListingCapitalRaiseMessage(message));
			}
			break;
        default:
            throw std::runtime_error("Unknown message type");
    }
}
