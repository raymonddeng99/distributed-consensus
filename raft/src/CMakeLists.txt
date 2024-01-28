cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

# Add the source files for the raft library
set(RAFT_SRC
    server.h
    server.cpp
)

# Create the raft library
add_library(raft ${RAFT_SRC})

# Include the headers from the raft library
target_include_directories(raft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})