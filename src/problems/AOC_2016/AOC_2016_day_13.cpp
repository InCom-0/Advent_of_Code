#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

/*
Adapted a kind of Djikstra algorithm for this problem.
Of course there is no 'true' graph, but the maze could be thought of as a graph where all edges are of length 1.
Also, somehow the code seems needlessly complicated

Could be optimized by adapting a version of A* algorithm ... at least I think so.
It is highly probable that the 'location' in queue that is geometrically the closest to the target is more likely to
reach it first. (thus possibly eliminating needless computation of other branches). Possible adverse effect on extreme
edge cases.
*/

namespace AOC2016 {
int day13_1(std::string dataFile, int favNumber, const int targetCol, const int targetRow) {
    struct oneLocation_t {
        int  numOfSteps = INT_MAX;
        bool reached    = false;
    };

    std::vector<std::vector<oneLocation_t>> mazeMinDists(2, std::vector<oneLocation_t>(2, {INT_MAX}));
    auto                                    posIsWall = [favN = favNumber](const int &row, const int &col) {
        int  val = col * col + 3 * col + 2 * row * col + row + row * row + favN;
        auto isWall = flux::from_range(std::format("{0:b}", val)).count_if([](const auto &a) { return a == '1'; }) % 2;
        if (isWall) { return true; }
        else { return false; }
    };

    for (int i = 0; i < mazeMinDists.size(); ++i) {
        for (int j = 0; j < mazeMinDists[0].size(); ++j) { mazeMinDists[i][j].reached = posIsWall(i, j); }
    }
    mazeMinDists[1][1].numOfSteps = 0; // Start position is 1:1.
    mazeMinDists[1][1].reached    = true;

    auto checkExistenceOrCreate = [&](const int &row, const int &col) {
        if (mazeMinDists.size() == row) { mazeMinDists.push_back(std::vector<oneLocation_t>()); }
        for (int i = mazeMinDists[row].size(); i <= col; ++i) {
            mazeMinDists[row].push_back(oneLocation_t{INT_MAX, posIsWall(row, i)});
        }
    };

    std::queue<std::pair<int, int>> queue;
    queue.push({1, 1});
    std::vector<std::pair<int, int>> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (not queue.empty()) {
        auto [rowSel, colSel] = queue.front();
        queue.pop();
        for (auto &[dirRow, dirCol] : directions) {
            const int newRow = rowSel + dirRow;
            const int newCol = colSel + dirCol;

            if (newRow >= 0 && newCol >= 0) {
                checkExistenceOrCreate(newRow, newCol);
                if (mazeMinDists[newRow][newCol].reached == false) {
                    mazeMinDists[newRow][newCol].numOfSteps = mazeMinDists[rowSel][colSel].numOfSteps + 1;
                    queue.push({newRow, newCol});
                    mazeMinDists[newRow][newCol].reached = true;
                    if (newRow == targetRow && newCol == targetCol) { return mazeMinDists[newRow][newCol].numOfSteps; }
                }
            }
        }
    }
    return -999;
}

int day13_2(std::string dataFile, int favNumber, const int maxSteps) {
    struct oneLocation_t {
        int  numOfSteps = INT_MAX;
        bool reached    = false;
    };

    std::vector<std::vector<oneLocation_t>> mazeMinDists(2, std::vector<oneLocation_t>(2, {INT_MAX}));
    auto                                    posIsWall = [favN = favNumber](const int &row, const int &col) {
        int  val = col * col + 3 * col + 2 * row * col + row + row * row + favN;
        auto isWall = flux::from_range(std::format("{0:b}", val)).count_if([](const auto &a) { return a == '1'; }) % 2;
        if (isWall) { return true; }
        else { return false; }
    };

    for (int i = 0; i < mazeMinDists.size(); ++i) {
        for (int j = 0; j < mazeMinDists[0].size(); ++j) { mazeMinDists[i][j].reached = posIsWall(i, j); }
    }
    mazeMinDists[1][1].numOfSteps = 0; // Start position is 1:1.
    mazeMinDists[1][1].reached    = true;

    int reachedLocations = 1;

    auto checkExistenceOrCreate = [&](const int &row, const int &col) {
        if (mazeMinDists.size() == row) { mazeMinDists.push_back(std::vector<oneLocation_t>()); }
        for (int i = mazeMinDists[row].size(); i <= col; ++i) {
            mazeMinDists[row].push_back(oneLocation_t{INT_MAX, posIsWall(row, i)});
        }
    };

    std::queue<std::pair<int, int>> queue;
    queue.push({1, 1});
    std::vector<std::pair<int, int>> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (not queue.empty()) {
        auto [rowSel, colSel] = queue.front();
        queue.pop();
        for (auto &[dirRow, dirCol] : directions) {
            const int newRow = rowSel + dirRow;
            const int newCol = colSel + dirCol;

            if (newRow >= 0 && newCol >= 0) {
                checkExistenceOrCreate(newRow, newCol);
                if (mazeMinDists[newRow][newCol].reached == false) {
                    mazeMinDists[newRow][newCol].numOfSteps = mazeMinDists[rowSel][colSel].numOfSteps + 1;
                    mazeMinDists[newRow][newCol].reached    = true;

                    if (mazeMinDists[newRow][newCol].numOfSteps > maxSteps) { return reachedLocations; }
                    queue.push({newRow, newCol});
                    reachedLocations++;
                }
            }
        }
    }
    return -999;
}
} // namespace AOC2016
