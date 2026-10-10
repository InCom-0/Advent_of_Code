#include <filesystem>
#include <iostream>

#include <AOC_2015.h>
#include <AOC_2016.h>
#include <AOC_2017.h>
#include <AOC_2018.h>
#include <AOC_2019.h>
#include <AOC_2023.h>
#include <AOC_2024.h>
#include <AOC_2025.h>
#include <incom_commons.hpp>


/*
Compile and run 'AOC_Tests_ALL' (Google tests) target to execute solutions with the right input data, verify
functionality etc.

Note that this main is used while developing the solutions.
*/

int
main() {

    // auto pth_1    = std::filesystem::path(DATAFOLDER_2019) / "2019_25_1.txt";
    // auto pth_2024 = std::filesystem::path(DATAFOLDER_2024) / "2024_25_1.txt";
    auto pth_2023 = std::filesystem::path(DATAFOLDER_2023) / "2023_13_1.txt";

    std::cout << AOC2023::day13_2(pth_2023.generic_string()) << '\n';


    return 1;
}