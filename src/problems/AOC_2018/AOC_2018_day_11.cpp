#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>
#include <utility>


namespace AOC2018 {
std::string day11_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    int  input  = std::stoi(AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front());

    std::vector<std::pair<int, int>> dirs{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    std::vector<std::vector<int>> cells(302, std::vector<int>(302, 0));
    std::vector<std::vector<int>> groupsByNine(304, std::vector<int>(304, 0));

    for (int row = 1; row < 301; ++row) {
        for (int col = 1; col < 301; ++col) {
            cells[row][col] = ((((((col + 10) * row) + input) * (col + 10)) % 1000) / 100) - 5;
            for (auto &oneDir : dirs) { groupsByNine[row + oneDir.first][col + oneDir.second] += cells[row][col]; }
        }
    }

    int                 max = INT_MIN;
    std::pair<int, int> res;
    for (int row = 2; row < 302; ++row) {
        for (int col = 2; col < 302; ++col) {
            if (groupsByNine[row][col] > max) {
                res.first  = row;
                res.second = col;
                max        = groupsByNine[row][col];
            }
        }
    }

    return std::to_string(res.second - 1) + ',' + std::to_string(res.first - 1);
}

// Heavy brute force solution ... not so nice. Test for this is DISABLED
std::string day11_2(std::string dataFile) {
    // CUSTOM DATA TYPES
    auto d_ctre = ctre::search<R"(\d+)">;
    int  input  = std::stoi(AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front());

    // DATA PREP
    std::vector vecOfMaps(301, std::vector<std::vector<int>>(301, std::vector<int>(301, 0)));

    for (int row = 1; row < 301; ++row) {
        for (int col = 1; col < 301; ++col) {
            vecOfMaps[1][row][col] = ((((((col + 10) * row) + input) * (col + 10)) % 1000) / 100) - 5;
        }
    }

    // LAMBDA DEFINITIONS
    std::vector<int> maxSoFar{INT_MIN, INT_MIN, INT_MIN, INT_MIN};

    auto computeMapRawAdd = [&](auto &&sqSize) {
        int mapSideSize2create = 302 - sqSize;
        for (int row = 1; row < mapSideSize2create; ++row) {
            for (int col = 1; col < mapSideSize2create; ++col) {
                vecOfMaps[sqSize][row][col] = vecOfMaps[sqSize - 1][row][col];

                for (int mpCol = col; mpCol < col + sqSize; ++mpCol) {
                    vecOfMaps[sqSize][row][col] += vecOfMaps[1][row + sqSize - 1][mpCol];
                }
                for (int mpRow = row; mpRow < row + sqSize - 1; ++mpRow) {
                    vecOfMaps[sqSize][row][col] += vecOfMaps[1][mpRow][col + sqSize - 1];
                }
                if (vecOfMaps[sqSize][row][col] > maxSoFar[2]) {
                    maxSoFar[2] = vecOfMaps[sqSize][row][col];
                    maxSoFar[0] = row;
                    maxSoFar[1] = col;
                    maxSoFar[3] = sqSize;
                }
            }
        }
    };
    auto computeMap2 = [&](int &sqSize) {
        int srcSq              = sqSize / 2;
        int mapSideSize2create = 302 - sqSize;
        for (int row = 1; row < mapSideSize2create; ++row) {
            for (int col = 1; col < mapSideSize2create; ++col) {
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row][col];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row][col + srcSq];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq][col];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq][col + srcSq];

                if (vecOfMaps[sqSize][row][col] > maxSoFar[2]) {
                    maxSoFar[2] = vecOfMaps[sqSize][row][col];
                    maxSoFar[0] = row;
                    maxSoFar[1] = col;
                    maxSoFar[3] = sqSize;
                }
            }
        }
    };
    auto computeMap3 = [&](int &sqSize) {
        int srcSq              = sqSize / 3;
        int mapSideSize2create = 302 - sqSize;
        for (int row = 1; row < mapSideSize2create; ++row) {
            for (int col = 1; col < mapSideSize2create; ++col) {
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row][col];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row][col + srcSq];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row][col + srcSq + srcSq];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq][col];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq][col + srcSq];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq][col + srcSq + srcSq];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq + srcSq][col];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq + srcSq][col + srcSq];
                vecOfMaps[sqSize][row][col] += vecOfMaps[srcSq][row + srcSq + srcSq][col + srcSq + srcSq];

                if (vecOfMaps[sqSize][row][col] > maxSoFar[2]) {
                    maxSoFar[2] = vecOfMaps[sqSize][row][col];
                    maxSoFar[0] = row;
                    maxSoFar[1] = col;
                    maxSoFar[3] = sqSize;
                }
            }
        }
    };


    // MAIN LOGIC
    for (int sqToCompute = 2; sqToCompute < 301; ++sqToCompute) {
        if (sqToCompute % 2 == 0) { computeMap2(sqToCompute); }
        else if (sqToCompute % 3 == 0) { computeMap3(sqToCompute); }
        else { computeMapRawAdd(sqToCompute); }
    }
    return std::to_string(maxSoFar[1]) + ',' + std::to_string(maxSoFar[0]) + ',' + std::to_string(maxSoFar[3]);
}

// Better solution with a 5 times speedup over the previous one.
std::string day11_3(std::string dataFile) {
    // CUSTOM TYPES
    auto d_ctre = ctre::search<R"(\d+)">;
    int  input  = std::stoi(AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front());

    // DATA PREP
    std::vector vecOfMaps(301, std::vector<std::vector<int>>(301, std::vector<int>(301, 0)));
    for (int row = 1; row < 301; ++row) {
        for (int col = 1; col < 301; ++col) {
            vecOfMaps[1][row][col] = ((((((col + 10) * row) + input) * (col + 10)) % 1000) / 100) - 5;
        }
    }

    // Computes 'right' and 'bottom' sides SUMS of each 'square' in the vecOfMaps
    // Indices are 0) row, 1) column of bottom right corner, 2) size of square in question.
    std::vector mapOfRightAndBottomSides(301, std::vector<std::vector<int>>(301, std::vector<int>(301, 0)));
    for (int i = 2; i < 301; ++i) {
        for (int start = 1; start < 300; ++start) {
            int prevHor = 0;
            int prevVer = 0;
            for (int end = start; end < start + i && end < 301; ++end) {
                mapOfRightAndBottomSides[i][end][end - start + 1] += prevHor + vecOfMaps[1][i][end];
                prevHor                                           = prevHor + vecOfMaps[1][i][end];

                mapOfRightAndBottomSides[end][i][end - start + 1] += prevVer + vecOfMaps[1][end][i];
                prevVer                                            = prevVer + vecOfMaps[1][end][i];
            }
        }
    }

    // LAMBDA DEFINITIONS
    std::vector<int> maxSoFar{INT_MIN, INT_MIN, INT_MIN, INT_MIN};

    auto computeMapRawAdd = [&](const auto &sqSize) {
        int mapSideSize2create = 302 - sqSize;
        for (int row = 1; row < mapSideSize2create; ++row) {
            for (int col = 1; col < mapSideSize2create; ++col) {
                vecOfMaps[sqSize][row][col]  = vecOfMaps[sqSize - 1][row][col];
                vecOfMaps[sqSize][row][col] += mapOfRightAndBottomSides[row + sqSize - 1][col + sqSize - 1][sqSize];
                vecOfMaps[sqSize][row][col] -= vecOfMaps[1][row + sqSize - 1][col + sqSize - 1];

                if (vecOfMaps[sqSize][row][col] > maxSoFar[2]) {
                    maxSoFar[2] = vecOfMaps[sqSize][row][col];
                    maxSoFar[0] = row;
                    maxSoFar[1] = col;
                    maxSoFar[3] = sqSize;
                }
            }
        }
    };

    // MAIN LOGIC
    for (int sqToCompute = 2; sqToCompute < 301; ++sqToCompute) {
        // if (sqToCompute % 10 == 0) { std::cout << sqToCompute << '\n'; }
        computeMapRawAdd(sqToCompute);
    }
    return std::to_string(maxSoFar[1]) + ',' + std::to_string(maxSoFar[0]) + ',' + std::to_string(maxSoFar[3]);
}


} // namespace AOC2018