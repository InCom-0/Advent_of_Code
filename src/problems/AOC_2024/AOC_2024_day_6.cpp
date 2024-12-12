#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <utility>


namespace AOC2024 {
namespace incc = incom::commons;
long long day6_1(std::string dataFile) {

    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    int curRow = 0;
    int curCol = 0;
    for (; curRow < input.size(); ++curRow) {
        bool quit = false;
        for (curCol = 0; curCol < input[curRow].size(); ++curCol) {
            if (input[curRow][curCol] == '^') {
                quit = true;
                break;
            }
        }
        if (quit) { break; }
    }
    struct OneDir {
        unsigned int one : 2;
    } od{0};

    input[curRow][curCol] = '.';
    long long counter     = 0;

    while (curRow >= 0 && curRow < input.size() && curCol >= 0 && curCol < input.front().size()) {
        if (input[curRow][curCol] == '.') { counter++; }
        input[curRow][curCol] = '-';

        if (curRow + dirs[od.one].front() < 0 || curRow + dirs[od.one].front() == input.size()) { break; }
        if (curCol + dirs[od.one].back() < 0 || curCol + dirs[od.one].back() == input.front().size()) { break; }
        if (input[curRow + dirs[od.one].front()][curCol + dirs[od.one].back()] == '#') { od.one++; }
        curRow += dirs[od.one].front();
        curCol += dirs[od.one].back();
    }

    return counter;
}

long long day6_2(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    using LOC = std::array<int, 2>;
    LOC curloc{0, 0};

    for (; curloc.front() < input.size(); ++curloc.front()) {
        bool quit = false;
        for (curloc.back() = 0; curloc.back() < input[curloc.front()].size(); ++curloc.back()) {
            if (input[curloc.front()][curloc.back()] == '^') {
                quit = true;
                break;
            }
        }
        if (quit) { break; }
    }
    struct OneDir {
        unsigned int one : 2;
    } od{0};


    input[curloc.front()][curloc.back()] = '.';
    long long counter                    = 0;

    auto boundsCheck = [&](int const row, int const col) {
        return (row >= 0 && row < input.size() && col >= 0 && col < input.front().size());
    };

    ankerl::unordered_dense::set<std::pair<LOC, unsigned int>, incom::commons::XXH3Hasher> visited;

    auto traceBack = [&]() -> bool {
        // Make local copies of relevant data
        auto   visited_loc = visited;
        OneDir od_loc      = od;
        LOC    curLoc_loc  = curloc;

        // Mark new obstacle
        input[curLoc_loc.front() + dirs[od_loc.one].front()][curLoc_loc.back() + dirs[od_loc.one].back()] = '#';

        while (true) {
            if (visited_loc.contains({curLoc_loc, od_loc.one + 0})) { return true; }
            else if (not boundsCheck(curLoc_loc.front() + dirs[od_loc.one].front(),
                                     curLoc_loc.back() + dirs[od_loc.one].back())) {
                return false;
            }
            else if (input[curLoc_loc.front() + dirs[od_loc.one].front()]
                          [curLoc_loc.back() + dirs[od_loc.one].back()] == '#') {
                visited_loc.emplace(curLoc_loc, od_loc.one + 0);
                od_loc.one++;
            }
            else {
                visited_loc.emplace(curLoc_loc, od_loc.one + 0);
                curLoc_loc.front() += dirs[od_loc.one].front();
                curLoc_loc.back()  += dirs[od_loc.one].back();
            }
        }
    };


    while (boundsCheck(curloc.front() + dirs[od.one].front(), curloc.back() + dirs[od.one].back())) {

        if (input[curloc.front() + dirs[od.one].front()][curloc.back() + dirs[od.one].back()] == '#') {
            visited.emplace(curloc, od.one + 0);
            input[curloc.front()][curloc.back()] = '-';
            od.one++;
            continue;
        }
        else {
            if (input[curloc.front() + dirs[od.one].front()][curloc.back() + dirs[od.one].back()] == '.') {
                if (traceBack()) { counter++; }
                input[curloc.front() + dirs[od.one].front()][curloc.back() + dirs[od.one].back()] = '.';
            }
            visited.emplace(curloc, od.one + 0);
            input[curloc.front()][curloc.back()]  = '-';
            curloc.front()                       += dirs[od.one].front();
            curloc.back()                        += dirs[od.one].back();
        }
    }

    return counter;
}
} // namespace AOC2024