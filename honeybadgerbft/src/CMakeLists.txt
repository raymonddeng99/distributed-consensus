cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(HONEYBADGERBFT_SRC
	configurator.h
    server.h
    server.cpp
)

add_library(honeybadgerbft ${HONEYBADGERBFT_SRC})

target_include_directories(honeybadgerbft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})