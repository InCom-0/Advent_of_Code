#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <functional>
#include <incom_commons.hpp>
#include <incstd/core/buffers.hpp>
#include <ranges>
#include <string>
#include <vector>


namespace AOC2023 {
auto
day9_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(-?\d+)">;
    return incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre) |
           std::views::transform([](auto const &line) {
               return line |
                      std::views::transform([](auto const &oneVal_asString) { return std::stoll(oneVal_asString); }) |
                      std::ranges::to<std::vector>();
           }) |
           std::ranges::to<std::vector>();
    ;
}

auto
day9_1(std::string dataFile) {
    auto input = day9_0(dataFile);

    auto getNextVal = [](std::vector<long long> const &inpVals) {
        if (inpVals.size() < 2) { return 0ll; }
        else if (inpVals.size() == 2) { return ((2 * inpVals.back()) - inpVals.front()); }

        incstd::buffers::DoubleBuffer<std::vector<long long>> buf(inpVals);

        std::vector<long long> lastDiag{};
        lastDiag.reserve(inpVals.size());
        lastDiag.push_back(inpVals.back());

        while (std::ranges::any_of(buf.getCurrent(), [](auto val) { return val != 0; })) {
            for (auto const [first, second] : std::views::pairwise(buf.getCurrent())) {
                buf.getNext().push_back(second - first);
            }
            lastDiag.push_back(buf.getNext().back());
            buf.swap_buffers();
            buf.getNext().clear();
        }

        return std::ranges::fold_left(lastDiag | std::views::reverse, 0ll, std::plus{});
    };

    return std::ranges::fold_left(
        std::views::transform(input, [&](auto const &inpLine) { return getNextVal(inpLine); }), 0ll, std::plus{});
}

auto
day9_2(std::string dataFile) {
    auto input = day9_0(dataFile);

    auto getNextVal = [](std::vector<long long> const &inpVals) {
        if (inpVals.size() < 2) { return 0ll; }
        else if (inpVals.size() == 2) { return ((2 * inpVals.back()) - inpVals.front()); }

        incstd::buffers::DoubleBuffer<std::vector<long long>> buf(inpVals);

        std::vector<long long> lastDiag{};
        lastDiag.reserve(inpVals.size());
        lastDiag.push_back(inpVals.front());

        while (std::ranges::any_of(buf.getCurrent(), [](auto val) { return val != 0; })) {
            for (auto const [first, second] : std::views::pairwise(buf.getCurrent())) {
                buf.getNext().push_back(second - first);
            }
            lastDiag.push_back(buf.getNext().front());
            buf.swap_buffers();
            buf.getNext().clear();
        }

        return std::ranges::fold_right(lastDiag, 0ll, std::minus{});
    };

    return std::ranges::fold_left(
        std::views::transform(input, [&](auto const &inpLine) { return getNextVal(inpLine); }), 0ll, std::plus{});
}

} // namespace AOC2023