#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <bits/ranges_algo.h>
#include <charconv>
#include <climits>
#include <cstdint>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <vector>


namespace AOC2024 {
namespace incc = incom::commons;
long long day9_1(std::string dataFile) {
    using namespace std::literals;
    auto             any_ctre = ctre::search<R"(.+)">;
    auto             input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front().front();
    std::vector<int> data(input.size(), INT_MIN);

    for (int i = 0; auto const &chr : input) {
        std::from_chars(input.data() + i, input.data() + i + 1, data[i]);
        i++;
    }

    long long curBlockID         = 0;
    size_t    fromBack_pos       = ((data.size() % 2) == 1 ? input.size() - 1 : input.size() - 2);
    int       fromBack_remaining = data[fromBack_pos];
    int       fromBack_ID        = data.size() / 2 + data.size() % 2 - 1;

    auto getFromBack = [&]() {
        if (fromBack_remaining == 0) {
            fromBack_pos       -= 2;
            fromBack_remaining  = data[fromBack_pos];
            fromBack_ID--;
        }
        fromBack_remaining--;
        return fromBack_ID;
    };

    size_t fromFront_pos       = 0;
    int    fromFront_remaining = data[fromFront_pos];
    int    fromFront_ID        = 0;

    size_t posInRes = 0;
    size_t res      = 0;
    while (fromFront_pos < fromBack_pos) {
        while (fromFront_remaining > 0) {
            res += fromFront_ID * posInRes++;
            fromFront_remaining--;
        }
        fromFront_ID++;

        // Prep the moving from back
        fromFront_pos++;
        fromFront_remaining = data[fromFront_pos];

        // Do the moving from back
        while (fromFront_remaining > 0) {
            res += getFromBack() * posInRes++;
            fromFront_remaining--;
        }
        // Prep the next cycle
        fromFront_pos++;
        fromFront_remaining = data[fromFront_pos];
    }

    while (fromBack_remaining > 0) {
        res += fromBack_ID * posInRes++;
        fromBack_remaining--;
    }
    return res;
}

long long day9_2(std::string dataFile) {
    using namespace std::literals;
    auto             any_ctre = ctre::search<R"(.+)">;
    auto             input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front().front();
    std::vector<int> data(input.size(), INT_MIN);

    for (int i = 0; auto const &chr : input) {
        std::from_chars(input.data() + i, input.data() + i + 1, data[i]);
        i++;
    }

    std::vector<std::pair<size_t, size_t>> id_2_maxSpace_inRes;

    size_t fromBack_pos       = ((data.size() % 2) == 1 ? input.size() - 1 : input.size() - 2);
    int    fromBack_remaining = data[fromBack_pos];
    int    fromBack_ID        = data.size() / 2 + data.size() % 2 - 1;

    size_t fromFront_pos       = 0;
    int    fromFront_remaining = data[fromFront_pos];
    int    fromFront_ID        = 0;

    size_t posInRes = 0;
    size_t res      = 0;

    std::vector<size_t> fromFrontRes;
    std::vector<size_t> fromBackRes;


    while (fromFront_pos < data.size()) {
        fromFront_remaining = data[fromFront_pos];
        res                 = 0;

        while (fromFront_remaining-- > 0) { res += fromFront_ID * posInRes++; }
        fromFrontRes.push_back(res);
        fromFront_ID++;

        // Prep the moving from back
        fromFront_pos++;
        if (fromFront_pos >= data.size()) { break; }
        id_2_maxSpace_inRes.push_back({posInRes, data[fromFront_pos]});
        posInRes += data[fromFront_pos];

        // Prep the next cycle
        fromFront_pos++;
    }

    std::vector<size_t> lastFoundAt{SIZE_MAX, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int cycle = 0;
    while (fromBack_pos > 1) {
        fromBack_remaining = data[fromBack_pos];
        res                = 0;

        if (lastFoundAt[data[fromBack_pos]] <= id_2_maxSpace_inRes.size() - cycle) {

            auto iter = std::ranges::find_if(id_2_maxSpace_inRes.begin() + lastFoundAt[data[fromBack_pos]],
                                             id_2_maxSpace_inRes.end() - cycle,
                                             [&](auto const &item) { return item.second >= data[fromBack_pos]; });
            if (iter != id_2_maxSpace_inRes.end() - cycle) {
                lastFoundAt[data[fromBack_pos]] = iter - id_2_maxSpace_inRes.begin();

                while (fromBack_remaining-- > 0) {
                    res += (fromBack_ID * iter->first++);
                    iter->second--;
                }
                fromBackRes.push_back(res);
                fromFrontRes[fromBack_ID] = 0; // If we can move from the back we set that of in fromFrontRes to 0
            }
        }

        // Prep next
        fromBack_ID--;
        fromBack_pos -= 2;
        cycle++;
    }
    return std::ranges::fold_left_first(fromFrontRes, std::plus()).value() +
           std::ranges::fold_left_first(fromBackRes, std::plus()).value();
}
} // namespace AOC2024