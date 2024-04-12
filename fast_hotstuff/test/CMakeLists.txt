set(TEST_SOURCES
    test_crypto.cpp
)

# Create an executable from the sources
add_executable(test_crypto ${TEST_SOURCES})

# Link the executable with the HotStuff library
target_link_libraries(test_crypto PRIVATE hotstuff)

# Include the /src directory as an include directory for headers
target_include_directories(test_crypto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Find Cap'n Proto package
find_package(CapnProto REQUIRED)

# Link against Cap'n Proto library
target_link_libraries(test_crypto PRIVATE CapnProto::capnp)