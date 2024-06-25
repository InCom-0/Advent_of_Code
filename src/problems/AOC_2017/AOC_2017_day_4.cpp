#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2017 {
    int day4_1(std::string dataFile) {
        std::vector<std::vector<std::string>> VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, ctre::search<R"(\w+)">);

        for (auto &line: VofV) std::sort(line.begin(),line.end(),std::less());
        return flux::ref(VofV)
                            .count_if([] (auto &&a) {
                                return flux::ref(a).dedup().count() == flux::ref(a).count();
                            });
    }

    int day4_2(std::string dataFile) {
        std::vector<std::vector<std::string>> VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, ctre::search<R"(\w+)">);

        for (auto &stringLine: VofV) {
            for (auto &str: stringLine) {
                std::sort(str.begin(),str.end(),std::less());
            }
        }
        for (auto &line: VofV) std::sort(line.begin(),line.end(),std::less());

        return flux::ref(VofV)
                            .count_if([] (auto &&a) {
                                return flux::ref(a).dedup().count() == flux::ref(a).count();
                            });
    }
}