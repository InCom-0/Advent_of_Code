#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

#include "AOC_2016_day_1.h"



namespace AOC2016 {
    int day1_1(std::string dataFile) {
        std::vector<std::vector<std::string>> inp = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(.\d+)">);

        /*
        Fun stuff with a simple bitfield ...

        Doing some sort of bitpacking would be a complete overkill for this.
        */
        struct BF {
            unsigned int bf: 2 = 0;
        };
        auto zipped = flux::ref(inp[0])
                                        .map ([] (auto &&a) {
                                            auto replacement = std::pair<BF,int>();
                                            if (a[0] == 'R') replacement.first.bf = 1;
                                            else replacement.first.bf = 3;
                                            replacement.second = std::stoi(a.substr(1));
                                            return replacement;
                                        })
                                        .to<std::vector<std::pair<BF, int>>>();

        BF last;
        for (auto &onePair: zipped) {
            last.bf = last.bf + onePair.first.bf;
            if (last.bf > 1) onePair.second *= -1;
            onePair.first.bf = last.bf % 2;
        }

        int NS = flux::ref(zipped)
                .filter([](auto &&a) { return a.first.bf == 0; })
                .fold([](auto &&a, auto &&b) { return a + b.second; }, 0);

        int WE = flux::ref(zipped)
                .filter([](auto &&a) { return a.first.bf == 1; })
                .fold([](auto &&a, auto &&b) { return a + b.second; }, 0);


        return abs(NS) + abs(WE);
    }

    int day1_2(std::string dataFile) {
        std::vector<std::vector<std::string>> inp = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(.\d+)">);

        struct BF {
            unsigned int bf: 2 = 0;
        };
        auto zipped = flux::ref(inp[0])
                                        .map ([] (auto &&a) {
                                            auto replacement = std::pair<BF,int>();
                                            if (a[0] == 'R') replacement.first.bf = 1;
                                            else replacement.first.bf = 3;
                                            replacement.second = std::stoi(a.substr(1));
                                            return replacement;
                                        })
                                        .to<std::vector<std::pair<BF, int>>>();

        BF last;
        for (auto &onePair: zipped) {
            last.bf = last.bf + onePair.first.bf;
            if (last.bf > 1) onePair.second *= -1;
            onePair.first.bf = last.bf % 2 == 0;
        }
        std::vector<std::vector<bool> > map(1001, std::vector<bool>(1001, false));

        int row = 500;
        int col = 500;

        for (auto &onePair: zipped) {
            int incOrDec = (onePair.second < 0) ? -1 : 1;
            if (onePair.first.bf == 0) {
                for (int i = row; i != row + onePair.second; i = i + incOrDec) {
                    if (map[i][col]) {
                        return (i + col - 1000);
                    }
                    map[i][col] = true;
                }
                row += onePair.second;
            } else {
                for (int i = col; i != col + onePair.second; i = i + incOrDec) {
                    if (map[row][i]) {
                        return (row + i - 1000);
                    }
                    map[row][i] = true;
                }
                col += onePair.second;
            }
        }
        return INT_MIN;
    }
}