cmake_minimum_required(VERSION 3.25)
project (AOC)

set(CMAKE_CXX_STANDARD 23)


file(GLOB AOC_TESTS_CPPS ${CMAKE_CURRENT_SOURCE_DIR}/problems/*.cpp)



enable_testing()

add_executable(AOC_Tests ${AOC_TESTS_CPPS})

target_include_directories(AOC_Tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(AOC_Tests PRIVATE AOC_Commons AOC_2015 AOC_2016 AOC_2017 gtest_main gmock_main)