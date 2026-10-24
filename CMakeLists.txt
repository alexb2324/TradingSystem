cmake_minimum_required(VERSION 3.31.6)
project(TradingSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(TradingSystem src/main.cpp
        libs/nlohmann/json.hpp
        src/parser/Decode.cpp
        src/parser/Decode.h
        src/parser/Dispatch.cpp
        src/parser/Dispatch.h
        src/parser/Messages.cpp
        src/parser/Messages.h
        src/orderbook/Orderbook.cpp
        src/orderbook/Orderbook.h
        src/tradebook/Trade.h
        src/tradebook/Tradebook.cpp
        src/tradebook/Tradebook.h
        src/stockdirectory/StockDirectory.cpp
        src/stockdirectory/StockDirectory.h
        src/marketstate/MarketState.cpp
        src/marketstate/MarketState.h
        src/Constants.h
        src/Usings.h
        src/datasource/filesource/FileSource.cpp
        src/datasource/filesource/FileSource.h
        src/orderbook/Order.cpp
        src/orderbook/Order.h
        src/orderbook/OrderbookLevel.cpp
        src/orderbook/OrderbookLevel.h
        src/marketstate/Market.cpp
        src/marketstate/Market.h
)
