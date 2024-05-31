#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"



namespace AOC2016 {
    int day3_1(std::string dataFile) {

        auto ctr = ctre::search<R"(\d+)">;
        auto inp = AOC_commons::parseInputUsingCTRE::processFile(dataFile, ctr, ctr, ctr);

        auto const ans = flux::zip(flux::ref(inp[0]), flux::ref(inp[1]), flux::ref(inp[2]))
                                .count_if([] (auto&& a) {
                                    std::vector<int> tmpStorage {std::stoi(std::get<0>(a)), std::stoi(std::get<1>(a)), std::stoi(std::get<2>(a))};
                                    int maxEle = flux::ref(tmpStorage)
                                                        .max([] (auto& aa, auto& bb) {return aa < bb;})
                                                        .value_or(0);

                                    int sum = flux::ref(tmpStorage).fold([] (auto&& aaa, auto&& bbb) {return aaa+bbb;},0);

                                    return sum > (2*maxEle);
                                });
        return ans;
    }

    int day3_2(std::string dataFile) {
        auto ctr = ctre::search<R"(\d+)">;
        auto inp = AOC_commons::parseInputUsingCTRE::processFile(dataFile, ctr, ctr, ctr);

        return flux::ref(inp).fold([] (auto&& z, auto&& y) {
            return z +
                flux::ref(y)
                .chunk(3)
                .count_if([] (auto&& a) {
                    std::vector<int> tmpStorage;
                    for (auto& strDim: a) tmpStorage.push_back(std::stoi(strDim));

                    int maxEle = flux::ref(tmpStorage)
                                        .max([] (auto& aa, auto& bb) {return aa < bb;})
                                        .value_or(0);

                    int sum = flux::ref(tmpStorage).fold([] (auto&& aaa, auto&& bbb) {return aaa+bbb;},0);

                    return sum > (2*maxEle);
                });
        }, 0);
    }


}