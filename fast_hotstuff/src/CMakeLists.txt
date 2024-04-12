cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

# Add the source files for the HotStuff library
set(HOTSTUFF_SRC
	utilities.h
	configurator.h
	crypto.h
)

# Find Cap'n Proto find_package
find_package(CapnProto REQUIRED)

# Generate C++ code from Cap'n Proto schema
capnp_generate_cpp(CAPNP_SOURCES CAPNP_HEADERS HotStuff.capnp)

# Create the HotStuff library
add_library(hotstuff ${CHORD_SRC} ${CAPNP_SOURCES})

# Link against Cap'n Proto library
target_link_libraries(hotstuff PRIVATE CapnProto::capnp-rpc)

# Include the headers from the HotStuff library
target_include_directories(hotstuff PRIVATE ${CMAKE_CURRENT_BINARY_DIR})%  