cmake_minimum_required(VERSION 3.19)
project(TP5_Server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TP5_Server
        HTTPServer.cpp
        HTTPServer.h main.cpp)

target_link_libraries(TP5_Server boost boost-asio)