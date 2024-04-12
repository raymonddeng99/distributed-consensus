cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(ALGORAND_SRC
	configurator.h
    server.h
    server.cpp
)

add_library(algorand ${ALGORAND_SRC})

target_include_directories(algorand PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})