#include <algorithm>
#include <cmath>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <string>


namespace AOC2024 {
long long day2_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector data(input.size(), std::vector<int>());
    for (int i = 0; auto const &line : input) {
        for (auto &str : line) { data[i].push_back(std::stoi(str)); }
        i++;
    }

    auto res = std::ranges::count_if(data, [](auto &item) {
        auto rt  = std::views::pairwise_transform(item, [](auto &a, auto &b) { return a - b; });
        auto rt2 = std::views::pairwise_transform(
            rt, [](auto const &a, auto const &b) { return std::signbit(a) == std::signbit(b); });

        bool retA =
            std::ranges::all_of(rt, [](auto const &dist) { return (std::abs(dist) > 0 && std::abs(dist) < 4); });
        auto retB = std::ranges::all_of(rt2, [](auto const &aa) { return aa; });

        return retA && retB;
    });

    return res;
}

long long day2_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector data(input.size(), std::vector<int>());
    for (int i = 0; auto const &line : input) {
        for (auto &str : line) { data[i].push_back(std::stoi(str)); }
        i++;
    }

    auto eva = [](auto &vec) -> bool {
        auto rt  = std::views::pairwise_transform(vec, [](auto const &a, auto const &b) { return a - b; });
        auto rt2 = std::views::pairwise_transform(
            rt, [](auto const &a, auto const &b) { return std::signbit(a) == std::signbit(b); });

        bool retA =
            std::ranges::all_of(rt, [](auto const &dist) { return (std::abs(dist) > 0 && std::abs(dist) < 4); });
        auto retB = std::ranges::all_of(rt2, [](auto const &aa) { return aa; });

        return retA && retB;
    };

    auto res = std::ranges::count_if(data, [&](auto const &vec2) {
        size_t cursor = 0;
        auto attempt = std::views::enumerate(vec2) |
                       std::views::filter([&](auto const &a) { return std::get<0>(a) != cursor; }) |
                       std::views::transform([](auto const &aa) { return std::get<1>(aa); });

        for (int i = 0; i < vec2.size(); i++) {
            if (eva(attempt)) { return true; }
            cursor++;
        }
        return false;
    });

    return res;
}
} // namespace AOC2024