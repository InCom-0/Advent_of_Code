#include <algorithm>
#include <array>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;

size_t day25_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    std::vector<std::array<int, 5>> locks;
    std::vector<std::array<int, 5>> keys;

    size_t curLineID = 0;

    auto eva_oneInput = [&]() -> void {
        if (input.at(curLineID) == "#####") {
            locks.push_back({0, 0, 0, 0, 0});
            for (int line = 0; line < 5; ++line) {
                curLineID++;
                for (int itemInLine = 0; itemInLine < 5; ++itemInLine) {
                    locks.back()[itemInLine] += (input.at(curLineID).at(itemInLine) == '#');
                }
            }
        }
        else if (input.at(curLineID) == ".....") {
            keys.push_back({0, 0, 0, 0, 0});
            for (int line = 0; line < 5; ++line) {
                curLineID++;
                for (int itemInLine = 0; itemInLine < 5; ++itemInLine) {
                    keys.back()[itemInLine] += (input.at(curLineID).at(itemInLine) == '#');
                }
            }
        }
        else {
            assert((void("Incorrect allignment of eva_oneInput ... first line is not '######' nor '......' "), false));
        }
        curLineID += 2;
    };

    while (curLineID < input.size()) { eva_oneInput(); }

    auto const cartProd = std::views::cartesian_product(locks, keys);
    auto       res      = std::ranges::count_if(cartProd, [](auto &&a) {
        int cmp = 0;
        for (int i = 0; i < 5; ++i) { cmp += (std::get<0>(a)[i] + std::get<1>(a)[i]) <= 5; }
        return cmp == 5;
    });

    return res;
}
} // namespace AOC2024