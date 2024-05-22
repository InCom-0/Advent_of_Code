cmake_minimum_required(VERSION 3.25)
project (AOC)

set(CMAKE_CXX_STANDARD 23)


file(GLOB AOC_2015_CPPS ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2015/*.cpp)
file(GLOB AOC_2016_CPPS ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2016/*.cpp)

file(GLOB HASH_LIBRARY_FILES ${CMAKE_SOURCE_DIR}/src/external/hash-library/*.cpp ${CMAKE_SOURCE_DIR}/src/external/hash-library/*.h)


add_library(AOC_Commons AOC_commons.cpp ${CMAKE_SOURCE_DIR}/include/AOC_commons.h)
add_library(AOC_2015 ${AOC_2015_CPPS} ${CMAKE_SOURCE_DIR}/include/AOC_2015.h)
add_library(AOC_2016 ${AOC_2016_CPPS} ${CMAKE_SOURCE_DIR}/include/AOC_2016.h)
add_library(hash-library ${HASH_LIBRARY_FILES})

target_include_directories(AOC_Commons PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(AOC_2015 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2015)
target_include_directories(AOC_2016 PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/problems/AOC_2016)
target_include_directories(hash-library PUBLIC ${CMAKE_SOURCE_DIR}/src/external/hash-library)

target_link_libraries(AOC_Commons PUBLIC  fmt::fmt glaze::glaze flux::flux ctre::ctre hash-library xxHash::xxhash bitlib::bitlib)
target_link_libraries(AOC_2015 PUBLIC AOC_Commons)
target_link_libraries(AOC_2016 PUBLIC AOC_Commons)