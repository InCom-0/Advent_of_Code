cmake_minimum_required(VERSION 3.27)

set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
set(VCPKG_HOST_TRIPLET x64-mingw-dynamic)

project(AOC)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    bitlib
    GIT_REPOSITORY https://github.com/bkille/bitlib.git
    GIT_TAG origin/master
)
FetchContent_MakeAvailable(bitlib)

FetchContent_Declare(
    flux
    GIT_REPOSITORY https://github.com/tcbrindle/flux.git
    GIT_TAG main
)
FetchContent_MakeAvailable(flux)

FetchContent_Declare(
    more_concepts
    GIT_REPOSITORY https://github.com/InCom-0/more_concepts.git
    GIT_TAG origin/master
)
FetchContent_MakeAvailable(more_concepts)

find_package(fmt CONFIG REQUIRED)
find_package(glaze REQUIRED)
find_package(ctre REQUIRED)
find_package(xxHash REQUIRED)
find_package(unordered_dense REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

add_executable(AOC)
target_sources(AOC PUBLIC src/main.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AOC PRIVATE AOC_2015 AOC_2016 AOC_2017 AOC_2018)