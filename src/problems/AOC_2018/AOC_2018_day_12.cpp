#include "flux/op/ref.hpp"
#include <AOC_commons.h>
#include <algorithm>
#include <bitlib/bitlib.hpp>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>
#include <unordered_map>


namespace AOC2018 {
int day12_1(std::string dataFile, long long generationsToSimulate) {
    // DATA PREP
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return INT_MIN; }

    std::string oneStr;
    std::getline(iStream, oneStr);
    std::string curStateA(4, '.');
    curStateA.append(oneStr.begin() + 15, oneStr.end());
    curStateA.append(4, '.');
    std::getline(iStream, oneStr);

    std::unordered_map<std::string, char, AOC_commons::XXH3Hasher> map;
    while (std::getline(iStream, oneStr)) {
        map.emplace(std::string(oneStr.begin(), oneStr.begin() + 5), oneStr.back());
    };

    // LAMBDA DEFINITIONS
    int  addedOnTheLeft = 4;
    auto addAndRotate   = [&]() {
        if (curStateA[2] == '#') {
            curStateA.push_back('.');
            curStateA.push_back('.');
            std::rotate(curStateA.rbegin(), curStateA.rbegin() + 2, curStateA.rend());
            addedOnTheLeft += 2;
        }
        else if (curStateA[3] == '#') {
            curStateA.push_back('.');
            std::rotate(curStateA.rbegin(), curStateA.rbegin() + 1, curStateA.rend());
            addedOnTheLeft++;
        }

        if (curStateA[curStateA.size() - 3] == '#') {
            curStateA.push_back('.');
            curStateA.push_back('.');
        }
        else if (curStateA[curStateA.size() - 4] == '#') { curStateA.push_back('.'); }
    };

    // MAIN LOGIC
    for (int gen = 0; gen < generationsToSimulate; ++gen) {
        addAndRotate();
        std::string curStateB = curStateA;

        for (int fiveBlock = 0; fiveBlock < (curStateA.size() - 5); ++fiveBlock) {
            std::string toLookup(curStateB.begin() + fiveBlock, curStateB.begin() + fiveBlock + 5);
            if (map.contains(toLookup)) { curStateA[fiveBlock + 2] = map.at(toLookup); }
        }
        std::cout << addedOnTheLeft << '\n';
        std::cout << curStateA << '\n';
    }

    auto res = flux::ref(curStateA).fold(
        [potID = -addedOnTheLeft](auto &&ini, auto &&a) mutable {
            if (a == '#') { ini += potID; }
            potID++;
            return ini;
        },
        0);
    return res;
}

long long day12_2(std::string dataFile, long long generationsToSimulate) {
    // DATA PREP
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return INT_MIN; }

    std::string oneStr;
    std::getline(iStream, oneStr);
    std::string curStateA(4, '.');
    curStateA.append(oneStr.begin() + 15, oneStr.end());
    curStateA.append(4, '.');
    std::getline(iStream, oneStr);

    std::unordered_map<std::string, char, AOC_commons::XXH3Hasher> map;
    while (std::getline(iStream, oneStr)) {
        map.emplace(std::string(oneStr.begin(), oneStr.begin() + 5), oneStr.back());
    };

    // LAMBDA DEFINITIONS
    int  addedOnTheLeft = 4;
    auto addAndRotate   = [&]() {
        if (curStateA[2] == '#') {
            curStateA.push_back('.');
            curStateA.push_back('.');
            std::rotate(curStateA.rbegin(), curStateA.rbegin() + 2, curStateA.rend());
            addedOnTheLeft += 2;
        }
        else if (curStateA[3] == '#') {
            curStateA.push_back('.');
            std::rotate(curStateA.rbegin(), curStateA.rbegin() + 1, curStateA.rend());
            addedOnTheLeft++;
        }

        if (curStateA[curStateA.size() - 3] == '#') {
            curStateA.push_back('.');
            curStateA.push_back('.');
        }
        else if (curStateA[curStateA.size() - 4] == '#') { curStateA.push_back('.'); }
    };

    // MAIN LOGIC
    for (int gen = 0; gen < 200; ++gen) {
        addAndRotate();
        std::string curStateB = curStateA;

        for (int fiveBlock = 0; fiveBlock < (curStateA.size() - 5); ++fiveBlock) {
            std::string toLookup(curStateB.begin() + fiveBlock, curStateB.begin() + fiveBlock + 5);
            if (map.contains(toLookup)) { curStateA[fiveBlock + 2] = map.at(toLookup); }
        }
        // std::cout << addedOnTheLeft << '\n';
        // std::cout << curStateA << '\n';
    }

    long long res = flux::ref(curStateA).fold(
        [potID = -addedOnTheLeft](auto &&ini, auto &&a) mutable {
            if (a == '#') { ini += potID; }
            potID++;
            return ini;
        },
        0);
    auto countOfPlants = flux::ref(curStateA).count_if([](auto &&a) { return a == '#'; });

    res += (generationsToSimulate - 200) * countOfPlants;
    return res;
}
} // namespace AOC2018