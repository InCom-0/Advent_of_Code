#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>



namespace AOC2023 {

auto
day10_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto res    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();
    res.push_back(std::string(res.back().size(), '#'));
    res.push_back(std::string(res.back().size(), '#'));
    std::ranges::rotate(res, res.end() - 1);

    for (auto &line : res) {
        line.push_back('#');
        line.push_back('#');
        std::ranges::rotate(line, line.end() - 1);
    }
    return res;
}

auto
day10_1(std::string dataFile) {
    auto input                  = day10_0(dataFile);
    auto const [yStart, xStart] = [&]() {
        size_t yStart = 0uz;
        size_t xStart = 0uz;

        while (yStart < input.size()) {
            if (auto it = std::ranges::find(input.at(yStart), 'S'); it != input.at(yStart).end()) {
                return std::make_pair(yStart, static_cast<size_t>(it - input.at(yStart).begin()));
            }
            yStart++;
        }
        return std::make_pair(0uz, 0uz);
    }();


    struct Dirs {
        int yFrom = 0;
        int xFrom = 0;
        int yTo   = 0;
        int xTo   = 0;
    };

    std::array<Dirs, 256> mp{};
    mp.at('|') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 1, .xTo = 0};
    mp.at('-') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 0, .xTo = 1};
    mp.at('L') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('J') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = -1};
    mp.at('7') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 1, .xTo = 0};
    mp.at('F') = Dirs{.yFrom = 1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('.') = Dirs{.yFrom = 0, .xFrom = 0, .yTo = 0, .xTo = 0};

    auto convertFrom  = [&](char c) { return mp.at(c); };
    auto [yCur, xCur] = std::make_pair(yStart, xStart);


    Dirs curDir([&]() {
        std::pair<int, int>              r;
        std::vector<std::pair<int, int>> vec;
        if (Dirs d = mp.at(input[yCur - 1][xCur]); ((d.yFrom == 1 && d.xFrom == 0) || (d.yTo == 1 && d.xTo == 0))) {
            vec.push_back({-1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur - 1]); ((d.yFrom == 0 && d.xFrom == 1) || (d.yTo == 0 && d.xTo == 1))) {
            vec.push_back({0, -1});
        }
        if (Dirs d = mp.at(input[yCur + 1][xCur]); ((d.yFrom == -1 && d.xFrom == 0) || (d.yTo == -1 && d.xTo == 0))) {
            vec.push_back({1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur + 1]); ((d.yFrom == 0 && d.xFrom == -1) || (d.yTo == 0 && d.xTo == -1))) {
            vec.push_back({0, 1});
        }

        return Dirs{vec.begin()->first, (vec.begin())->second, (vec.begin() + 1)->first, (vec.begin() + 1)->second};
    }());


    size_t stepsCounter{};
    bool   switched = false;

    do {
        yCur = yCur + (switched ? curDir.yFrom : curDir.yTo);
        xCur = xCur + (switched ? curDir.xFrom : curDir.xTo);

        auto const &nextDirs = mp[input[yCur][xCur]];
        if ((((nextDirs.yTo * (-1)) == (switched ? curDir.yFrom : curDir.yTo)) &&
             ((nextDirs.xTo * (-1)) == (switched ? curDir.xFrom : curDir.xTo)))) {
            switched = true;
        }
        else { switched = false; }

        curDir = nextDirs;
        stepsCounter++;
    } while (not(yCur == yStart && xCur == xStart));

    return stepsCounter / 2;
}

auto
day10_2(std::string dataFile) {
    auto const input            = day10_0(dataFile);
    auto const [yStart, xStart] = [&]() {
        size_t yStart = 0uz;
        size_t xStart = 0uz;

        while (yStart < input.size()) {
            if (auto it = std::ranges::find(input.at(yStart), 'S'); it != input.at(yStart).end()) {
                return std::make_pair(yStart, static_cast<size_t>(it - input.at(yStart).begin()));
            }
            yStart++;
        }
        return std::make_pair(0uz, 0uz);
    }();


    struct Dirs {
        int yFrom = 0;
        int xFrom = 0;
        int yTo   = 0;
        int xTo   = 0;
    };

    auto [yCur, xCur] = std::make_pair(yStart, xStart);
    std::array<Dirs, 256> mp{};
    mp.at('|') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 1, .xTo = 0};
    mp.at('-') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 0, .xTo = 1};
    mp.at('L') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('J') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = -1};
    mp.at('7') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 1, .xTo = 0};
    mp.at('F') = Dirs{.yFrom = 1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('.') = Dirs{.yFrom = 0, .xFrom = 0, .yTo = 0, .xTo = 0};
    mp.at('S') = [&]() {
        std::pair<int, int>              r;
        std::vector<std::pair<int, int>> vec;
        if (Dirs d = mp.at(input[yCur - 1][xCur]); ((d.yFrom == 1 && d.xFrom == 0) || (d.yTo == 1 && d.xTo == 0))) {
            vec.push_back({-1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur - 1]); ((d.yFrom == 0 && d.xFrom == 1) || (d.yTo == 0 && d.xTo == 1))) {
            vec.push_back({0, -1});
        }
        if (Dirs d = mp.at(input[yCur + 1][xCur]); ((d.yFrom == -1 && d.xFrom == 0) || (d.yTo == -1 && d.xTo == 0))) {
            vec.push_back({1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur + 1]); ((d.yFrom == 0 && d.xFrom == -1) || (d.yTo == 0 && d.xTo == -1))) {
            vec.push_back({0, 1});
        }

        return Dirs{vec.begin()->first, (vec.begin())->second, (vec.begin() + 1)->first, (vec.begin() + 1)->second};
    }();

    Dirs curDirs = mp.at('S');


    bool      switched = false;
    long long LR{};

    auto inputCopy = input;

    auto swtch = [&](size_t const &yC, size_t const &xC) {
        switch (input[yC][xC]) {
            case 'L': switched ? LR++ : LR--; break;
            case 'J': switched ? LR-- : LR++; break;
            case '7': switched ? LR-- : LR++; break;
            case 'F': switched ? LR-- : LR++; break;
            default:  void();
        }
    };

    size_t stepsCounter{};
    do {
        swtch(yCur, xCur);
        inputCopy[yCur][xCur] = '#';

        yCur = yCur + (switched ? curDirs.yFrom : curDirs.yTo);
        xCur = xCur + (switched ? curDirs.xFrom : curDirs.xTo);

        auto const &nextDirs = mp[input[yCur][xCur]];
        if ((((nextDirs.yTo * (-1)) == (switched ? curDirs.yFrom : curDirs.yTo)) &&
             ((nextDirs.xTo * (-1)) == (switched ? curDirs.xFrom : curDirs.xTo)))) {
            switched = true;
        }
        else { switched = false; }

        curDirs = nextDirs;
        stepsCounter++;
    } while (not(yCur == yStart && xCur == xStart));

    curDirs  = mp.at('S');
    switched = false;

    auto rotLR = [&](std::array<long long, 2> const &rot, bool const swtch) {
        using MpItem = std::pair<std::array<long long, 2>, std::array<long long, 2>>;
        constexpr std::array<MpItem, 4> mp{
            {{{-1, 0}, {0, -1}}, {{0, -1}, {1, 0}}, {{1, 0}, {0, 1}}, {{0, 1}, {-1, 0}}}};

        std::array<long long, 2> res =
            std::ranges::find_if(mp, [&](MpItem const &item) { return item.first == rot; })->second;
        if (swtch) {
            res[0] *= -1;
            res[1] *= -1;
        }
        return res;
    };


    auto ff = incstd::explorers::FloodFill(
        std::array{input.size(), input.front().size()},
        [&](std::array<size_t, 2uz> const &pos) { return inputCopy[pos[0]][pos[1]] != '#'; });
    size_t enclosedCounter{};

    do {
        auto const [yChng, xChng] =
            rotLR({switched ? curDirs.yTo : curDirs.yFrom, switched ? curDirs.xTo : curDirs.xFrom}, LR < 0);

        auto const [yChng2, xChng2] =
            rotLR({switched ? curDirs.yFrom : curDirs.yTo, switched ? curDirs.xFrom : curDirs.xTo}, LR >= 0);

        enclosedCounter += ff.execute_fill({yCur + yChng, xCur + xChng});
        enclosedCounter += ff.execute_fill({yCur + yChng2, xCur + xChng2});

        yCur = yCur + (switched ? curDirs.yFrom : curDirs.yTo);
        xCur = xCur + (switched ? curDirs.xFrom : curDirs.xTo);

        auto const &nextDirs = mp[input[yCur][xCur]];
        if ((((nextDirs.yTo * (-1)) == (switched ? curDirs.yFrom : curDirs.yTo)) &&
             ((nextDirs.xTo * (-1)) == (switched ? curDirs.xFrom : curDirs.xTo)))) {
            switched = true;
        }
        else { switched = false; }

        curDirs = nextDirs;
    } while (not(yCur == yStart && xCur == xStart));


    return enclosedCounter;
}
} // namespace AOC2023