#include <fstream>
#include <iostream>
#include <queue>

#include <incom_commons.h>
#include <ctre.hpp>
#include <intrin.h>
#include <unordered_set>

#include <flux.hpp>

namespace AOC2017 {
int day13_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofV   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre);

    std::unordered_map<int, int, incom::commons::XXH3Hasher> mp;
    for (auto &inpLine : VofV) { mp.emplace(std::stoi(inpLine.front()), std::stoi(inpLine.back())); }

    int lastDepth    = std::ranges::max_element(mp, [](auto &&a, auto &&b) { return a.first < b.first; })->first;
    int severityAccu = 0;

    for (int i = 0; i < lastDepth; ++i) {
        if (mp.contains(i) && (i == 0 || ((i % ((mp.at(i) - 1) * 2)) == 0))) { severityAccu += (mp.at(i) * i); }
    }
    return severityAccu;
}
int day13_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofV   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre);

    std::unordered_map<int, int, incom::commons::XXH3Hasher> mp;
    for (auto &inpLine : VofV) { mp.emplace(std::stoi(inpLine.front()), std::stoi(inpLine.back())); }

    bool brk;
    int  inputToEvaluate = -1;
    do {
        brk = false;
        inputToEvaluate++;
        for (auto iter = mp.begin(); iter != mp.end(); ++iter) {
            if (((iter->first + inputToEvaluate) % ((iter->second - 1) * 2)) == 0) {
                brk = true;
                break;
            }
        }
    } while (brk);

    return inputToEvaluate;
}
} // namespace AOC2017