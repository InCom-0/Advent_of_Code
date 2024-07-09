#include "flux/op/zip.hpp"
#include <algorithm>
#include <cassert>
#include <ctre/wrapper.hpp>
#include <fstream>

#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <utility>


namespace AOC2018 {
long long day2_1(std::string dataFile) {

    auto all_ctre = ctre::search<R"(.+)">;
    auto input_V  = AOC_commons::parseInputUsingCTRE::processFile(dataFile, all_ctre).front();

    for (auto &line : input_V) { std::ranges::sort(line, std::less()); }

    auto res = flux::ref(input_V).fold(
        [&](auto &&init, auto &&a) {
            auto chunked  = flux::ref(a).chunk_by([](auto &&aa, auto &&bb) { return aa == bb; });
            init.first         += chunked.any([&](auto &&aaa) { return aaa.size() == 2; });
            init.second         += chunked.any([&](auto &&aaa) { return aaa.size() == 3; });
            return init;
        },
        std::make_pair(0, 0));
    return res.first * res.second;
}

std::string day2_2(std::string dataFile) {
    auto all_ctre = ctre::search<R"(.+)">;
    auto input_V  = AOC_commons::parseInputUsingCTRE::processFile(dataFile, all_ctre).front();

    for (int i = 0; i < input_V.size(); ++i) {
        for (int j = i + 1; j < input_V.size(); ++j) {
            unsigned int accu = flux::zip(flux::ref(input_V[i]), flux::ref(input_V[j]))
                                    .fold([](auto &&init, auto &&a) { return init + (a.first != a.second); }, 0);
            if (accu == 1) {
                return flux::zip(flux::ref(input_V[i]), flux::ref(input_V[j]))
                    .fold(
                        [](auto &&init, auto &&a) {
                            if (a.first != a.second) { return init; }

                            init.push_back(a.first);
                            return init;
                        },
                        std::string());
            }
        }
    }
    assert((void("No boxes with just 1 differing letter in their ID"), false));
    std::unreachable();
}
} // namespace AOC2018