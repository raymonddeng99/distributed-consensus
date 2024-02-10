set(TEST_SOURCES
    test_paxos.cpp
)

# Create an executable from the sources
add_executable(test_paxos ${TEST_SOURCES})

# Link the executable with the Paxos library
target_link_libraries(test_paxos PRIVATE paxos)

# Include the /src directory as an include directory for headers
target_include_directories(test_paxos PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Find Cap'n Proto package
find_package(CapnProto REQUIRED)

# Link against Cap'n Proto library
target_link_libraries(test_paxos PRIVATE CapnProto::capnp)