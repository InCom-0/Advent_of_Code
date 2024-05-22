#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day6_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);

    std::string oneStr;
    std::string tempString;
    char chr;

    if (not iStream.is_open()) return -1;

    int accu = 0;

    std::vector<std::vector<int> > instructions;
    std::vector<int> instructionLine;

    while (std::getline(iStream, oneStr)) {
        if (oneStr[6] == 'n') instructionLine.push_back(1);
        else if (oneStr[6] == 'f') instructionLine.push_back(2);
        else instructionLine.push_back(0);

        flux::ref(oneStr)
                .chunk_by([](auto &&a, auto &&b) {
                    if (a == ' ' || a == ',' || b == ' ' || b == ',') return false;
                    return true;
                })
                .filter([&](auto &&a) {
                    chr = *a.front();
                    if (chr > 47 && chr < 58) return true;
                    return false;
                })
                .for_each([&](auto &&a) {
                    instructionLine.push_back(std::stoi(flux::to<std::string>(a)));
                    return;
                });

        instructions.push_back(instructionLine);
        instructionLine.clear();
    }

    iStream.clear();
    iStream.close();
    int length;
    int height;

    std::vector<std::bitset<1000> > dp(1000, std::bitset<1000>());
    std::bitset<1000> oneLine;
    std::bitset<1000> hlprLine;

    for (auto &oneInstruction: instructions) {
        length = oneInstruction[4] - oneInstruction[2] + 1;
        height = oneInstruction[3] - oneInstruction[1] + 1;

        if (oneInstruction[0] == 0) {
            for (int i = oneInstruction[2]; i < oneInstruction[2] + length; ++i) hlprLine.flip(i);

            for (int i = oneInstruction[1]; i < oneInstruction[1] + height; ++i) {
                oneLine = dp[i];
                oneLine.flip();
                oneLine &= hlprLine;

                hlprLine.flip();
                dp[i] = oneLine | (dp[i] & hlprLine);

                hlprLine.flip();
            }
        } else if (oneInstruction[0] == 1) {
            for (int i = oneInstruction[2]; i < oneInstruction[2] + length; ++i) oneLine.flip(i);
            for (int i = oneInstruction[1]; i < oneInstruction[1] + height; ++i) dp[i] |= oneLine;
        } else {
            oneLine.flip();
            for (int i = oneInstruction[2]; i < oneInstruction[2] + length; ++i) oneLine.flip(i);
            for (int i = oneInstruction[1]; i < oneInstruction[1] + height; ++i) dp[i] &= oneLine;
        }

        hlprLine.reset();
        oneLine.reset();
    }

    for (auto &line: dp) accu += line.count();

    return accu;
}

int day6_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);

    std::string oneStr;
    std::string tempString;
    char chr;

    if (not iStream.is_open()) return -1;

    int accu = 0;

    std::vector<std::vector<int> > instructions;
    std::vector<int> instructionLine;

    while (std::getline(iStream, oneStr)) {
        if (oneStr[6] == 'n') instructionLine.push_back(1);
        else if (oneStr[6] == 'f') instructionLine.push_back(2);
        else instructionLine.push_back(0);

        flux::ref(oneStr)
                .chunk_by([](auto &&a, auto &&b) {
                    if (a == ' ' || a == ',' || b == ' ' || b == ',') return false;
                    return true;
                })
                .filter([&](auto &&a) {
                    chr = *a.front();
                    if (chr > 47 && chr < 58) return true;
                    return false;
                })
                .for_each([&](auto &&a) {
                    instructionLine.push_back(std::stoi(flux::to<std::string>(a)));
                    return;
                });

        instructions.push_back(instructionLine);
        instructionLine.clear();
    }

    iStream.clear();
    iStream.close();
    int length;
    int height;
    int adder;

    std::vector<int> dp(1'000'000, 0);

    for (auto &oneInstruction: instructions) {
        length = oneInstruction[4] - oneInstruction[2] + 1;
        height = oneInstruction[3] - oneInstruction[1] + 1;

        if (oneInstruction[0] == 2) {
            for (int i = oneInstruction[1]; i < oneInstruction[1] + height; ++i) {
                for (int j = oneInstruction[2]; j < oneInstruction[2] + length; ++j) {
                    dp[i * 1000 + j] = std::max(dp[i * 1000 + j] - 1, 0);
                }
            }
        } else {
            adder = oneInstruction[0] == 0 ? 2 : 1;
            for (int i = oneInstruction[1]; i < oneInstruction[1] + height; ++i) {
                for (int j = oneInstruction[2]; j < oneInstruction[2] + length; ++j) {
                    dp[i * 1000 + j] = dp[i * 1000 + j] + adder;
                }
            }
        }
    }

    for (auto &item: dp) accu += item;

    return accu;
}
}