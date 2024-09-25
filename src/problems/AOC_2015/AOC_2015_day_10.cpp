#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
/*
VERSION 1:
Best performant
*/
int day10_1(std::string dataFile, int rep) {
    auto inputVofV = incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(\d+)">);
    auto lambda    = [&](this auto const &self, std::string const lamInput) -> int {
        int         identicalCount = 0;
        char        prev           = lamInput[0];
        std::string outString      = "";

        for (auto const &itemChar : lamInput) {
            if (prev != itemChar) {
                outString += std::to_string(identicalCount);
                outString.push_back(prev);
                identicalCount = 1;
            }
            else { identicalCount++; }
            prev = itemChar;
        }
        outString += std::to_string(identicalCount);
        outString.push_back(prev);

        rep--;
        if (rep > 0) { return self(outString); }
        else { return outString.size(); }
    };

    return lambda(inputVofV.front().front());
}

/*
VERSION 2:
On paper this should be as performant as version 1 ...
Naturaly the culprit must be the 'chunk_by' ... but don't know why.

Might be worth investigating at some point to understand it better.
*/
int day10_2(std::string dataFile, int rep) {
    auto inputVofV = incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(\d+)">);
    auto lambda    = [&](this auto const &self, std::string const &lamInput) -> int {
        std::string outString = "";

        flux::ref(lamInput).chunk_by([](auto &&a, auto &&b) { return a == b; }).for_each([&](auto &&a) {
            outString += std::to_string(a.size());
            outString.push_back(a.read_at(a.first()));
        });
        rep--;
        if (rep > 0) { return self(outString); }
        else { return outString.size(); }
    };

    return lambda(inputVofV.front().front());
}

/*
VERSION 3:
Worst performance, probbably because it is creating too many strings, concatenating them, etc.
*/
int day10_3(std::string dataFile, int rep) {
    auto inputVofV = incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(\d+)">);

    auto lambda = [&](this auto const &self, std::string const &lamInput) -> int {
        auto test = flux::ref(lamInput)
                        .chunk_by([](auto &a, auto &b) { return a == b; })
                        .map([&](auto &&a) { return std::to_string(a.size()) + a.read_at(a.first()); })
                        .flatten();

        std::string outString = flux::to<std::string>(test);

        rep--;
        if (rep > 0) { return self(outString); }
        else { return outString.size(); }
    };

    return lambda(inputVofV.front().front());
}
} // namespace AOC2015