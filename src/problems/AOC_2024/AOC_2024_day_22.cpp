#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <array>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <utility>
#include <vector>


namespace AOC2024 {
namespace incc = incom::commons;
long long day22_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    std::vector<size_t> data;
    for (auto const &item : input) { data.push_back(std::stoull(item)); }

    constexpr size_t c_moduloBy = 16'777'216;

    // the database of previously calculated values;
    std::vector<size_t> calculated(c_moduloBy, SIZE_MAX);

    auto get_nextSecret = [&](size_t curSecret) -> void {
        if (calculated[curSecret] != SIZE_MAX) { return; }
        else {
            size_t &src = calculated[curSecret];
            curSecret   = ((curSecret * 64) xor curSecret) % c_moduloBy;
            curSecret   = ((curSecret / 32) xor curSecret) % c_moduloBy;
            curSecret   = ((curSecret * 2048) xor curSecret) % c_moduloBy;
            src         = curSecret; // update the database
        }
    };

    for (size_t i = 0; i < c_moduloBy; ++i) { get_nextSecret(i); }

    std::vector<size_t> res;
    for (auto &curNum : data) {
        for (size_t i = 0; i < 2000; ++i) { curNum = calculated[curNum]; }
    }


    return std::ranges::fold_left_first(data, std::plus()).value_or(0uz);
}

long long day22_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    std::vector<size_t> data;
    for (auto const &item : input) { data.push_back(std::stoull(item)); }

    constexpr size_t c_moduloBy = 16'777'216;

    // the database of previously calculated values;
    std::vector<size_t> calculated(c_moduloBy, SIZE_MAX);

    auto get_nextSecret = [&](size_t curSecret) -> size_t {
        if (calculated[curSecret] == SIZE_MAX) {
            size_t &src = calculated[curSecret];
            curSecret   = ((curSecret * 64) xor curSecret) % c_moduloBy;
            curSecret   = ((curSecret / 32) xor curSecret) % c_moduloBy;
            curSecret   = ((curSecret * 2048) xor curSecret) % c_moduloBy;
            src         = curSecret; // update the database
            return src;
        }
        else { return calculated[curSecret]; }
    };

    for (size_t i = 0; i < c_moduloBy; ++i) { get_nextSecret(i); }

    std::vector<size_t>                 res;
    std::vector<std::vector<long long>> prices;
    for (int i = 0; auto &curNum : data) {
        prices.push_back(std::vector<long long>(1, data.at(i) - ((data.at(i) / 10) * 10)));
        i++;
        for (size_t j = 0; j < 2000; ++j) {
            curNum = calculated[curNum];
            prices.back().push_back(curNum - ((curNum / 10) * 10));
        }
    }

    ankerl::unordered_dense::map<std::array<long long, 4>, size_t, incc::XXH3Hasher> resMap;

    for (auto const &oneMonkeyPrices : prices) {
        ankerl::unordered_dense::map<std::array<long long, 4>, size_t, incc::XXH3Hasher> oneMap;

        auto priceChangesVec =
            std::views::adjacent_transform<2>(oneMonkeyPrices, [](auto &&a, auto &&b) { return b - a; }) |
            std::ranges::to<std::vector<long long>>();

        std::array<long long, 4> toInsert{LLONG_MIN, priceChangesVec[0], priceChangesVec[1], priceChangesVec[2]};

        for (size_t i = 4; i < oneMonkeyPrices.size(); ++i) {
            std::ranges::rotate(toInsert, toInsert.begin() + 1);
            toInsert.back() = priceChangesVec[i - 1];
            oneMap.insert({toInsert, oneMonkeyPrices[i]});
        }
        for (auto const &oneMap_item : oneMap) {
            auto eleIter           = resMap.insert({oneMap_item.first, 0});
            eleIter.first->second += oneMap_item.second;
        }
    }

    auto ans = std::ranges::max_element(resMap, [](auto &&a, auto &&b) { return a.second < b.second; });
    return ans->second;
}
} // namespace AOC2024