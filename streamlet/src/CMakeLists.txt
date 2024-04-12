cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(STREAMLET_SRC
	configurator.h
    server.h
    server.cpp
)

add_library(streamlet ${STREAMLET_SRC})

target_include_directories(streamlet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})