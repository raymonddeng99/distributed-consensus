set(TEST_SOURCES
    test_algorand.cpp
)

add_executable(test_algorand ${TEST_SOURCES})

target_link_libraries(test_algorand PRIVATE algorand)

target_include_directories(test_algorand PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(CapnProto REQUIRED)

target_link_libraries(test_algorand PRIVATE CapnProto::capnp)