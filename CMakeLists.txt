cmake_minimum_required(VERSION 3.31.6)
project(TradingSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(TradingSystem src/main.cpp
        src/parser/Decode.cpp
        src/parser/Decode.h
        src/parser/Messages.cpp
        src/parser/Messages.h
        libs/nlohmann/json.hpp
        src/parser/Decode.h
        src/orderbook/Orderbook.cpp
        src/orderbook/Orderbook.h
        src/Constants.h
        src/orderbook/LevelInfo.h
        src/orderbook/LevelInfo.h
        src/Usings.h
        src/tradebook/Tradebook.cpp
        src/directory/StockDirectory.cpp
        src/directory/StockDirectory.h
        src/marketstate/MarketState.cpp
        src/marketstate/MarketState.h
        src/tradebook/Trade.h
        src/parser/Dispatch.cpp
        src/parser/Dispatch.h
)
