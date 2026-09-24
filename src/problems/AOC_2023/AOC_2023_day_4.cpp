#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.h>
#include <iostream>
#include <ranges>
#include <span>
#include <string>


namespace AOC2023 {

size_t
day4_1(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    auto const vals =
        std::views::transform(input,
                              [](auto const &line) {
                                  return std::views::transform(line, [](auto const &val) { return std::stoull(val); }) |
                                         std::ranges::to<std::vector>();
                              }) |
        std::ranges::to<std::vector>();

    size_t totalRes = 0uz;
    for (auto const &valLine : vals) {
        auto const winningView = std::span(valLine.begin() + 1, valLine.begin() + 11);
        size_t     locRes      = 0uz;

        for (auto const &ticketVal : std::span(valLine.begin() + 11, valLine.end())) {
            if (std::ranges::find(winningView, ticketVal) != winningView.end()) {
                locRes *= 2;
                locRes += (locRes == 0uz);
            }
        }
        totalRes += locRes;
    }

    return totalRes;
}

size_t
day4_2(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    auto const vals =
        std::views::transform(input,
                              [](auto const &line) {
                                  return std::views::transform(line, [](auto const &val) { return std::stoull(val); }) |
                                         std::ranges::to<std::vector>();
                              }) |
        std::ranges::to<std::vector>();

    std::vector cardsCounts(vals.size(), 1uz);

    for (size_t valsID = 0uz; auto const &valLine : vals) {
        auto const winningView = std::span(valLine.begin() + 1, valLine.begin() + 11);
        size_t     winNumCount = 0uz;

        for (auto const &ticketVal : std::span(valLine.begin() + 11, valLine.end())) {
            if (std::ranges::find(winningView, ticketVal) != winningView.end()) { winNumCount++; }
        }

        for (size_t incrementID = valsID + 1; incrementID < (valsID + 1 + winNumCount); ++incrementID) {
            cardsCounts.at(incrementID) += cardsCounts.at(valsID);
        }

        valsID++;
    }

    return std::ranges::fold_left(cardsCounts, 0uz, std::plus{});
}
} // namespace AOC2023