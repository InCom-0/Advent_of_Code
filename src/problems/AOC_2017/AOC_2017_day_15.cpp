#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"


namespace AOC2017 {
    int day15_1(std::string dataFile) {
        auto d_ctre = ctre::search<R"(\d+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

        unsigned long long lastA = std::stoi(VofV.front().front());
        unsigned long long lastB = std::stoi(VofV.back().front());

        unsigned long long const genA_mult = 16807;
        unsigned long long const genB_mult = 48271;

        unsigned long long const divi = INT_MAX;
        unsigned long long const mask = USHRT_MAX;

        int counter = 0;
        for (int i = 0; i < 40'000'000; ++i) {
            lastA = (lastA * genA_mult) % divi;
            lastB = (lastB * genB_mult) % divi;
            counter += ((lastA & mask) == (lastB & mask));
        }

        return counter;
    }
    int day15_2(std::string dataFile) {
        auto d_ctre = ctre::search<R"(\d+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

        unsigned long long lastA = std::stoi(VofV.front().front());
        unsigned long long lastB = std::stoi(VofV.back().front());

        unsigned long long const genA_mult = 16807;
        unsigned long long const genB_mult = 48271;

        unsigned long long const divi = INT_MAX;
        unsigned long long const mask = USHRT_MAX;

        std::queue<unsigned long long> queA;
        std::queue<unsigned long long> queB;

        int counter = 0;
        int considerationCount = 5'000'000;
        while (true) {
            lastA = (lastA * genA_mult) % divi;
            lastB = (lastB * genB_mult) % divi;
            if ((lastA % 4) == 0) queA.push(lastA);
            if ((lastB % 8) == 0) queB.push(lastB);
            if (queA.size() > 0 && queB.size() > 0) {
                considerationCount--;
                counter += ((queA.front() & mask) == (queB.front() & mask));
                queA.pop(); queB.pop();
            }
            if (considerationCount == 0) break;
        }
        return counter;
    }

}