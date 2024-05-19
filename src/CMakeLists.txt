cmake_minimum_required(VERSION 3.25)
project (AOC)

set(CMAKE_CXX_STANDARD 23)



file(GLOB LC_PROBLEMS_CPPS_2015 ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2015/*.cpp)
file(GLOB LC_PROBLEMS_CPPS_2016 ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2016/*.cpp)

file(GLOB HASH_LIBRARY_FILES ${CMAKE_SOURCE_DIR}/src/external/hash-library/*.cpp ${CMAKE_SOURCE_DIR}/src/external/hash-library/*.h)



add_library(LC_Commons LC_commons.cpp)
add_library(LC_Problems_2015 ${LC_PROBLEMS_CPPS_2015} ${CMAKE_SOURCE_DIR}/include/AOC_2015.h)
add_library(LC_Problems_2016 ${LC_PROBLEMS_CPPS_2016} ${CMAKE_SOURCE_DIR}/include/AOC_2016.h)
add_library(hash-library ${HASH_LIBRARY_FILES})

target_include_directories(LC_Commons PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(LC_Problems_2015 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2015)
target_include_directories(LC_Problems_2016 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2016)
target_include_directories(hash-library PUBLIC ${CMAKE_SOURCE_DIR}/src/external/hash-library)

target_link_libraries(LC_Commons PUBLIC  fmt::fmt glaze::glaze flux::flux ctre::ctre hash-library xxHash::xxhash bitlib::bitlib)
target_link_libraries(LC_Problems_2015 PUBLIC LC_Commons)
target_link_libraries(LC_Problems_2016 PUBLIC LC_Commons)