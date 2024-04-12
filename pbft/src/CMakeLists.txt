cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(PBFT_SRC
	configurator.h
    server.h
    server.cpp
)

add_library(pbft ${PBFT_SRC})

target_include_directories(pbft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})