#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <limits>


namespace AOC2025 {
size_t
day4_1(std::string dataFile) {
    auto anyCtre = ctre::search<R"(.+)">;
    auto VofV    = incom::commons::parseInputUsingCTRE::processFile(dataFile, anyCtre).front();

    std::vector<std::string> input;

    input.push_back(std::string(VofV.front().size() + 2, '.'));
    for (auto const &line : VofV) {
        input.push_back(std::string(1, '.'));
        input.back().append(line);
        input.back().push_back('.');
    }
    input.push_back(std::string(VofV.front().size() + 2, '.'));

    auto surLessThan4 = [&](size_t row, size_t col) -> bool {
        size_t counter = 0uz;
        if (input[row][col] != '@') { return false; }

        for (int r = row - 1; r < row + 2; ++r) {
            for (int c = col - 1; c < col + 2; ++c) {
                if (input[r][c] == '.') { counter++; }
            }
        }
        if (counter >= 5) { return true; }
        else { return false; }
    };

    size_t res = 0uz;
    for (size_t r = 1; r < input.size() - 1; ++r) {
        for (size_t c = 1; c < input.at(r).size() - 1; ++c) { res += surLessThan4(r, c); }
    }

    return res;
}
size_t
day4_2(std::string dataFile) {
    auto anyCtre = ctre::search<R"(.+)">;
    auto VofV    = incom::commons::parseInputUsingCTRE::processFile(dataFile, anyCtre).front();

    std::vector<std::string> input;

    input.push_back(std::string(VofV.front().size() + 2, '.'));
    for (auto const &line : VofV) {
        input.push_back(std::string(1, '.'));
        input.back().append(line);
        input.back().push_back('.');
    }
    input.push_back(std::string(VofV.front().size() + 2, '.'));

    auto surLessThan4 = [&](size_t row, size_t col) -> bool {
        size_t counter = 0uz;
        if (input[row][col] != '@') { return false; }

        for (int r = row - 1; r < row + 2; ++r) {
            for (int c = col - 1; c < col + 2; ++c) {
                if (input[r][c] == '.') { counter++; }
            }
        }
        if (counter >= 5) { return true; }
        else { return false; }
    };

    size_t iterRes = std::numeric_limits<size_t>::max();
    size_t res     = 0uz;

    while (iterRes > 0uz) {
        iterRes = 0uz;
        for (size_t r = 1; r < input.size() - 1; ++r) {
            for (size_t c = 1; c < input.at(r).size() - 1; ++c) {
                if (surLessThan4(r, c)) {
                    iterRes++;
                    input[r][c] = '.';
                };
            }
        }
        res += iterRes;
    }

    return res;
}
} // namespace AOC2025