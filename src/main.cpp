#include <filesystem>
#include <iostream>

#include <AOC_2015.h>
#include <AOC_2016.h>
#include <AOC_2017.h>
#include <AOC_2018.h>
#include <AOC_commons.h>

#define DATAFOLDER      "../data"
#define DATAFOLDER_2018 "../data/AOC_2018"


/*
Compile and run 'AOC_Tests_ALL' (Google tests) target to execute solutions with the right input data, verify
functionality etc.

Note that this main is used while developing the solutions.
*/

int main() {

    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_3_1.txt";

    std::cout << AOC2018::day3_2(pth.generic_string()) << '\n';


    return 1;
}