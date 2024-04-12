set(TEST_SOURCES
    test_streamlet.cpp
)

add_executable(test_streamlet ${TEST_SOURCES})

target_link_libraries(test_streamlet PRIVATE streamlet)

target_include_directories(test_streamlet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(CapnProto REQUIRED)

target_link_libraries(test_streamlet PRIVATE CapnProto::capnp)