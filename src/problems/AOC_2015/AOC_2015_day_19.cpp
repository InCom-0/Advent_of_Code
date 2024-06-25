#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {

int day19_1(std::string dataFile, std::string input) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return -1;

    std::string oneStr;
    std::vector<std::pair<std::string, std::string> > changes;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
        bg = re.get_end_position();
        changes.push_back(
            std::pair<std::string, std::string>(re.to_string(), ctre::search<R"(\w+)">(bg, oneStr.end()).to_string()));
    }
    std::vector<std::string> stringsForDecrementing;

    auto add2Left = flux::ref(changes)
            .filter([](auto &&a) { return a.second.ends_with(a.first); })
            .map([](auto &&a) {
                return std::pair<std::string, std::string>(
                    a.first, a.second.substr(0, a.second.size() - a.first.size()));
            });

    flux::ref(changes)
            .filter([](auto &&a) { return a.second.starts_with(a.first); })
            .map([](auto &&a) { return std::pair<std::string, std::string>(a.first, a.second.substr(a.first.size())); })
            .for_each([&](auto &&a) {
                add2Left.for_each([&](auto &&a2l) {
                    if (a.second == a2l.second) stringsForDecrementing.push_back(a.first + a2l.first);
                });
            });

    auto stringsForIncrementing = flux::ref(changes)
            .map([](auto &&a) { return a.first; })
            .dedup()
            .map([&](auto &&a) {
                return std::pair<std::string, int>(
                    a, flux::ref(changes).count_if([&](auto &&b) { return a == b.first; }));
            })
            .to<std::vector<std::pair<std::string, int> > >();
    int ans = 0;
    flux::ref(stringsForIncrementing).for_each([&](auto &&a) {
        size_t foundPos = -1;
        int existenceCount = -1;
        do {
            existenceCount++;
            foundPos = input.find(a.first, foundPos + 1);
        } while (foundPos != std::string::npos);
        ans += (existenceCount * a.second);
    });

    flux::ref(stringsForDecrementing).for_each([&](auto &&a) {
        size_t foundPos = -1;
        int existenceCount = -1;
        do {
            existenceCount++;
            foundPos = input.find(a, foundPos + 1);
        } while (foundPos != std::string::npos);
        ans -= existenceCount;
    });

    return ans;
}

int day19_3(std::string dataFile, std::string input) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return -1;

    std::string oneStr;
    std::vector<std::pair<std::string, std::string> > changes;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
        bg = re.get_end_position();
        changes.push_back(
            std::pair<std::string, std::string>(re.to_string(), ctre::search<R"(\w+)">(bg, oneStr.end()).to_string()));
    }

    int replacements = 0;
    int changeToTryID = 0;
    int posToReplace;

    while (input != "e") {
        posToReplace = input.find(changes[changeToTryID].second);
        if (posToReplace != std::string::npos) {
            input.replace(posToReplace, changes[changeToTryID].second.size(), changes[changeToTryID].first);
            replacements++;
        }
        changeToTryID++;
        if (changeToTryID == changes.size()) changeToTryID = 0;
    }


    return replacements;
}
}