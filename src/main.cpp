#include <filesystem>
#include <iostream>

#include <AOC_2015.h>
#include <AOC_2016.h>
#include <AOC_2017.h>
#include <AOC_2018.h>
#include <incom_commons.h>

#define DATAFOLDER      "../data"
#define DATAFOLDER_2018 "../data/AOC_2018"


/*
Compile and run 'AOC_Tests_ALL' (Google tests) target to execute solutions with the right input data, verify
functionality etc.

Note that this main is used while developing the solutions.
*/

int main() {

    auto pth_1 = std::filesystem::path(DATAFOLDER_2018) / "2018_20_1.txt";

    std::cout << AOC2018::day20_1(pth_1.generic_string()).second << '\n';
    

    return 1;
}