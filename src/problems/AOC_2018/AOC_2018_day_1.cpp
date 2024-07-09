#include "bitlib/bit-containers/bit_vector.hpp"
#include "bitlib/bit-iterator/bit_value.hpp"
#include <climits>
#include <ctre/wrapper.hpp>
#include <fstream>

#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2018 {
int day1_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(.+)">;
    auto VofV   = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<int> values;
    for (auto &line : VofV) { values.push_back(std::stoi(line.front())); }

    int accu = 0;
    for (auto &inputVal : values) { accu += inputVal; }

    return accu;
}

int day1_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto VofV   = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<int> values;
    for (auto &line : VofV) { values.push_back(std::stoi(line.front())); }

    bit::bit_vector<unsigned char> bv(INT_MAX, bit::bit0);
    int                            accu = INT_MAX / 2;
    while (true) {
        for (auto &inputVal : values) {
            if (bv[accu] == bit::bit1) { return accu - (INT_MAX / 2); }
            bv[accu]  = bit::bit1;
            accu     += inputVal;
        }
    }
    return accu;
}
} // namespace AOC2018