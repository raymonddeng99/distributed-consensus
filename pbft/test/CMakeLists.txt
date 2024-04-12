set(TEST_SOURCES
    test_pbft.cpp
)

# Create an executable from the sources
add_executable(test_pbft ${TEST_SOURCES})

# Link the executable with the pbft library
target_link_libraries(test_pbft PRIVATE pbft)

# Include the /src directory as an include directory for headers
target_include_directories(test_pbft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Find Cap'n Proto package
find_package(CapnProto REQUIRED)

# Link against Cap'n Proto library
target_link_libraries(test_pbft PRIVATE CapnProto::capnp)