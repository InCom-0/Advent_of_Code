#include <AOC_2022.h>
#include <algorithm>
#include <functional>
#include <incom_commons.hpp>
#include <ranges>

namespace AOC2022 {
auto
day8_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto inp      = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    return std::views::transform(inp,
                                 [](auto const &line) {
                                     return std::views::transform(
                                                line, [](auto const &c) { return static_cast<int>(c - '0'); }) |
                                            std::ranges::to<std::vector>();
                                 }) |
           std::ranges::to<std::vector>();
}

std::size_t
day8_1(std::string dataFile) {

    auto input = day8_0(dataFile);

    std::vector<int> tracker(input.size() * input.front().size(), 0);
    incpf::mdspan    trackerView(tracker.data(), input.size(), input.front().size());

    for (size_t lineID{}; auto const &line : input) {
        int curMax = -1;
        for (size_t fl{}; fl < line.size(); ++fl) {
            if (line[fl] > curMax) { trackerView[lineID, fl] = 1; }
            curMax = std::max(curMax, line[fl]);
        }
        curMax = -1;
        for (size_t fr = line.size() - 1; fr < line.size(); --fr) {
            if (line[fr] > curMax) { trackerView[lineID, fr] = 1; }
            curMax = std::max(curMax, line[fr]);
        }
        ++lineID;
    }

    for (size_t colID{}; colID < input.front().size(); ++colID) {
        int curMax = -1;
        for (size_t ft{}; ft < input.size(); ++ft) {
            if (input[ft][colID] > curMax) { trackerView[ft, colID] = 1; }
            curMax = std::max(curMax, input[ft][colID]);
        }
        curMax = -1;
        for (size_t fb = input.size() - 1; fb < input.size(); --fb) {
            if (input[fb][colID] > curMax) { trackerView[fb, colID] = 1; }
            curMax = std::max(curMax, input[fb][colID]);
        }
    }

    return std::ranges::count(tracker, 1);
}

std::size_t
day8_2(std::string dataFile) {
    auto input = day8_0(dataFile);

    std::vector<int> tracker(input.size() * input.front().size(), 1);
    incpf::mdspan    tv(tracker.data(), input.size(), input.front().size());


    std::vector<int> curVisiCountFromSize(10, 0);

    for (size_t lineID{}; auto const &line : input) {
        std::ranges::fill(curVisiCountFromSize, 0);
        for (size_t fl{}; fl < line.size(); ++fl) {
            tv[lineID, fl] *= curVisiCountFromSize[line[fl]];

            for (size_t i{}; i <= line[fl]; ++i) { curVisiCountFromSize[i] = 1; }
            for (size_t i = line[fl] + 1; i <= 9; ++i) { curVisiCountFromSize[i] += 1; }
        }

        std::ranges::fill(curVisiCountFromSize, 0);
        for (size_t fr = line.size() - 1; fr < line.size(); --fr) {
            tv[lineID, fr] *= curVisiCountFromSize[line[fr]];

            for (size_t i{}; i <= line[fr]; ++i) { curVisiCountFromSize[i] = 1; }
            for (size_t i = line[fr] + 1; i <= 9; ++i) { curVisiCountFromSize[i] += 1; }
        }
        ++lineID;
    }

    for (size_t colID{}; colID < input.front().size(); ++colID) {
        std::ranges::fill(curVisiCountFromSize, 0);
        for (size_t ft{}; ft < input.size(); ++ft) {
            tv[ft, colID] *= curVisiCountFromSize[input[ft][colID]];
            for (size_t i{}; i <= input[ft][colID]; ++i) { curVisiCountFromSize[i] = 1; }
            for (size_t i = input[ft][colID] + 1; i <= 9; ++i) { curVisiCountFromSize[i] += 1; }
        }

        std::ranges::fill(curVisiCountFromSize, 0);
        for (size_t fb = input.size() - 1; fb < input.size(); --fb) {
            tv[fb, colID] *= curVisiCountFromSize[input[fb][colID]];

            for (size_t i{}; i <= input[fb][colID]; ++i) { curVisiCountFromSize[i] = 1; }
            for (size_t i = input[fb][colID] + 1; i <= 9; ++i) { curVisiCountFromSize[i] += 1; }
        }
    }


    auto res = std::ranges::max_element(tracker, std::less{});
    if (res == tracker.end()) { assert(false); }

    return *res;
}
} // namespace AOC2022