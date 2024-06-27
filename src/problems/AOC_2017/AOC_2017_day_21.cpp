#include <fstream>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2017 {

auto matrixRotateLeft(std::vector<std::vector<int>> &&VofVlike) {
    if (VofVlike.size() < 2) { return VofVlike; }
    if (VofVlike.size() != VofVlike.front().size()) { return std::vector<std::vector<int>>{std::vector<int>{INT_MIN}}; }

    int sideLength = VofVlike.size();
    int circles    = (sideLength + 1) / 2;

    for (int cir = 0; cir < circles; cir++) {
        for (int i = 0; sideLength; ++i) {}
    }

    return VofVlike;
}

int day21_1(std::string dataFile) {}

int day21_2(std::string dataFile) {}

} // namespace AOC2017