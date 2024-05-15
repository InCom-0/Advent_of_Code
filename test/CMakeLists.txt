cmake_minimum_required(VERSION 3.25)
project (AOC)

set(CMAKE_CXX_STANDARD 23)


file(GLOB LC_PROBLEMS_TESTS_CPPS ${CMAKE_CURRENT_SOURCE_DIR}/problems/*.cpp)

foreach(test_file IN LISTS LC_PROBLEMS_TESTS_CPPS)
    message(STATUS ${test_file})
endforeach()




enable_testing()

add_executable(LC_Tests ${LC_PROBLEMS_TESTS_CPPS} ${LC_INCLUDE})

target_include_directories(LC_Tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(LC_Tests LC_Commons LC_Problems gtest_main gmock_main)