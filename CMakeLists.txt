cmake_minimum_required(VERSION 3.30...4.2)

project(AOC-InCom
    HOMEPAGE_URL "https://github.com/InCom-0/Advent_of_Code"
    DESCRIPTION
    "Advent of Code 2015 2016 2017 2018 2019 2024 2025"
    LANGUAGES CXX
)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 23)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(AOC-InCom_BUILD_SHARED_LIB "Build a shared version of AOC-InCom" ${BUILD_SHARED_LIBS})
option(AOC-InCom-lib_STAGE_OUTPUTS "Stage build output artifacts into proper directory structure" ON)

option(AOC-InCom-lib_BUILD_TESTS "Build test executables for incplot-lib" ${PROJECT_IS_TOP_LEVEL})

include(cmake/stdlibQuery.cmake)


#####################################################################
### Platform specifics + hacks ###
#####################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/utf-8)
    add_compile_definitions(NOMINMAX)
endif()


#####################################################################
### Staging ###
#####################################################################
include(cmake/StageOutputs.cmake)
if(AOC-InCom_STAGE_OUTPUTS)
    stageOutputDirsFor(AOC-InCom)
endif()


######################################################################
### Target specification ###
######################################################################
include(CMake_dependencies.cmake) ### Loads and declares the required external libraries using CPM or CMake's FetchContent.

add_subdirectory(src)

add_executable(AOC-InCom)
target_sources(AOC-InCom PUBLIC src/main.cpp)

target_compile_features(AOC-InCom PRIVATE cxx_std_23)
target_link_libraries(AOC-InCom PRIVATE AOC_2015 AOC_2016 AOC_2017 AOC_2018 AOC_2019 AOC_2024 AOC_2025)
if(USING_LIBSTDCXX)
    target_link_libraries(AOC-InCom PRIVATE "-lstdc++exp")
endif()

if(AOC-InCom-lib_BUILD_TESTS)
    add_subdirectory(test)
endif()
