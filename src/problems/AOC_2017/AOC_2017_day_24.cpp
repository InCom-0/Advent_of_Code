#include <fstream>
#include <iostream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>

namespace AOC2017 {

int day24_1(std::string dataFile) {
    // CUSTOM TYPES
    struct component {
        unsigned int pinA;
        unsigned int pinB;
        bool         used = true;
    };

    auto left_ctre  = ctre::search<R"(.+(?=\/))">;
    auto right_ctre = ctre::search<R"((?<=\/).+)">;

    // DATA AND LAMBDA PREP
    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, left_ctre, right_ctre);
    std::vector<std::vector<unsigned int>> inted;
    unsigned int                           maxPins = 0;
    for (auto &item : VofV) {
        inted.push_back({std::stoul(item.front()), std::stoul(item.back())});
        maxPins = std::max(maxPins, inted.back().front());
        maxPins = std::max(maxPins, inted.back().back());
    }

    std::vector<std::vector<component>> map(maxPins + 1, std::vector<component>(maxPins + 1, component{}));
    for (auto &item : inted) {
        map[item.front()][item.back()] = {item.front(), item.back(), false};
        map[item.back()][item.front()] = {item.back(), item.front(), false};
    }

    unsigned int curMaxStrength = 0;
    unsigned int curStrength    = 0;
    auto         dfsFinder      = [&](this auto &self, unsigned int next2select) -> void {
        for (auto &componentItem : map[next2select]) {
            if (componentItem.used) { continue; }
            componentItem.used                                = true;
            map[componentItem.pinB][componentItem.pinA].used  = true;
            curStrength                                      += componentItem.pinA;
            curStrength                                      += componentItem.pinB;

            self(componentItem.pinB);

            curStrength                                      -= componentItem.pinA;
            curStrength                                      -= componentItem.pinB;
            map[componentItem.pinB][componentItem.pinA].used  = false;
            componentItem.used                                = false;
        }
        curMaxStrength = std::max(curMaxStrength, curStrength);
    };

    dfsFinder(0);
    return curMaxStrength;
}

int day24_2(std::string dataFile) {
    // CUSTOM TYPES
    struct component {
        unsigned int pinA;
        unsigned int pinB;
        bool         used = true;
    };

    auto left_ctre  = ctre::search<R"(.+(?=\/))">;
    auto right_ctre = ctre::search<R"((?<=\/).+)">;

    // DATA AND LAMBDA PREP
    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, left_ctre, right_ctre);
    std::vector<std::vector<unsigned int>> inted;
    unsigned int                           maxPins = 0;
    for (auto &item : VofV) {
        inted.push_back({std::stoul(item.front()), std::stoul(item.back())});
        maxPins = std::max(maxPins, inted.back().front());
        maxPins = std::max(maxPins, inted.back().back());
    }

    std::vector<std::vector<component>> map(maxPins + 1, std::vector<component>(maxPins + 1, component{}));
    for (auto &item : inted) {
        map[item.front()][item.back()] = {item.front(), item.back(), false};
        map[item.back()][item.front()] = {item.back(), item.front(), false};
    }

    unsigned int curMaxStrength = 0;
    unsigned int curStrength    = 0;
    unsigned int curMaxLength   = 0;
    unsigned int curLength      = 0;


    auto dfsFinder = [&](this auto &self, unsigned int next2select) -> void {
        for (auto &componentItem : map[next2select]) {
            if (componentItem.used) { continue; }
            componentItem.used                                = true;
            map[componentItem.pinB][componentItem.pinA].used  = true;
            curStrength                                      += componentItem.pinA;
            curStrength                                      += componentItem.pinB;
            curLength++;

            self(componentItem.pinB);

            curLength--;
            curStrength                                      -= componentItem.pinA;
            curStrength                                      -= componentItem.pinB;
            map[componentItem.pinB][componentItem.pinA].used  = false;
            componentItem.used                                = false;
        }
        if (curLength >= curMaxLength) {
            curMaxStrength = std::max(curMaxStrength, curStrength);
            curMaxLength   = curLength;
        }
    };

    dfsFinder(0);
    return curMaxStrength;
}
} // namespace AOC2017