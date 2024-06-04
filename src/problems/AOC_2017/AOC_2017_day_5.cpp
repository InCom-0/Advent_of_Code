#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"


namespace AOC2017 {
    int day5_1(std::string dataFile) {
        auto VofV = AOC_commons::parseInputUsingCTRE::processFile(dataFile, ctre::search<R"(-?\d+)">);
        std::vector<int> input;
        for (auto &str: VofV[0]) input.push_back(std::stoi(str));

        int cur = 0, iter = 0;
        while (cur < input.size()) {
            cur += input[cur]++;
            iter++;
        }
        return iter;
    }
    int day5_2(std::string dataFile) {
        auto VofV = AOC_commons::parseInputUsingCTRE::processFile(dataFile, ctre::search<R"(-?\d+)">);
        std::vector<int> input;
        for (auto &str: VofV[0]) input.push_back(std::stoi(str));

        int cur = 0, iter = 0, jumpBy = 0;
        while (cur < input.size()) {
            jumpBy = input[cur];
            input[cur] += jumpBy >= 3 ? -1 : 1;
            cur += jumpBy;
            iter++;
        }
        return iter;
    }
    // Misguided attempt to avoid a conditional ... :-)
    int day5_3(std::string dataFile) {
        auto VofV = AOC_commons::parseInputUsingCTRE::processFile(dataFile, ctre::search<R"(-?\d+)">);
        std::vector<int> input;
        for (auto &str: VofV[0]) input.push_back(std::stoi(str));

        int cur = 0, iter = 0, jumpBy = 0;
        while (cur < input.size()) {
            jumpBy = input[cur];
            input[cur] += 1 + (-2)*(bool)(((jumpBy & ~(jumpBy >> 31))-1)/2);
            cur += jumpBy;
            iter++;
        }
        return iter;
    }
}