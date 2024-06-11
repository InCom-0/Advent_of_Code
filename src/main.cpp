#include <iostream>
#include <queue>
#include <filesystem>

#include <AOC_2015.h>
#include <AOC_2016.h>
#include <AOC_2017.h>
#include <AOC_commons.h>

#define DATAFOLDER "../data"
#define DATAFOLDER_2017 "../data/AOC_2017"


/*
Compile and run 'AOC_Tests_ALL' (Google tests) target to execute solutions with the right input data, verify functionality etc.

Note that this main is used while developing the solutions.
*/

int main()
{
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_16_1.txt";

    std::cout << AOC2017::day16_2(pth.generic_string()) << '\n';

    return 1;
}
