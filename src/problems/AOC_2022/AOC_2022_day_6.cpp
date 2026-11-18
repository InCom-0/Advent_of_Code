#include <AOC_2022.h>
#include <bitlib/bit-algorithms/count.hpp>
#include <bitlib/bit-iterator/bit_value.hpp>
#include <cassert>
#include <incom_commons.hpp>

#include <bitlib/bitlib.hpp>

namespace AOC2022 {
auto
day6_0(std::string dataFile) {

    bit::bit_vector<unsigned char> bv;


    auto any_ctre = ctre::search<R"(.+)">;
    return incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front().front();
}
std::size_t
day6_1(std::string dataFile) {
    auto const input = day6_0(dataFile);

    bit::bit_vector<unsigned char> bv(256, bit::bit0);
    if (input.size() < 4) { assert(false); }

    auto   inpSlideView = std::views::slide(input, 4);
    size_t res{};

    for (size_t i{}; auto const &sldView : std::views::slide(input, 4)) {
        for (auto const &c : sldView) { bv[c] = bit::bit1; }

        auto test = bit::count(bv.begin(), bv.end(), bit::bit1);
        if (test == 4) {
            res = i + 4;
            break;
        }
        for (auto const &c : sldView) { bv[c] = bit::bit0; }
        ++i;
    }

    return res;
}
std::size_t
day6_2(std::string dataFile) {
    auto const input = day6_0(dataFile);

    bit::bit_vector<unsigned char> bv(256, bit::bit0);
    if (input.size() < 14) { assert(false); }

    size_t res{};

    for (size_t i{}; auto const &sldView : std::views::slide(input, 14)) {
        for (auto const &c : sldView) { bv[c] = bit::bit1; }

        auto test = bit::count(bv.begin(), bv.end(), bit::bit1);
        if (test == 14) {
            res = i + 14;
            break;
        }
        for (auto const &c : sldView) { bv[c] = bit::bit0; }
        ++i;
    }

    return res;
}
} // namespace AOC2022