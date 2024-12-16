#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/format.h>
#include <functional>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;
long long day14_1(std::string dataFile, size_t cyclesToSimulate) {
    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre, d_ctre, d_ctre);

    constexpr long long Xsize = 101;
    constexpr long long Ysize = 103;

    std::vector map(Ysize, std::vector(Xsize, 0uz));

    for (auto const &line : input) {
        std::array<long long, 4> roboNums;
        for (int i = 0; auto const &strNum : line) { roboNums[i++] = std::stoll(strNum); }

        roboNums[0] += roboNums[2] * cyclesToSimulate;
        roboNums[1] += roboNums[3] * cyclesToSimulate;

        if (roboNums[0] >= 0) { roboNums[0] = roboNums[0] % Xsize; }
        else {
            roboNums[0] = (roboNums[0] % Xsize) + Xsize;
            if (roboNums[0] == Xsize) { roboNums[0] = 0; }
        }

        if (roboNums[1] >= 0) { roboNums[1] = roboNums[1] % Ysize; }
        else {
            roboNums[1] = (roboNums[1] % Ysize) + Ysize;
            if (roboNums[1] == Ysize) { roboNums[1] = 0; }
        }
        map[roboNums[1]][roboNums[0]]++;
    }

    std::array<size_t, 4> accuArr{0, 0, 0, 0};
    for (int i = 0; i < map.size(); ++i) {
        if (i == Ysize / 2) { continue; }
        for (int j = 0; j < map[i].size(); ++j) {
            if (j == Xsize / 2) { continue; }
            accuArr[(i > Ysize / 2) * 2 + (j > Xsize / 2)] += map[i][j];
        }
    }


    return std::ranges::fold_left_first(accuArr, std::multiplies()).value();
}

long long day14_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre, d_ctre, d_ctre);

    constexpr long long Xsize = 101;
    constexpr long long Ysize = 103;

    std::vector                           map(Ysize, std::vector(Xsize, 0uz));
    std::vector<std::array<long long, 4>> robots;
    size_t                                curOverlapping = 0;
    for (auto const &line : input) {
        std::array<long long, 4> roboNums;
        for (int i = 0; auto const &strNum : line) { roboNums[i++] = std::stoll(strNum); }
        robots.push_back(std::move(roboNums));
        map[robots.back()[1]][robots.back()[0]]++;
        if (map[robots.back()[1]][robots.back()[0]] > 1) { curOverlapping++; }
    }

    size_t counter = 0;

    while (true) {
        for (int i = 0; i < robots.size(); ++i) {

            map[robots[i][1]][robots[i][0]]--;
            if (map[robots[i][1]][robots[i][0]] > 0) { curOverlapping--; }
            robots[i][0] += robots[i][2];
            robots[i][1] += robots[i][3];

            if (robots[i][0] >= 0) { robots[i][0] = robots[i][0] % Xsize; }
            else {
                robots[i][0] = (robots[i][0] % Xsize) + Xsize;
                if (robots[i][0] == Xsize) { robots[i][0] = 0; }
            }

            if (robots[i][1] >= 0) { robots[i][1] = robots[i][1] % Ysize; }
            else {
                robots[i][1] = (robots[i][1] % Ysize) + Ysize;
                if (robots[i][1] == Ysize) { robots[i][1] = 0; }
            }
            map[robots[i][1]][robots[i][0]]++;
            if (map[robots[i][1]][robots[i][0]] > 1) { curOverlapping++; }
        }

        counter++;
        if (curOverlapping < 3) { break; }
    }
    return counter;
}
} // namespace AOC2024