#include <iostream>
#include <fstream>
#include <queue>
#include <format>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <unordered_set>
#include <glaze/glaze.hpp>



namespace AOC2017 {

    int day6_1(std::string dataFile) {
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, ctre::search<R"(\d+)">);
        std::vector<int> banks;


        for (auto &bank: VofV.front()) banks.push_back(std::stoi(bank));
        std::unordered_set<std::vector<int>, AOC_commons::XXH3Hasher> pastConfs;

        int cycleCount = 0;
        while (not pastConfs.contains(banks)) {
            pastConfs.insert(banks);
            auto maxEle = std::max_element(banks.begin(), banks.end(), std::less());

            int blocksToDistribute = maxEle.operator*();
            maxEle.operator*() = 0;

            while (blocksToDistribute > 0) {
                ++maxEle;
                if (maxEle == banks.end()) maxEle = banks.begin();
                maxEle.operator*()++;
                blocksToDistribute--;
            }
            cycleCount++;
        }
        return cycleCount;
    }
    int day6_2(std::string dataFile) {
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, ctre::search<R"(\d+)">);
        std::vector<int> banks;

        for (auto &bank: VofV.front()) banks.push_back(std::stoi(bank));
        std::unordered_map<std::vector<int>, int, AOC_commons::XXH3Hasher> pastConfs;

        int cycleCount = 0;
        while (not pastConfs.contains(banks)) {
            pastConfs.insert({banks, cycleCount});
            auto maxEle = std::max_element(banks.begin(), banks.end(), std::less());

            int blocksToDistribute = maxEle.operator*();
            maxEle.operator*() = 0;

            while (blocksToDistribute > 0) {
                ++maxEle;
                if (maxEle == banks.end()) maxEle = banks.begin();
                maxEle.operator*()++;
                blocksToDistribute--;
            }
            cycleCount++;
        }
        return cycleCount - pastConfs.at(banks);
    }
}