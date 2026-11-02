#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <ranges>
#include <string>


namespace AOC2023 {
auto
day25_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"([\-]?\d+)">;

    auto input = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, any_ctre).front();
    return input;
}

size_t
day25_1(std::string dataFile) {

    auto input = day25_0(dataFile);
    return 0uz;
}

size_t
day25_2(std::string dataFile) {
    return 0uz;
}
} // namespace AOC2025