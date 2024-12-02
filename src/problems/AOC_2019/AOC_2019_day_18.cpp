#include <AOC_2019_day_17.h>
#include <cstdint>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <incom_seq.h>
#include <vector>


namespace AOC2019 {
long long day18_1(std::string dataFile) {
    auto scafMap = day17_0(dataFile);

    // Find the rover location and its direction
    std::pair<size_t, size_t> rovLoc;
    struct Dir {
        unsigned int dir : 2;
    } rovDir;

    for (int row = 0; row < scafMap.size(); ++row) {
        for (int col = 0; col < scafMap.at(row).size(); ++col) {
            if (scafMap[row][col] == '<') { rovDir.dir = 3; }
            else if (scafMap[row][col] == '>') { rovDir.dir = 1; }
            else if (scafMap[row][col] == 'v') { rovDir.dir = 2; }
            else if (scafMap[row][col] == '^') { rovDir.dir = 0; }
            else { continue; }
            rovLoc = std::make_pair(row, col);
        }
    }

    // Helper variables to deal with turning directions in other functions
    constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    constexpr std::array<char, 3> const               dirMap{'L', '_', 'R'};

    // Simple type for storing the instructions + its helper
    using Robo_Instr         = std::pair<char, int>;
    auto robo_instr_2_string = [&](Robo_Instr const &ri) {
        std::string res;
        res.push_back(ri.first);
        res.push_back(',');
        res.append(std::to_string(ri.second));
        res.push_back(',');
        return res;
    };

    // 'Explore' the maze and build the 'vector of instructions' along the way
    std::vector<Robo_Instr> onePieceInstructions;
    auto                    buildInstructions = [&]() -> bool {
        // Find out whether to turn left or right (or end of maze)
        auto getTurnDir = [&]() -> int {
            rovDir.dir--;
            if (scafMap[rovLoc.first + dirs[rovDir.dir].front()][rovLoc.second + dirs[rovDir.dir].back()] == '#') {
                rovDir.dir++;
                return -1;
            }
            rovDir.dir += 2;
            if (scafMap[rovLoc.first + dirs[rovDir.dir].front()][rovLoc.second + dirs[rovDir.dir].back()] == '#') {
                rovDir.dir--;
                return 1;
            }
            else { return 0; }
        };
        int newDir = getTurnDir();
        if (newDir == 0) { return false; }
        rovDir.dir += newDir;

        int steps = 0;
        while (scafMap[rovLoc.first + dirs[rovDir.dir].front()][rovLoc.second + dirs[rovDir.dir].back()] == '#') {
            rovLoc.first  += dirs[rovDir.dir].front();
            rovLoc.second += dirs[rovDir.dir].back();
            steps++;
        }
        onePieceInstructions.push_back({dirMap.at(newDir + 1), steps});
        return true;
    };

    while (buildInstructions()) {}

    // auto map_OfSubseq = incom::seq::build_map_uniqueSubSeq2startPos(onePieceInstructions, 2);

    auto filter_forUnusableSubseq = [](auto const &subSeq) -> bool {
        size_t accu = 0;
        for (auto &instr : subSeq) { accu += (3 + std::to_string(instr.second).size()); }
        return (accu - 1) < 21;
    };

    std::vector testVec1{2, 3, 4, 5, 1, 2, 3, 2, 3, 4, 5, 1};

    // auto res = incom::seq::solvers::solve_seqFromRepUniqueSubseq<decltype(onePieceInstructions),
    //                                                              decltype(filter_forUnusableSubseq), 2>(
    //     onePieceInstructions, filter_forUnusableSubseq, 1, SIZE_MAX, 1, 3);

    auto res2 =
        incom::seq::solvers::solve_seqFromRepUniqueSubseq<decltype(testVec1), SIZE_MAX>(testVec1, 3, 1, 1, 3, 2);

    return -999;
}

long long day18_2(std::string dataFile) {


    return -999;
}
} // namespace AOC2019