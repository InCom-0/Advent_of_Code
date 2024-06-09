#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <unordered_set>

#include "flux.hpp"


namespace AOC2017 {
    int day12_1(std::string dataFile) {
        auto d_ctre = ctre::search<R"(\d+)">;
        auto d2end_ctre = ctre::search<R"(\d.+)">;

        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d2end_ctre);

        std::unordered_map<const int, std::vector<int>, AOC_commons::XXH3Hasher> mp;
        for (auto &line: VofV) {
            std::vector<std::string> toInsert = AOC_commons::parseInputUsingCTRE::processOneLineRPToneVect(line.back(), d_ctre);
            std::vector<int> toInsertInts;
            for (auto &str: toInsert) toInsertInts.push_back(std::stoi(str));
            mp.emplace(std::stoi(line.front()), std::move(toInsertInts));
        }
        std::queue<int> queOfNodes;
        std::unordered_set<int> found;
        queOfNodes.push(0);

        while (not queOfNodes.empty()) {
            for (auto &oneTarget: mp[queOfNodes.front()]) {
                if (not found.contains(oneTarget)) queOfNodes.push(oneTarget);
            }
            found.emplace(queOfNodes.front());
            queOfNodes.pop();
        }
        return found.size();
    }

    int day12_2(std::string dataFile) {
        auto d_ctre = ctre::search<R"(\d+)">;
        auto d2end_ctre = ctre::search<R"(\d.+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d2end_ctre);

        std::unordered_map<const int, std::vector<int>, AOC_commons::XXH3Hasher> mp;
        for (auto &line: VofV) {
            std::vector<std::string> toInsert = AOC_commons::parseInputUsingCTRE::processOneLineRPToneVect(line.back(), d_ctre);
            std::vector<int> toInsertInts;
            for (auto &str: toInsert) toInsertInts.push_back(std::stoi(str));
            mp.emplace(std::stoi(line.front()), std::move(toInsertInts));
        }

        std::unordered_set<int> allFounds;
        int numOfGroups = 0;

        for (auto &onePair: mp) {
            if (allFounds.contains(onePair.first)) continue;

            numOfGroups++;
            std::queue<int> queOfNodes;
            queOfNodes.push(onePair.first);

            while (not queOfNodes.empty()) {
                for (auto &oneTarget: mp[queOfNodes.front()]) {
                    if (not allFounds.contains(oneTarget)) queOfNodes.push(oneTarget);
                }
                allFounds.emplace(queOfNodes.front());
                queOfNodes.pop();
            }
        }
        return numOfGroups;
    }


}