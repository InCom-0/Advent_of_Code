#include <cstdint>
#include <ctre/wrapper.hpp>
#include <fstream>

#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>


namespace AOC2018 {
unsigned int day3_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofV   = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::vector<int>> rectsBorders;
    for (auto &line : VofV) {
        rectsBorders.push_back({{std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3]), std::stoi(line[4])}});
        rectsBorders.back()[2] += (rectsBorders.back()[0]);
        rectsBorders.back()[3] += (rectsBorders.back()[1]);
    }
    std::vector<std::vector<int>> map = std::vector<std::vector<int>>(1000, std::vector<int>(1000, 0));

    for (auto &ob : rectsBorders) {
        for (int i = ob[1]; i < ob[3]; ++i) {
            for (int j = ob[0]; j < ob[2]; ++j) { map[i][j]++; }
        }
    }

    auto res = flux::ref(map).fold(
        [](auto &&init, auto &&a) {
            return init + flux::ref(a).fold([](auto &&init2, auto &&a) { return init2 + (bool)(a / 2); }, 0);
        },
        0);

    return res;
}

unsigned int day3_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofV   = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::vector<int>> rectsBorders;
    for (auto &line : VofV) {
        rectsBorders.push_back(
            {{std::stoi(line[0]), std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3]), std::stoi(line[4])}});
        rectsBorders.back()[3] += (rectsBorders.back()[1]);
        rectsBorders.back()[4] += (rectsBorders.back()[2]);
    }
    std::vector<std::vector<int>> map = std::vector<std::vector<int>>(1000, std::vector<int>(1000, 0));

    for (auto &ob : rectsBorders) {
        for (int i = ob[2]; i < ob[4]; ++i) {
            for (int j = ob[1]; j < ob[3]; ++j) { map[i][j]++; }
        }
    }

    for (auto &ob : rectsBorders) {
        int counter = 0;
        for (int i = ob[2]; i < ob[4]; ++i) {
            for (int j = ob[1]; j < ob[3]; ++j) {
                if (map[i][j] == 1) { counter++; }
            }
        }
        if (counter == ((ob[4] - ob[2]) * (ob[3] - ob[1]))) {return ob[0];}
    }
    return UINT32_MAX;
}
} // namespace AOC2018