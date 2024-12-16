#include <ankerl/unordered_dense.h>
#include <bits/ranges_algo.h>
#include <bitset>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <utility>


namespace AOC2024 {
namespace incc = incom::commons;
long long day12_1(std::string dataFile) {

    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    auto boundsCheck = [&](size_t const &row, size_t const &col) -> bool {
        return (row >= 0 && row < input.size() && col >= 0 && col < input[row].size());
    };
    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};


    ankerl::unordered_dense::map<size_t, std::pair<size_t, size_t>, incc::XXH3Hasher> regionData;
    std::vector dp(input.size(), std::vector(input.front().size(), std::optional<size_t>()));

    auto solve = [&, ID = 0uz](size_t row, size_t col) mutable -> bool {
        if (dp[row][col].has_value()) { return false; }
        else {
            char chrOfFirst      = input[row][col];
            dp[row][col]         = ID;
            size_t regionSize    = 1;
            size_t perimeterSize = 0;

            auto rec_solve_inside = [&](this auto const &self, size_t const &row, size_t const &col) -> void {
                for (auto const &dr : dirs) {
                    size_t row_loc = row + dr.front();
                    size_t col_loc = col + dr.back();
                    if (not boundsCheck(row_loc, col_loc) || input[row_loc][col_loc] != chrOfFirst) { perimeterSize++; }
                    else if (dp[row_loc][col_loc].has_value()) { continue; }
                    else {
                        dp[row_loc][col_loc] = ID;
                        regionSize++;
                        self(row_loc, col_loc); // Recursive call
                    }
                }
            };
            rec_solve_inside(row, col);
            regionData.insert({ID, {regionSize, perimeterSize}});
            ID++;
            return true;
        }
    };

    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) { solve(row, col); }
    }

    return std::ranges::fold_left(regionData, 0uz,
                                  [](auto &&ini, auto &item) { return ini + item.second.first * item.second.second; });
}

long long day12_2(std::string dataFile) {

    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    auto boundsCheck = [&](size_t const &row, size_t const &col) -> bool {
        return (row >= 0 && row < input.size() && col >= 0 && col < input[row].size());
    };
    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    ankerl::unordered_dense::map<size_t, std::pair<size_t, size_t>, incc::XXH3Hasher> regionData;
    std::vector dp(input.size(), std::vector(input.front().size(), std::optional<size_t>()));
    std::vector dp_fence(input.size(), std::vector(input.front().size(), std::bitset<4>()));

    std::vector dpLimit(input.size(), std::vector(input.front().size(), bool(false)));

    auto prep = [&, ID = 0uz](size_t row, size_t col) mutable -> bool {
        size_t regionSize    = 1;
        size_t perimeterSize = 0;

        if (dp[row][col].has_value()) { return false; }
        else {
            dp[row][col]         = ID;
            char chrOfFirst      = input[row][col];
            auto rec_prep_inside = [&](this auto const &self, size_t const &row, size_t const &col) -> void {
                // Create a bitset of fences
                for (int i = 0; auto const &dr : dirs) {
                    size_t row_loc = row + dr.front();
                    size_t col_loc = col + dr.back();
                    if (not boundsCheck(row_loc, col_loc) || input[row_loc][col_loc] != chrOfFirst) {
                        dp_fence[row][col][i] = true;
                    }
                    i++;
                }

                for (int i = 0; auto const &dr : dirs) {
                    size_t row_loc = row + dr.front();
                    size_t col_loc = col + dr.back();

                    if (not boundsCheck(row_loc, col_loc) || input[row_loc][col_loc] != chrOfFirst) { perimeterSize++; }
                    else if (dp[row_loc][col_loc].has_value()) { continue; }
                    else {
                        regionSize++;
                        dp[row_loc][col_loc] = ID;
                        self(row_loc, col_loc);
                    }
                }
            };
            rec_prep_inside(row, col);
            regionData.insert({ID, {regionSize, perimeterSize}});
            ID++;
            return true;
        }
    };

    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) { prep(row, col); }
    }

    std::vector<long long> removerOfLongerSides(regionData.size(), 0);
    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) {
            if (row + 1 < dp.size()) {
                if (dp[row][col] == dp[row + 1][col]) {
                    removerOfLongerSides[dp[row][col].value()] -= (dp_fence[row][col] & dp_fence[row + 1][col]).count();
                }
            }
            if (col + 1 < dp[row].size()) {
                if (dp[row][col] == dp[row][col + 1]) {
                    removerOfLongerSides[dp[row][col].value()] -= (dp_fence[row][col] & dp_fence[row][col + 1]).count();
                }
            }
        }
    }

    return std::ranges::fold_left(regionData, 0uz, [&](auto &&ini, auto &item) {
        return ini + item.second.first * (item.second.second + removerOfLongerSides[item.first]);
    });
}
} // namespace AOC2024