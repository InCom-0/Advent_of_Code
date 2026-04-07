#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>
#include <ranges>


namespace AOC2025 {
size_t
day3_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    size_t accu = 0uz;
    for (auto const &line : input) {
        char left  = '0';
        char right = '0';
        for (auto [l, r] : std::views::pairwise(line)) {
            if (l > left) {
                left  = l;
                right = '0';
            }
            if (r > right) { right = r; }
        }

        accu += ((left - 48) * 10) + (right - 48);
    }
    return accu;
}


size_t
day3_2(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    size_t accu = 0uz;
    for (auto const &line : input) {
        std::array<char, 12> cur{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
        for (auto window : std::views::slide(line, 12)) {
            for (size_t id = 0; auto oneChar : window) {
                if (oneChar > cur.at(id)) {
                    cur.at(id) = oneChar;
                    for (auto &toReset : std::ranges::subrange(cur.begin() + id + 1, cur.end())) { toReset = '0'; }
                }
                id++;
            }
        }
        size_t mult = 1;
        for (char oneDigit : std::views::reverse(cur)) {
            accu += (oneDigit - 48) * mult;
            mult *= 10;
        }
    }
    return accu;
}
} // namespace AOC2025