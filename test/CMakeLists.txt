cmake_minimum_required(VERSION 3.25)
project (AOC)

set(CMAKE_CXX_STANDARD 23)


file(GLOB AOC_TESTS_CPPS ${CMAKE_CURRENT_SOURCE_DIR}/problems/*.cpp)

foreach(test_file IN LISTS AOC_TESTS_CPPS)
    message(STATUS ${test_file})
endforeach()



enable_testing()

add_executable(AOC_Tests ${AOC_TESTS_CPPS})

target_include_directories(AOC_Tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(AOC_Tests AOC_Commons AOC_2015 AOC_2016 gtest_main gmock_main)