cmake_minimum_required(VERSION 3.19)
project(TP5_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TP5_Client
        cmdparser.cpp
        cmdparser.h
        HTTPClient.cpp
        HTTPClient.h HTTPClientMain.cpp)

target_link_libraries(TP5_Client curl)