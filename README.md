# Advent of Code 2015 2016 2017 2018 2019 2024 #

My solutions to Advent of Code (<https://adventofcode.com>) puzzles.

## General information ##

* This code was developed as a learning excercise for myself in C++ itself, STL, its tooling and build systems.
* The code is written in (I dare say) very modern C++ particularly utilizing the more 'functional programming' features.
* That being said, the code is not the 'pinnacle of perfection' in terms of performance or any other metric nor is it 'industrial grade' and it doesn't attempt to be.
* That being said, an effort was expended to obtain reasonably performant as well as a reasonably structured code as opposed to writing 'working' solution as quickly as possible.
* All the solutions were developed by myself only. Nothing is copied from anywhere including from the output of LLMs.
* Naturally though, inspiration was drawn from various sources mostly in terms of programmatic idioms, broad problem solving strategies and various technicalities of the language and STL.

## Technical information ##

* Written in C++ and requires C++23 capable compiler (eg. GCC 14.2+)
* Uses CMake v3.30+. Not recommended to build the solutions without CMake as its linking various external libraries.
* Uses vcpkg as well as CMake's FetchContent. Therefore, one has to pass the location of 'vcpkg.cmake' to CMake in CMAKE_TOOLCHAIN_FILE unless vcpkg is automatically integrated by your IDE.
* Intended to be built on Windows in MSYS2 MinGW environment. Might not build 'as-is' on other platforms. However, minor appropriate CMakeLists adjustment should be enough to compile on Linux.
* Uses gtest and one can verify the correctness of the provided 'puzzle solutions' by running one of the respective gtest targets.

## External libraries used ##

* [flux](https://github.com/tcbrindle/flux) --- Used heavily
* [ctre](https://github.com/hanickadot/compile-time-regular-expressions) --- Used heavily
* [BitLib](https://github.com/bkille/bitlib)
* [more_concepts](https://github.com/MiSo1289/more_concepts)
* [xxHash](https://github.com/Cyan4973/xxHash)
* [glaze](https://github.com/stephenberry/glaze)
* [unordered_dense](https://github.com/martinus/unordered_dense)
