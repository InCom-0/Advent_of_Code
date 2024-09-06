#pragma once

#include <string>
#include <flux.hpp>

namespace AOC2018 {
    std::pair<size_t, size_t> day17_0(std::string dataFile);
    size_t day17_1(std::string dataFile);
    size_t day17_2(std::string dataFile);

    std::vector<std::vector<char>> day17_alt_0(std::string dataFile);
} // namespace AOC2018