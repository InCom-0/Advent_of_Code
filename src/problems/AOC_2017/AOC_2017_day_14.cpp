#include <AOC_2017_day_10.h>
#include <bitset>
#include <fstream>
#include <queue>

#include "bitlib/bitlib.hpp"
#include <incom_commons.h>
#include <bits/ranges_algo.h>
#include <ctre.hpp>

#include <flux.hpp>

namespace AOC2017 {
int day14_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    int bitAccu      = 0;
    oneStr          += '-';
    int trueCounter  = 0;

    for (int i = 0; i < 128; ++i) {
        std::string input  = oneStr + std::to_string(i);
        input              = day10_3(std::move(input));
        trueCounter       += std::ranges::count_if(input, [](auto &&oneChar) { return oneChar == '1'; });
    }

    return trueCounter;
}
int day14_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    // DATA PREP
    oneStr += '-';
    std::vector<std::string> map;
    for (int i = 0; i < 128; ++i) {
        std::string input = oneStr + std::to_string(i);
        input             = day10_3(std::move(input));
        map.push_back(input);
    }

    // DP: bit::bit1 means 'visited'
    bit::bit_vector<unsigned char> dp_line(130, bit::bit0);
    dp_line.front() = bit::bit1;
    dp_line.back()  = bit::bit1;
    std::vector<bit::bit_vector<unsigned char>> dp;

    dp.push_back(bit::bit_vector<unsigned char>(130, bit::bit1));
    for (int i = 0; i < 128; ++i) { dp.push_back(dp_line); }
    dp.push_back(bit::bit_vector<unsigned char>(130, bit::bit1));

    // LAMBDA CLOSURE DEFINITION
    std::vector<std::pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    auto                             groupFinder = [&](this auto &self, int &rowID, int &colID) -> bool {
        if (dp[rowID + 1][colID + 1] == bit::bit1) { return false; }
        else if (map[rowID][colID] == '0') {
            dp[rowID + 1][colID + 1] = bit::bit1;
            return false;
        }
        else {
            for (auto &oneDir : dirs) {
                dp[rowID + 1][colID + 1]  = bit::bit1;
                rowID                    += oneDir.first;
                colID                    += oneDir.second;
                self(rowID, colID); // Recursive call
                rowID -= oneDir.first;
                colID -= oneDir.second;
            }
            return true;
        }
        std::unreachable();
    };

    // MAIN LOGIC
    int groupCounter = 0;
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 128; ++j) {
            if (groupFinder(i, j) == true) { groupCounter++; }
        }
    }
    return groupCounter;
}

} // namespace AOC2017