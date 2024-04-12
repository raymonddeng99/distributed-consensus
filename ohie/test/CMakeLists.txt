set(TEST_SOURCES
    test_ohie.cpp
)

add_executable(test_ohie ${TEST_SOURCES})

target_link_libraries(test_ohie PRIVATE ohie)

target_include_directories(test_ohie PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(CapnProto REQUIRED)

target_link_libraries(test_ohie PRIVATE CapnProto::capnp)