set(TEST_SOURCES
    test_prism.cpp
)

add_executable(test_prism ${TEST_SOURCES})

target_link_libraries(test_prism PRIVATE raft)

target_include_directories(test_prism PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(CapnProto REQUIRED)

target_link_libraries(test_prism PRIVATE CapnProto::capnp)