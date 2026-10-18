cmake_minimum_required(VERSION 4.3)
project(TradingSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(TradingSystem src/main.cpp
        src/parser/MarketDataParser.cpp
        src/parser/MarketDataParser.h
        src/parser/Messages.cpp
        src/parser/Messages.h
        libs/nlohmann/json.hpp
        src/parser/MarketDataParser.h
        src/orderbook/Orderbook.cpp
        src/orderbook/Orderbook.h
        src/Constants.h
        src/orderbook/LevelInfo.h
        src/orderbook/LevelInfo.h
)
