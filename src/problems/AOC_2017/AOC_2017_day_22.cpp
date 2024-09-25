#include <deque>

#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2017 {
int day22_1(std::string dataFile, int iterations) {
    // CUSTOM LOCAL TYPES
    auto line_ctre = ctre::search<R"(.+)">;
    auto VofV      = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, line_ctre);
    struct BF {
        unsigned int dir : 2 = 0;
    } dir;

    // DATA PREP
    std::deque<std::deque<char>> map;
    for (auto &VofV_line : VofV) {
        map.push_back(std::deque<char>());
        for (auto &VofV_char : VofV_line.front()) { map.back().push_back(VofV_char); }
    }

    size_t curRow           = map.size() / 2;
    size_t curCol           = map.front().size() / 2;
    size_t infectionCounter = 0;

    // LAMBDA DEFINITIONS
    auto checkAndAddIfNeeded = [&]() {
        if (curRow == map.size() - 1) { map.emplace_back(std::deque<char>(map.back().size(), '.')); }
        else if (curRow == 0) {
            map.emplace_front(std::deque<char>(map.front().size(), '.'));
            curRow++;
        }
        else if (curCol == map[curRow].size() - 1) {
            for (auto &mapLine : map) { mapLine.emplace_back('.'); }
        }
        else if (curCol == 0) {
            for (auto &mapLine : map) { mapLine.emplace_front('.'); }
            curCol++;
        }
    };
    auto evaluatePos = [&]() {
        if (map[curRow][curCol] == '#') { dir.dir++; }
        else { dir.dir--; }

        if (map[curRow][curCol] == '.') {
            map[curRow][curCol] = '#';
            infectionCounter++;
        }
        else { map[curRow][curCol] = '.'; }
    };
    auto advancePos = [&]() {
        curRow = curRow + ((dir.dir % 2 == 0) ? (((int)dir.dir) - 1) : 0);
        curCol = curCol + ((dir.dir % 2 != 0) ? (((int)dir.dir) * (-1) + 2) : 0);
    };

    // MAIN LOGIC
    for (int it = 0; it < iterations; ++it) {
        checkAndAddIfNeeded();
        evaluatePos();
        advancePos();
    }
    return infectionCounter;
}

// Initial attemp at part 2 using 'deque of deques' ... horrible performance for this type of thing
int day22_2(std::string dataFile, int iterations) {

    // CUSTOM LOCAL TYPES
    auto line_ctre = ctre::search<R"(.+)">;
    auto VofV      = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, line_ctre);
    struct BF {
        unsigned int dir : 2 = 0;
    } dir;

    // DATA PREP
    std::deque<std::deque<BF>> map;
    for (auto &VofV_line : VofV) {
        map.push_back(std::deque<BF>());
        for (auto &VofV_char : VofV_line.front()) {
            map.back().push_back(BF{(unsigned int)((bool)(VofV_char - '.') * 2)});
        }
    }

    size_t curRow           = map.size() / 2;
    size_t curCol           = map.front().size() / 2;
    size_t infectionCounter = 0;

    // LAMBDA DEFINITIONS
    auto checkAndAddIfNeeded = [&]() {
        if (curRow == map.size() - 1) { map.emplace_back(std::deque<BF>(map.back().size(), BF{0})); }
        else if (curRow == 0) {
            map.emplace_front(std::deque<BF>(map.front().size(), BF{0}));
            curRow++;
        }
        else if (curCol == map[curRow].size() - 1) {
            for (auto &mapLine : map) { mapLine.emplace_back(0); }
        }
        else if (curCol == 0) {
            for (auto &mapLine : map) { mapLine.emplace_front(0); }
            curCol++;
        }
    };
    auto evaluatePos = [&]() {
        dir.dir += (map[curRow][curCol].dir - 1); // the 4 states correspond to the directional changes perfectly
        if (map[curRow][curCol].dir == 1) { infectionCounter++; }

        map[curRow][curCol].dir++; // 4 states so a circular 2 bit type rotation => bitfield
    };
    auto advancePos = [&]() {
        curRow = curRow + ((dir.dir % 2 == 0) ? (((int)dir.dir) - 1) : 0);
        curCol = curCol + ((dir.dir % 2 != 0) ? (((int)dir.dir) * (-1) + 2) : 0);
    };

    // MAIN LOGIC
    for (int it = 0; it < iterations; ++it) {
        checkAndAddIfNeeded();
        evaluatePos();
        advancePos();
    }
    return infectionCounter;
}

/// Much better part 2 using a 'vector of vector' + rotating the vector into correct position when it grows.
/// Also uses some bitfield (dare I say) black magic for both tracking the 'direction' and tracking the state of each
/// 'node'.
int day22_3(std::string dataFile, int iterations) {
    // CUSTOM LOCAL TYPES
    auto line_ctre = ctre::search<R"(.+)">;
    auto VofV      = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, line_ctre);
    struct BF {
        unsigned int dir : 2 = 0;
    } dir;

    // DATA PREP
    std::vector<std::vector<BF>> map;
    for (auto &VofV_line : VofV) {
        map.push_back(std::vector<BF>());
        for (auto &VofV_char : VofV_line.front()) {
            map.back().push_back(BF{(unsigned int)((bool)(VofV_char - '.') * 2)});
        }
    }

    size_t curRow           = map.size() / 2;
    size_t curCol           = map.front().size() / 2;
    size_t infectionCounter = 0;

    // LAMBDA DEFINITIONS
    auto checkAndAddIfNeeded = [&]() {
        if (curRow == map.size() - 1 || curRow == 0) {
            map.resize(map.capacity() * 2, std::vector<BF>{map.back().size(), BF{0}});
            std::rotate(map.rbegin(), map.rbegin() + map.size() / 4, map.rend());
            curRow += map.size() / 4;
        }
        else if (curCol == map[curRow].size() - 1 || curCol == 0) {
            for (auto &mapLine : map) {
                mapLine.resize(mapLine.capacity() * 2, BF{0});
                std::rotate(mapLine.rbegin(), mapLine.rbegin() + mapLine.size() / 4, mapLine.rend());
            }
            curCol += map.front().size() / 4;
        }
    };
    auto evaluatePos = [&]() {
        dir.dir += (map[curRow][curCol].dir - 1); // the 4 states correspond to the directional changes perfectly
        if (map[curRow][curCol].dir == 1) { infectionCounter++; }

        map[curRow][curCol].dir++; // 4 states so a circular 2 bit type rotation => bitfield
    };
    auto advancePos = [&]() {
        curRow = curRow + ((dir.dir % 2 == 0) ? (((int)dir.dir) - 1) : 0);
        curCol = curCol + ((dir.dir % 2 != 0) ? (((int)dir.dir) * (-1) + 2) : 0);
    };

    // MAIN LOGIC
    for (int it = 0; it < iterations; ++it) {
        checkAndAddIfNeeded();
        evaluatePos();
        advancePos();
    }
    return infectionCounter;
}
} // namespace AOC2017