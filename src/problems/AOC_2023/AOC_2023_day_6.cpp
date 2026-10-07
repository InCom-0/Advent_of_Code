#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cassert>
#include <charconv>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <string>
#include <utility>


namespace AOC2023 {

auto
day6_0(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    return incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
}


size_t
day6_1(std::string dataFile) {
    auto input = day6_0(dataFile);

    size_t res = 1uz;
    for (auto const [seconds, record] :
         std::views::zip(input[0], input[1]) | std::views::transform([](auto const &stringPair) {
             return std::make_pair(std::stoull(std::get<0>(stringPair)), std::stoull(std::get<1>(stringPair)));
         })) {

        size_t half    = seconds / 2uz;
        size_t counter = 1uz;

        if (((seconds - half) * half) > record) {

            counter += (seconds % 2uz);
            half--;
            while (((seconds - half) * half) > record) {
                counter += 2uz;
                half--;
            }
        }

        res *= counter;
    }

    return res;
}


size_t
day6_2(std::string dataFile) {
    auto input = day6_0(dataFile);

    auto seconds = std::stoull(
        std::ranges::fold_left(input[0], "", [](auto const &init, auto const &item) { return init + item; }));
    auto record = std::stoull(
        std::ranges::fold_left(input[1], "", [](auto const &init, auto const &item) { return init + item; }));
    // auto secs = std::stoull(seconds);

    size_t res = 1uz;
    {
        size_t half    = seconds / 2uz;
        size_t counter = 1uz;

        if (((seconds - half) * half) > record) {

            counter += (seconds % 2uz);
            half--;
            while (((seconds - half) * half) > record) {
                counter += 2uz;
                half--;
            }
        }

        res *= counter;
    }

    return res;
}
} // namespace AOC2023