#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <limits>
#include <ranges>
#include <string>


namespace AOC2023 {

auto
day14_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    return incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
}

auto
day14_1(std::string dataFile) {
    auto input = day14_0(dataFile);

    for (size_t lineID{}; auto &line : input) {
        for (size_t colID{}; auto &c : line) {
            if (c == 'O') {
                size_t rockID = lineID;
                while (--rockID != std::numeric_limits<size_t>::max() && input[rockID][colID] == '.') {
                    std::swap(input[rockID][colID], input[rockID + 1][colID]);
                }
            }
            colID++;
        }
        lineID++;
    }
    size_t res{};
    for (size_t weight = input.size(); auto &line : input) {
        res += std::ranges::count_if(line, [&](auto const c) { return c == 'O'; }) * weight--;
    }

    return res;
}

auto
day14_2(std::string dataFile) {
    auto input = day14_0(dataFile);

    auto tiltTopOrDown = [&]<bool DOWN = false>() {
        auto inp = [&]() {
            if constexpr (DOWN) { return std::views::reverse(input); }
            else { return std::views::all(input); }
        }();

        for (size_t lineID{}; auto &line : inp) {
            for (size_t colID{}; auto &c : line) {
                if (c == 'O') {
                    size_t rockID = lineID;
                    while (--rockID != std::numeric_limits<size_t>::max() && inp[rockID][colID] == '.') {
                        std::swap(inp[rockID][colID], inp[rockID + 1][colID]);
                    }
                }
                colID++;
            }
            lineID++;
        }
    };
    auto tiltLeftOrRight = [&]<bool RIGHT = false>() {
        auto inp = [&](auto &someRng) {
            if constexpr (RIGHT) { return std::views::reverse(someRng); }
            else { return std::views::all(someRng); }
        };

        size_t const lineSz = input.front().size();
        for (size_t lineID{}; auto &line : input) {
            for (size_t colID{}; auto const c : inp(line)) {
                if (c == 'O') {
                    size_t rockID = colID;
                    while (--rockID != std::numeric_limits<size_t>::max() && inp(line)[rockID] == '.') {
                        std::swap(inp(line)[rockID], inp(line)[rockID + 1]);
                    }
                }
                colID++;
            }
            lineID++;
        }
    };

    auto oneCycle = [&]() {
        tiltTopOrDown();
        tiltLeftOrRight();
        tiltTopOrDown.operator()<true>();
        tiltLeftOrRight.operator()<true>();
    };

    ankerl::unordered_dense::set<decltype(input), incstd::hashing::XXH3Hasher> mp{};
    size_t const                                                               maxIter = 1'000'000'000uz;

    size_t res{};
    for (size_t i{}; i < maxIter; ++i) {
        if (auto pr = mp.insert(input); pr.second) { oneCycle(); }
        else {
            auto inputToLookFor = *pr.first;
            for (size_t i2 = i + 1; i2 < maxIter; ++i2) {
                oneCycle();
                if (inputToLookFor == input) {
                    size_t const cycleSize   = i2 - i;
                    size_t const cycleFrom_i = (maxIter - i) / cycleSize;

                    size_t howManyMore = (maxIter - i) - (cycleFrom_i * cycleSize);
                    while (howManyMore-- > 0uz) { oneCycle(); }
                    break;
                }
            }
            break;
        }
    }


    for (size_t weight = input.size(); auto &line : input) {
        res += std::ranges::count_if(line, [&](auto const c) { return c == 'O'; }) * weight--;
    }

    return res;
}

} // namespace AOC2023