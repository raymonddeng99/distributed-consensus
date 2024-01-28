set(TEST_SOURCES
    test_raft.cpp
)

# Create an executable from the sources
add_executable(test_raft ${TEST_SOURCES})

# Link the executable with the raft library
target_link_libraries(test_raft PRIVATE raft)

# Include the /src directory as an include directory for headers
target_include_directories(test_raft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Find Cap'n Proto package
find_package(CapnProto REQUIRED)

# Link against Cap'n Proto library
target_link_libraries(test_raft PRIVATE CapnProto::capnp)