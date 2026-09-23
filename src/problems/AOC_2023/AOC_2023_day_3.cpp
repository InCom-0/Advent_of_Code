#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>
#include <ranges>
#include <string>


namespace AOC2023 {

size_t
day3_1(std::string dataFile) {

    auto w_ctre = ctre::search<R"(.+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFile(dataFile, w_ctre).front();

    constexpr std::array<std::pair<int, int>, 8> kDirs8{
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};


    size_t      accuRes = 0uz;
    std::string cur;
    cur.reserve(16);

    for (size_t lineID = 1uz; lineID < (input.size() - 1); ++lineID) {
        for (size_t charID = 1uz; charID < (input[lineID].size() - 1); ++charID) {
            if (input[lineID][charID] != '.' && (input[lineID][charID] < '0' || input[lineID][charID] > '9')) {
                for (auto const [ychng, xchng] : kDirs8) {

                    int xTar = charID + xchng;
                    cur.clear();

                    while (xTar >= 0 && xTar < input[lineID + ychng].size() && input[lineID + ychng][xTar] >= '0' &&
                           input[lineID + ychng][xTar] <= '9') {
                        cur.push_back(input[lineID + ychng][xTar]);
                        input[lineID + ychng][xTar] = '.';
                        xTar--;
                    }
                    std::ranges::reverse(cur);

                    if (not cur.empty()) {
                        xTar = charID + xchng + 1;
                        while (xTar >= 0 && xTar < input[lineID + ychng].size() && input[lineID + ychng][xTar] >= '0' &&
                               input[lineID + ychng][xTar] <= '9') {
                            cur.push_back(input[lineID + ychng][xTar]);
                            input[lineID + ychng][xTar] = '.';
                            xTar++;
                        }
                    }

                    if (not cur.empty()) { accuRes += std::stoull(cur); }
                }
            }
        }
    }


    return accuRes;
}


size_t
day3_2(std::string dataFile) {

    auto w_ctre = ctre::search<R"(.+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFile(dataFile, w_ctre).front();

    constexpr std::array<std::pair<int, int>, 8> kDirs8{
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};


    size_t      accuRes = 0uz;
    std::string cur;
    cur.reserve(16);

    for (size_t lineID = 1uz; lineID < (input.size() - 1); ++lineID) {
        for (size_t charID = 1uz; charID < (input[lineID].size() - 1); ++charID) {
            if (input[lineID][charID] == '*') {

                size_t parts   = 0uz;
                size_t locAccu = 1uz;
                for (auto const [ychng, xchng] : kDirs8) {

                    int xTar = charID + xchng;
                    cur.clear();

                    while (xTar >= 0 && xTar < input[lineID + ychng].size() && input[lineID + ychng][xTar] >= '0' &&
                           input[lineID + ychng][xTar] <= '9') {
                        cur.push_back(input[lineID + ychng][xTar]);
                        xTar--;
                    }
                    std::ranges::reverse(cur);

                    if (not cur.empty()) {
                        parts++;
                        xTar = charID + xchng + 1;
                        while (xTar >= 0 && xTar < input[lineID + ychng].size() && input[lineID + ychng][xTar] >= '0' &&
                               input[lineID + ychng][xTar] <= '9') {
                            cur.push_back(input[lineID + ychng][xTar]);
                            input[lineID + ychng][xTar] = '.';
                            xTar++;
                        }

                        locAccu *= std::stoull(cur);
                    }
                }
                if (parts == 2uz) { accuRes += locAccu; }
            }
        }
    }


    return accuRes;
}

} // namespace AOC2023