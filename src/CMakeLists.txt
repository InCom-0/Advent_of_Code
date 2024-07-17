project(AOC)

add_library(AOC_Commons AOC_commons.cpp ${CMAKE_SOURCE_DIR}/include/AOC_commons.h)
target_include_directories(AOC_Commons PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(external)
add_subdirectory(problems)

target_link_libraries(AOC_Commons PUBLIC
    fmt::fmt
    glaze::glaze
    flux::flux
    ctre::ctre
    hash-library
    xxHash::xxhash
    bitlib::bitlib
    unordered_dense::unordered_dense
    more_concepts::more_concepts
    foonathan_memory)

target_link_options(AOC_Commons PUBLIC -static-libgcc -static-libstdc++ "LINKER:-Bstatic,--whole-archive" -lwinpthread "LINKER:-Bdynamic,--no-whole-archive")

target_link_libraries(AOC_2015 PUBLIC AOC_Commons)
target_link_libraries(AOC_2016 PUBLIC AOC_Commons)
target_link_libraries(AOC_2017 PUBLIC AOC_Commons)
target_link_libraries(AOC_2018 PUBLIC AOC_Commons)

set_target_properties(AOC_2015 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2016 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2017 PROPERTIES UNITY_BUILD ON)
set_target_properties(AOC_2018 PROPERTIES UNITY_BUILD ON)