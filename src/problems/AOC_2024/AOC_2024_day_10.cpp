#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;
long long day10_1(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    std::vector data(input.size(), std::vector(input.front().size(), INT_MIN));

    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].size(); ++j) {
            std::from_chars(input[i].data() + j, input[i].data() + j + 1, data[i][j]);
        }
    }
    using setOfPeaks = ankerl::unordered_dense::set<std::array<int, 2>, incc::XXH3Hasher>;
    std::vector resStorage(data.size(), std::vector(data.front().size(), std::optional<setOfPeaks>()));

    std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    auto rec_solve = [&](this auto const &self, std::array<int, 2> const &startPos) -> setOfPeaks & {
        auto &resOfThisPos = resStorage[startPos.front()][startPos.back()];
        if (resOfThisPos.has_value()) { return resOfThisPos.value(); }
        else if (data[startPos.front()][startPos.back()] == 9) {
            resOfThisPos = setOfPeaks();
            resOfThisPos->insert(std::array<int, 2>{startPos.front(), startPos.back()});
            return resOfThisPos.value();
        }

        else {
            resOfThisPos = setOfPeaks();
            std::array<int, 2> tmpPos;
            for (auto const &dr : dirs) {
                tmpPos.front() = startPos.front() + dr.front();
                tmpPos.back()  = startPos.back() + dr.back();
                // If bounds check is ok
                if (tmpPos.front() >= 0 && tmpPos.front() < data.size() && tmpPos.back() >= 0 &&
                    tmpPos.back() < data[tmpPos.front()].size()) {
                    if (data[tmpPos.front()][tmpPos.back()] == data[startPos.front()][startPos.back()] + 1) {
                        auto setFromOneDir = self(tmpPos); // Recursive call
                        for (auto const &oneResFromOneDir : setFromOneDir) { resOfThisPos->insert(oneResFromOneDir); }
                    }
                }
            }
            return resOfThisPos.value();
        }
    };

    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) {
            if (data[row][col] == 0) {
                std::array<int, 2> curStart{row, col};
                rec_solve(curStart);
            }
        }
    }
    long long accu = 0;
    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].size(); ++j) {
            if (data[i][j] == 0) { accu += resStorage[i][j].value().size(); }
        }
    }

    return accu;
}

long long day10_2(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    std::vector data(input.size(), std::vector(input.front().size(), INT_MIN));

    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].size(); ++j) {
            std::from_chars(input[i].data() + j, input[i].data() + j + 1, data[i][j]);
        }
    }
    // using setNumOfPeaks = ankerl::unordered_dense::set<int, incc::XXH3Hasher>;
    std::vector resStorage(data.size(), std::vector(data.front().size(), std::optional<int>()));

    std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    auto rec_solve = [&](this auto const &self, std::array<int, 2> const &startPos) -> int & {
        auto &resOfThisPos = resStorage[startPos.front()][startPos.back()];
        if (resOfThisPos.has_value()) { return resOfThisPos.value(); }
        else if (data[startPos.front()][startPos.back()] == 9) {
            resOfThisPos = 1;
            return resOfThisPos.value();
        }

        else {
            resOfThisPos = 0;
            std::array<int, 2> tmpPos;
            for (auto const &dr : dirs) {
                tmpPos.front() = startPos.front() + dr.front();
                tmpPos.back()  = startPos.back() + dr.back();
                // If bounds check is ok
                if (tmpPos.front() >= 0 && tmpPos.front() < data.size() && tmpPos.back() >= 0 &&
                    tmpPos.back() < data[tmpPos.front()].size()) {
                    if (data[tmpPos.front()][tmpPos.back()] == data[startPos.front()][startPos.back()] + 1) {
                        resOfThisPos.value() += self(tmpPos); // Recursive call
                    }
                }
            }
            return resOfThisPos.value();
        }
    };

    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) {
            if (data[row][col] == 0) {
                std::array<int, 2> curStart{row, col};
                rec_solve(curStart);
            }
        }
    }
    long long accu = 0;
    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].size(); ++j) {
            if (data[i][j] == 0) { accu += resStorage[i][j].value(); }
        }
    }

    return accu;
}
} // namespace AOC2024