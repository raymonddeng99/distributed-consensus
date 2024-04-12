cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(OHIE_SRC
	configurator.h
    server.h
    server.cpp
)

add_library(ohie ${OHIE_SRC})

target_include_directories(ohie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})