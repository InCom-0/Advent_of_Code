#include <fstream>

#include <incom_commons.h>
#include <bits/ranges_algo.h>
#include <ctre.hpp>

#include <flux.hpp>

namespace AOC2017 {
int day2_1(std::string dataFile) {
    auto ctr = ctre::search<R"(\d+)">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    std::vector<std::vector<int>> input;

    while (std::getline(iStream, oneStr)) {
        auto bg  = oneStr.begin();
        auto end = oneStr.end();
        input.push_back(std::vector<int>());

        while (auto re = ctr(bg, end)) {
            input.back().push_back(re.to_number());
            bg = re.get_end_position();
        }
    }

    auto res = flux::ref(input).fold(
        [](auto &&ini_1, auto &&a) {
            auto min_max = flux::ref(a).minmax([](auto &&aa, auto &&bb) { return aa < bb; });
            return ini_1 + (min_max->max - min_max->min);
        },
        0);
    return res;
}
int day2_2(std::string dataFile) {
    auto ctr = ctre::search<R"(\d+)">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    std::vector<std::vector<int>> input;

    while (std::getline(iStream, oneStr)) {
        auto bg  = oneStr.begin();
        auto end = oneStr.end();
        input.push_back(std::vector<int>());

        while (auto re = ctr(bg, end)) {
            input.back().push_back(re.to_number());
            bg = re.get_end_position();
        }
    }
    for (auto &oneLine : input) { std::ranges::sort(oneLine, std::less()); }

    auto res = flux::ref(input).fold(
        [](auto &&ini_1, auto &&a) {
            for (int i = 0; i < a.size(); ++i) {
                for (int j = i + 1; j < a.size(); ++j) {
                    if (a[j] % a[i] == 0) { return (ini_1 + (a[j] / a[i])); }
                }
            }
            return ini_1 + 0;
        },
        0);
    return res;
}
} // namespace AOC2017