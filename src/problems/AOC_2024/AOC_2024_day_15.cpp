#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>


namespace AOC2024 {
namespace incc = incom::commons;
long long day15_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    std::vector<std::string> maze;
    std::vector<int>         instructions;

    for (auto const &inpLine : input) {
        if (inpLine.size() > input.front().size()) {
            for (auto const &chr : inpLine) {
                if (chr == '^') { instructions.push_back(0); }
                else if (chr == '>') { instructions.push_back(1); }
                else if (chr == 'v') { instructions.push_back(2); }
                else if (chr == '<') { instructions.push_back(3); }
                else { assert(false); }
            }
        }
        else { maze.push_back(inpLine); }
    }

    std::pair<int, int> curRoboPos;
    std::pair<int, int> tempTarget;
    for (int row = 0; row < maze.size(); ++row) {
        for (int col = 0; col < maze[row].size(); ++col) {
            if (maze[row][col] == '@') {
                curRoboPos.first  = row;
                curRoboPos.second = col;
                maze[row][col]    = '.';
            }
        }
    }

    for (auto const &oneInstr : instructions) {
        tempTarget = curRoboPos;

        auto rec_boxMoveBy = [&](this auto const &self) -> std::optional<int> {
            tempTarget.first  += dirs[oneInstr].front();
            tempTarget.second += dirs[oneInstr].back();

            if (maze[tempTarget.first][tempTarget.second] == '.') { return 1; }
            else if (maze[tempTarget.first][tempTarget.second] == 'O') {
                // recursive call
                return self().and_then([](auto const &val) -> std::optional<int> { return val + 1; });
            }
            else { return std::nullopt; }
        };

        auto executeRoboMove = [&]() {
            auto moveBy = rec_boxMoveBy();
            // If we can move boxes somehow (or there is a free space in the direction of robo movement)
            if (moveBy.has_value()) {
                // Switch first box into the free position at the end
                std::swap(maze[curRoboPos.first + dirs[oneInstr].front()][curRoboPos.second + dirs[oneInstr].back()],
                          maze[curRoboPos.first + (dirs[oneInstr].front() * moveBy.value())]
                              [curRoboPos.second + (dirs[oneInstr].back() * moveBy.value())]);
                // Set new robot position (former box or was empty to begin with)
                curRoboPos.first  += dirs[oneInstr].front();
                curRoboPos.second += dirs[oneInstr].back();
            }
        };
        executeRoboMove();
    }


    size_t accu = 0;
    for (int row = 0; row < maze.size(); ++row) {
        for (int col = 0; col < maze[row].size(); ++col) {
            if (maze[row][col] == 'O') { accu += (row * 100) + col; }
        }
    }
    return accu;
}

long long day15_2(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    std::vector<std::string> maze_old;
    std::vector<int>         instructions;

    for (auto const &inpLine : input) {
        if (inpLine.size() > input.front().size()) {
            for (auto const &chr : inpLine) {
                if (chr == '^') { instructions.push_back(0); }
                else if (chr == '>') { instructions.push_back(1); }
                else if (chr == 'v') { instructions.push_back(2); }
                else if (chr == '<') { instructions.push_back(3); }
                else { assert(false); }
            }
        }
        else { maze_old.push_back(inpLine); }
    }

    std::pair<int, int>      curRoboPos;
    std::vector<std::string> maze(maze_old.size(), std::string());

    for (int row = 0; row < maze_old.size(); ++row) {
        for (int col = 0; col < maze_old[row].size(); ++col) {
            if (maze_old[row][col] == 'O') {
                maze[row].push_back('[');
                maze[row].push_back(']');
            }
            else if (maze_old[row][col] == '@') {
                curRoboPos.first  = row;
                curRoboPos.second = col * 2;
                maze[row].push_back('@');
                maze[row].push_back('.');
            }
            else {
                maze[row].push_back(maze_old[row][col]);
                maze[row].push_back(maze_old[row][col]);
            }
        }
    }

    for (auto const &oneInstr : instructions) {
        using ankSet = ankerl::unordered_dense::set<std::pair<int, int>, incc::XXH3Hasher>;
        incc::doubleBuffer<ankSet>       set_db;
        std::vector<std::pair<int, int>> validsToMove;

        auto rec_boxMoveByVer = [&](this auto const &self) -> std::optional<int> {
            while (not set_db.getCurrent().empty()) {
                for (auto const &setItem : set_db.getCurrent()) {
                    if (maze[setItem.first + dirs[oneInstr].front()][setItem.second + dirs[oneInstr].back()] == '#') {
                        return std::nullopt; // Absolutely no moves anywhere, exits instantly
                    }
                    else if (maze[setItem.first + dirs[oneInstr].front()][setItem.second + dirs[oneInstr].back()] ==
                             '[') {
                        set_db.getNext().insert(
                            {setItem.first + dirs[oneInstr].front(), setItem.second + dirs[oneInstr].back()});
                        if (oneInstr == 0 || oneInstr == 2) {
                            set_db.getNext().insert(
                                {setItem.first + dirs[oneInstr].front(), setItem.second + dirs[oneInstr].back() + 1});
                        }
                    }
                    else if (maze[setItem.first + dirs[oneInstr].front()][setItem.second + dirs[oneInstr].back()] ==
                             ']') {
                        if (oneInstr == 0 || oneInstr == 2) {
                            set_db.getNext().insert(
                                {setItem.first + dirs[oneInstr].front(), setItem.second + dirs[oneInstr].back() - 1});
                        }
                        set_db.getNext().insert(
                            {setItem.first + dirs[oneInstr].front(), setItem.second + dirs[oneInstr].back()});
                    }
                    else {}
                    validsToMove.push_back(setItem);
                }
                set_db.getCurrent().clear();
                set_db.swapBuffers();
            }
            // Move all the boxes by swapping ... do this from the 'back', so it will be done one 'level at a time'
            // thereby keeping the previous order
            for (auto const &oneLoc : std::views::reverse(validsToMove)) {
                std::swap(maze[oneLoc.first][oneLoc.second],
                          maze[oneLoc.first + dirs[oneInstr].front()][oneLoc.second + dirs[oneInstr].back()]);
            }
            return 1; // Some moves were made (ie. not early exit), therefore the robot must move also
        };

        auto executeRoboMove = [&]() {
            // Clear helpers from previous iteration
            set_db.getCurrent().clear();
            set_db.getNext().clear();
            validsToMove.clear();

            // Set the 'source' meaining the currrent roboPos in set_db
            set_db.getCurrent().insert({curRoboPos.first, curRoboPos.second});

            // If we can move boxes somehow (or there is a free space in the direction of robo movement)
            if (rec_boxMoveByVer().has_value()) {
                // Set new robot position (former box or was empty to begin with)
                curRoboPos.first  += dirs[oneInstr].front();
                curRoboPos.second += dirs[oneInstr].back();
            }
        };
        executeRoboMove();
    }


    size_t accu = 0;
    for (int row = 0; row < maze.size(); ++row) {
        for (int col = 0; col < maze[row].size(); ++col) {
            if (maze[row][col] == '[') { accu += (row * 100) + col; }
        }
    }
    return accu;
}
} // namespace AOC2024