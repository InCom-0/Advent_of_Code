#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;
long long day20_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();


    for (auto &line : input) {
        line.push_back('#');
        line.push_back('#');
        std::ranges::rotate(line, line.end() - 1);
    }
    input.insert(input.begin(), std::string(input.front().size(), '#'));
    input.push_back(std::string(input.front().size(), '#'));


    std::pair<size_t, size_t> startPos;
    std::pair<size_t, size_t> endPos;

    for (size_t row = 0; row < input.size(); ++row) {
        for (size_t col = 0; col < input[row].size(); ++col) {
            if (input[row][col] == 'S') { startPos = {row, col}; }
            else if (input[row][col] == 'E') { endPos = {row, col}; }
        }
    }

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::vector               stepsOnRoute(input.size(), std::vector(input.front().size(), LLONG_MIN));
    std::pair<size_t, size_t> thisPos             = startPos;
    stepsOnRoute[startPos.first][startPos.second] = 0;

    long long curStepCount = 0;
    while (thisPos != endPos) {
        for (auto const &dr : dirs) {
            if (input[thisPos.first + dr.front()][thisPos.second + dr.back()] != '#' &&
                stepsOnRoute[thisPos.first + dr.front()][thisPos.second + dr.back()] == LLONG_MIN) {
                stepsOnRoute[thisPos.first + dr.front()][thisPos.second + dr.back()]  = ++curStepCount;
                thisPos.first                                                        += dr.front();
                thisPos.second                                                       += dr.back();
                break;
            }
        }
    }

    thisPos                = startPos;
    long long cheatCounter = 0;
    while (thisPos != endPos) {
        for (auto const &dr : dirs) {
            // Plus '2' to counter the cost of 'cheating', plus '99' to get 'at least 100ps saved'
            if (stepsOnRoute[thisPos.first + dr.front() * 2][thisPos.second + dr.back() * 2] >
                stepsOnRoute[thisPos.first][thisPos.second] + 2 + 99) {
                cheatCounter++;
            }
        }
        for (auto const &dr : dirs) {
            if (stepsOnRoute[thisPos.first + dr.front()][thisPos.second + dr.back()] >
                stepsOnRoute[thisPos.first][thisPos.second]) {
                thisPos.first  += dr.front();
                thisPos.second += dr.back();
                break;
            }
        }
    }
    return cheatCounter;
}

long long day20_2(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    std::pair<size_t, size_t> startPos;
    std::pair<size_t, size_t> endPos;

    for (size_t row = 0; row < input.size(); ++row) {
        for (size_t col = 0; col < input[row].size(); ++col) {
            if (input[row][col] == 'S') { startPos = {row, col}; }
            else if (input[row][col] == 'E') { endPos = {row, col}; }
        }
    }

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::vector               stepsOnRoute(input.size(), std::vector(input.front().size(), LLONG_MIN));
    std::pair<size_t, size_t> thisPos             = startPos;
    stepsOnRoute[startPos.first][startPos.second] = 0;

    long long curStepCount = 0;
    while (thisPos != endPos) {
        for (auto const &dr : dirs) {
            if (input[thisPos.first + dr.front()][thisPos.second + dr.back()] != '#' &&
                stepsOnRoute[thisPos.first + dr.front()][thisPos.second + dr.back()] == LLONG_MIN) {
                stepsOnRoute[thisPos.first + dr.front()][thisPos.second + dr.back()]  = ++curStepCount;
                thisPos.first                                                        += dr.front();
                thisPos.second                                                       += dr.back();
                break;
            }
        }
    }
    auto boundsCheck = [&](int row, int col) -> bool {
        if (row >= 0 && col >= 0 && row < input.size() && col < input[row].size()) { return true; }
        else { return false; }
    };

    thisPos                = startPos;
    long long cheatCounter = 0;
    while (thisPos != endPos) {
        for (int rowChng = -20; rowChng <= 20; ++rowChng) {
            for (int colChng = (-20 + std::abs(rowChng)); colChng <= (20 - std::abs(rowChng)); ++colChng) {
                if (boundsCheck(thisPos.first + rowChng, thisPos.second + colChng)) {
                    if (stepsOnRoute[thisPos.first + rowChng][thisPos.second + colChng] >
                        stepsOnRoute[thisPos.first][thisPos.second] + std::abs(rowChng) + std::abs(colChng)+99) {
                        cheatCounter++;
                    }
                }
            }
        }
        for (auto const &dr : dirs) {
            if (stepsOnRoute[thisPos.first + dr.front()][thisPos.second + dr.back()] >
                stepsOnRoute[thisPos.first][thisPos.second]) {
                thisPos.first  += dr.front();
                thisPos.second += dr.back();
                break;
            }
        }
    }
    return cheatCounter;
}
} // namespace AOC2024