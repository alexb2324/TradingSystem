#include <fstream>

#include "Dispatch.h"
#include "Decode.h"
#include "Messages.h"

#include "../Constants.h"
#include "../Usings.h"

std::size_t parseData(const itch::Bytes data, MarketState& handler)
{
    std::size_t readOffset {};

    while (readOffset + Constants::kItchMessageOffset <= data.size())
    {
        const auto msgLength = convertBLEndian<itch::MessageLength>(data.data() + readOffset);

        if (readOffset + Constants::kItchMessageOffset + msgLength > data.size()) { break; }
        if (msgLength == 0) { break; }

        dispatch(data.subspan(readOffset + Constants::kItchMessageOffset, msgLength), handler);

        readOffset += Constants::kItchMessageOffset + msgLength;
    }

    return readOffset;
}

void dispatch(itch::Bytes message, MarketState& handler)
{
	switch (static_cast<MessageType>(message[0]))
    {
        case MessageType::SystemEventMessage:
        {
            break;
        }
        case MessageType::StockDirectoryMessage:
        {
            handler.onStockDirectory(decodeStockDirectoryMessage(message));
            break;
        }
        case MessageType::StockTradingActionMessage:
        {
            break;
        }
        case MessageType::RegSHORestrictionMessage:
        {
            break;
        }
        case MessageType::MarketParticipantPositionMessage:
        {
            break;
        }
        case MessageType::MWCBDeclineLevelMessage:
        {
            break;
        }
        case MessageType::MWCBStatusMessage:
        {
            break;
        }
        case MessageType::IPOQuotingPeriodUpdateMessage:
        {
            break;
        }
        case MessageType::LULDAuctionCollarMessage:
        {
            break;
        }
        case MessageType::OperationalHaltMessage:
        {
            break;
        }
        case MessageType::AddOrderMessage:
        {
            handler.onAddOrder(decodeAddOrderMessage(message));
            break;
        }
        case MessageType::AddOrderMPIDMessage:
        {
            handler.onAddOrderMPID(decodeAddOrderMPIDMessage(message));
            break;
        }
        case MessageType::OrderExecutedMessage:
        {
            handler.onOrderExecuted(decodeOrderExecutedMessage(message));
            break;
        }
        case MessageType::OrderExecutedWithPriceMessage:
        {
            handler.onOrderExecutedWithPrice(decodeOrderExecutedWithPriceMessage(message));
            break;
        }
        case MessageType::OrderCancelMessage:
        {
            handler.onOrderCancelled(decodeOrderCancelMessage(message));
            break;
        }
        case MessageType::OrderDeleteMessage:
        {
            handler.onOrderCancelled(decodeOrderCancelMessage(message));
            break;
        }
        case MessageType::OrderReplaceMessage:
        {
            handler.onOrderCancelled(decodeOrderCancelMessage(message));
            break;
        }
        case MessageType::TradeMessage:
        {
            handler.onTrade(decodeTradeMessage(message));
            break;
        }
        case MessageType::CrossTradeMessage:
        {
            handler.onCrossTrade(decodeCrossTradeMessage(message));
            break;
        }
        case MessageType::BrokenTradeMessage:
        {
            handler.onBrokenTrade(decodeBrokenTradeMessage(message));
            break;
        }
        case MessageType::NOIIMessage:
        {
            break;
        }
        case MessageType::RetailPriceImprovementMessage:
        {
            break;
        }
        case MessageType::DirectListingCapitalRaiseMessage:
        {
            break;
        }
        default:
            throw std::runtime_error("Unknown message type");
    }
}
