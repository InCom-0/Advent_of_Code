#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;

long long day8_1(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    ankerl::unordered_dense::map<char, std::vector<std::pair<int, int>>, incc::XXH3Hasher> antMap;

    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) {
            if (input[row][col] != '.') {
                auto it = antMap.try_emplace(input[row][col], std::vector<std::pair<int, int>>());
                it.first->second.push_back(std::make_pair(row, col));
            }
        }
    }

    auto testCoord = [&](std::pair<int, int> const &toTest) -> bool {
        return (toTest.first >= 0 && toTest.first < input.size() && toTest.second >= 0 &&
                toTest.second < input[toTest.first].size());
    };

    ankerl::unordered_dense::set<std::pair<int, int>, incc::XXH3Hasher> antiNodes_loc;

    std::ranges::for_each(antMap, [&](auto &antLocations) {
        if (antLocations.second.size() < 2) { return 0uz; }

        int                 row_dif, col_dif;
        std::pair<int, int> attempt;

        do {
            row_dif = antLocations.second[0].first - antLocations.second[1].first;
            col_dif = antLocations.second[0].second - antLocations.second[1].second;
            attempt = {antLocations.second[0].first + row_dif, antLocations.second[0].second + col_dif};
            if (testCoord(attempt)) { antiNodes_loc.insert(attempt); }

            attempt = {antLocations.second[1].first - row_dif, antLocations.second[1].second - col_dif};
            if (testCoord(attempt)) { antiNodes_loc.insert(attempt); }

        } while (std::ranges::next_permutation(antLocations.second, std::less()).found);
        return 1uz;
    });

    return antiNodes_loc.size();
}

long long day8_2(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    ankerl::unordered_dense::map<char, std::vector<std::pair<int, int>>, incc::XXH3Hasher> antMap;
    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) {
            if (input[row][col] != '.') {
                auto it = antMap.try_emplace(input[row][col], std::vector<std::pair<int, int>>());
                it.first->second.push_back(std::make_pair(row, col));
            }
        }
    }

    auto testCoord = [&](std::pair<int, int> const &toTest) -> bool {
        return (toTest.first >= 0 && toTest.first < input.size() && toTest.second >= 0 &&
                toTest.second < input[toTest.first].size());
    };

    ankerl::unordered_dense::set<std::pair<int, int>, incc::XXH3Hasher> antiNodes_loc;

    std::ranges::for_each(antMap, [&](auto &antLocations) {
        if (antLocations.second.size() < 2) { return; }

        int                                    row_dif, col_dif;
        std::pair<int, int>                    attempt;
        std::vector<std::pair<size_t, size_t>> antIndicesPairs;

        for (int i = 0; i < antLocations.second.size(); ++i) {
            for (int j = i + 1; j < antLocations.second.size(); ++j) { antIndicesPairs.push_back({i, j}); }
        }

        for (auto const &pr : antIndicesPairs) {
            row_dif = antLocations.second[pr.first].first - antLocations.second[pr.second].first;
            col_dif = antLocations.second[pr.first].second - antLocations.second[pr.second].second;

            attempt = {antLocations.second[pr.first].first, antLocations.second[pr.first].second};
            while (testCoord(attempt)) {
                antiNodes_loc.insert(attempt);
                attempt.first  += row_dif;
                attempt.second += col_dif;
            }

            attempt = {antLocations.second[pr.second].first, antLocations.second[pr.second].second};
            while (testCoord(attempt)) {
                antiNodes_loc.insert(attempt);
                attempt.first  -= row_dif;
                attempt.second -= col_dif;
            }
        }
    });

    return antiNodes_loc.size();
}
} // namespace AOC2024