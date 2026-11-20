#include <AOC_2022.h>
#include <bitlib/bit-algorithms/count.hpp>
#include <bitlib/bit-iterator/bit_value.hpp>
#include <cassert>
#include <incom_commons.hpp>

#include <bitlib/bitlib.hpp>
#include <limits>

namespace AOC2022 {
auto
day6_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    return incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front().front();
}

auto
day6_00(std::string const dataFile, size_t const uniqueCharsToFind) {
    auto const input = day6_0(dataFile);

    bit::bit_vector<unsigned char> bv(256, bit::bit0);
    if (input.size() < uniqueCharsToFind) { assert(false); }

    size_t res = std::numeric_limits<size_t>::max();
    for (size_t i{}; auto const &sldView : std::views::slide(input, uniqueCharsToFind)) {
        for (auto const &c : sldView) { bv[c] = bit::bit1; }

        auto test = bit::count(bv.begin(), bv.end(), bit::bit1);
        if (test == uniqueCharsToFind) {
            res = i + uniqueCharsToFind;
            goto RET;
        }
        for (auto const &c : sldView) { bv[c] = bit::bit0; }
        ++i;
    }
RET:
    return res;
}

std::size_t
day6_1(std::string dataFile) {
    return day6_00(dataFile, 4);
}
std::size_t
day6_2(std::string dataFile) {
    return day6_00(dataFile, 14);
}
} // namespace AOC2022