cmake_minimum_required(VERSION 3.30)

set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
set(VCPKG_HOST_TRIPLET x64-mingw-dynamic)

project(AOC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(CMake_dependencies.cmake)  ### Loads and declares the requires external libraries using both vcpkg and CMake's FetchContent.

add_subdirectory(src)
add_subdirectory(test)

add_executable(AOC)
target_sources(AOC PUBLIC src/main.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AOC PRIVATE AOC_2015 AOC_2016 AOC_2017 AOC_2018 AOC_2019)