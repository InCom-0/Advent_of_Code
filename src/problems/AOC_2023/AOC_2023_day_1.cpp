#include <ankerl/unordered_dense.h>
#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <incstd/core/hashing.hpp>
#include <iostream>
#include <ranges>
#include <string>


namespace AOC2023 {

long long
day1_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    size_t accu = 0uz;

    for (auto const &line : input) {
        for (auto const &digitString : std::views::take(line, 1)) {
            for (char const oneChr : std::views::take(digitString, 1)) {
                accu += (static_cast<size_t>(oneChr - '0') * 10);
            }
        }
        for (auto const &digitString : std::views::drop(line, line.size() - 1)) {
            for (char const oneChr : std::views::take(digitString, 1)) { accu += (static_cast<size_t>(oneChr - '0')); }
        }
    }

    return accu;
}

long long
day1_2(std::string dataFile) {
    auto first_re = ctre::search<R"(\d|one|two|three|four|five|six|seven|eight|nine)">;

    ankerl::unordered_dense::map<std::string, size_t, incom::standard::hashing::XXH3Hasher> mp{
        {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
        {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
    };

    size_t accu = 0uz;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }

    std::string oneStr;

    while (std::getline(iStream, oneStr)) {
        auto bg  = oneStr.begin();
        auto end = oneStr.end();

        auto result = first_re(bg, end);
        if (result) {
            auto locStr = result.to_string();
            if (locStr.size() == 1) { accu += (static_cast<size_t>(locStr.front() - '0') * 10); }
            else {
                if (auto foundIT = mp.find(locStr); foundIT != mp.end()) { accu += (foundIT->second) * 10; }
                else { assert(false); }
            }
        }

        auto beg2 = oneStr.end();
        result    = first_re(beg2, end);
        while (not result) { result = first_re(--beg2, end); }

        if (result) {
            auto locStr = result.to_string();
            if (locStr.size() == 1) { accu += (static_cast<size_t>(locStr.front() - '0')); }
            else {
                if (auto foundIT = mp.find(locStr); foundIT != mp.end()) { accu += (foundIT->second); }
                else { assert(false); }
            }
        }
    }

    return accu;
}

} // namespace AOC2023