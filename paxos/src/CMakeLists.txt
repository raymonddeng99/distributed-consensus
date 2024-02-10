cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

# Add the source files for the Paxos library
set(PAXOS_SRC
	configurator.h
    server.h
    server.cpp
)

# Find Cap'n Proto find_package
find_package(CapnProto REQUIRED)

# Generate C++ code from Cap'n Proto schema
capnp_generate_cpp(CAPNP_SOURCES CAPNP_HEADERS Paxos.capnp)

# Create the Paxos library
add_library(paxos ${CHORD_SRC} ${CAPNP_SOURCES})

# Link against Cap'n Proto library
target_link_libraries(paxos PRIVATE CapnProto::capnp-rpc)

# Include the headers from the Paxos library
target_include_directories(paxos PRIVATE ${CMAKE_CURRENT_BINARY_DIR})%  