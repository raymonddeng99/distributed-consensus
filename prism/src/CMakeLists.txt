cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(PRISM_SRC
	configurator.h
    server.h
    server.cpp
)

add_library(prism ${PRISM_SRC})

target_include_directories(prism PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})