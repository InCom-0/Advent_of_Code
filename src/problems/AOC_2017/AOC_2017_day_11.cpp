#include <fstream>
#include <iostream>
#include <queue>

#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2017 {
int day11_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    auto                     d_ctre    = ctre::search<R"(\w+)">;
    std::vector<std::string> inputVect = incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(oneStr, d_ctre);

    std::unordered_map<std::string, std::pair<int, int>> dirMap;
    dirMap.emplace("n", std::make_pair(-2, 0));
    dirMap.emplace("ne", std::make_pair(-1, 1));
    dirMap.emplace("se", std::make_pair(1, 1));
    dirMap.emplace("s", std::make_pair(2, 0));
    dirMap.emplace("sw", std::make_pair(1, -1));
    dirMap.emplace("nw", std::make_pair(-1, -1));

    std::vector<std::string> VVVVV;

    auto targetCoord = flux::ref(inputVect)
                           .map([&](auto &&a) { return dirMap.at(a); })
                           .fold(
                               [](auto &&ini, auto &&a) {
                                   ini.first  += a.first;
                                   ini.second += a.second;
                                   return ini;
                               },
                               std::make_pair(0, 0));
    targetCoord.first  = std::abs(targetCoord.first);
    targetCoord.second = std::abs(targetCoord.second);

    auto ans = targetCoord.second + (std::max(0, targetCoord.first - targetCoord.second) / 2);
    return ans;
}
int day11_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    auto                     d_ctre    = ctre::search<R"(\w+)">;
    std::vector<std::string> inputVect = incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(oneStr, d_ctre);

    // Hex grid => pure vertical movements are by 2. 'Diagonal' movements are by 1 both vertically and horizontally.
    std::unordered_map<std::string, std::pair<int, int>> dirMap;
    dirMap.emplace("n", std::make_pair(-2, 0));
    dirMap.emplace("ne", std::make_pair(-1, 1));
    dirMap.emplace("se", std::make_pair(1, 1));
    dirMap.emplace("s", std::make_pair(2, 0));
    dirMap.emplace("sw", std::make_pair(1, -1));
    dirMap.emplace("nw", std::make_pair(-1, -1));

    int  ans         = 0;
    auto targetCoord = flux::ref(inputVect)
                           .map([&](auto &&a) { return dirMap.at(a); })
                           .fold(
                               [&](auto &&ini, auto &&a) {
                                   ini.first  += a.first;
                                   ini.second += a.second;
                                   ans =
                                       std::max(ans, std::abs(ini.second) +
                                                         (std::max(0, std::abs(ini.first) - std::abs(ini.second)) / 2));
                                   return ini;
                               },
                               std::make_pair(0, 0));
    return ans;
}

} // namespace AOC2017