cmake_minimum_required(VERSION 3.30...4.0)

# set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
# set(VCPKG_HOST_TRIPLET x64-mingw-dynamic)

project(AOC)

set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(CMake_dependencies.cmake)  ### Loads and declares the requires external libraries using CPM.CMake

add_subdirectory(src)
add_subdirectory(test)

add_executable(AOC)
target_sources(AOC PUBLIC src/main.cpp)

target_link_libraries(AOC PRIVATE AOC_2015 AOC_2016 AOC_2017 AOC_2018 AOC_2019 AOC_2024)