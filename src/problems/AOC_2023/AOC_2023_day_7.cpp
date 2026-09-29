#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <string>


namespace AOC2023 {

auto
day7_0(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    return incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
}


size_t
day7_1(std::string dataFile) {

    return 0;
}


size_t
day7_2(std::string dataFile) {


    return 0;
}
} // namespace AOC2023