set(TEST_SOURCES
    test_honeybadgerbft.cpp
)

add_executable(test_honeybadgerbft ${TEST_SOURCES})

target_link_libraries(test_honeybadgerbft PRIVATE honeybadgerbft)

target_include_directories(test_honeybadgerbft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(CapnProto REQUIRED)

target_link_libraries(test_honeybadgerbft PRIVATE CapnProto::capnp)