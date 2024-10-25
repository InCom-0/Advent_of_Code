#include <algorithm>
#include <climits>
#include <cmath>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <numbers>
#include <numeric>


namespace AOC2019 {
std::pair<std::pair<long long, long long>, long long> day10_0(std::string dataFile) {

    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incom::commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    long long maxMapDimension = std::max(input.size(), input.front().size());

    std::vector divisorsMap(maxMapDimension, std::vector<long long>());

    for (long long sel = 2; sel < maxMapDimension; ++sel) {
        for (long long eva = sel; eva > 1; --eva) {
            if (sel % eva == 0) { divisorsMap[sel].push_back(eva); }
        }
    }
    divisorsMap[1].push_back(1);
    divisorsMap[0] = std::vector<long long>(maxMapDimension - 1, 0);
    std::ranges::iota(divisorsMap[0], 1);

    std::pair<std::pair<long long, long long>, long long> maxVisible = {{LLONG_MIN, LLONG_MIN}, LLONG_MIN};
    for (int selY = 0; selY < input.size(); ++selY) {
        for (int selX = 0; selX < input[selY].size(); ++selX) {
            if (input[selY][selX] == '.') { continue; }

            long long thisVisible = 0;

            for (int otherY = 0; otherY < input.size(); ++otherY) {
                for (int otherX = 0; otherX < input[otherY].size(); ++otherX) {
                    if (input[otherY][otherX] == '.') { continue; }

                    int distY = otherY - selY;
                    int distX = otherX - selX;

                    bool vis = true;
                    if (std::abs(distY) == 1 or std::abs(distX) == 1) {}
                    else {
                        for (auto diviAtt : divisorsMap[std::abs(distY)]) {
                            if (std::ranges::any_of(divisorsMap[std::abs(distX)],
                                                    [&](auto &&a) { return a == diviAtt; })) {
                                long long stepY = distY / diviAtt;
                                long long stepX = distX / diviAtt;

                                for (long long i = 1; i < diviAtt; ++i) {
                                    if (input[selY + (i * stepY)][selX + (i * stepX)] == '#') {
                                        vis = false;
                                        break;
                                    }
                                }
                                if (vis == false) { break; }
                            }
                        }
                    }
                    thisVisible += vis;
                }
            }
            if (maxVisible.second < thisVisible) { maxVisible = decltype(maxVisible){{selY, selX}, thisVisible}; }
        }
    }
    return maxVisible;
}
long long day10_1(std::string dataFile) {
    return day10_0(dataFile).second;
}

long long day10_2(std::string dataFile) {

    auto const laserLoc = day10_0(dataFile);

    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incom::commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    long long maxMapDimension = std::max(input.size(), input.front().size());

    std::vector divisorsMap(maxMapDimension, std::vector<long long>());

    for (long long sel = 2; sel < maxMapDimension; ++sel) {
        for (long long eva = sel; eva > 1; --eva) {
            if (sel % eva == 0) { divisorsMap[sel].push_back(eva); }
        }
    }
    divisorsMap[1].push_back(1);
    divisorsMap[0] = std::vector<long long>(maxMapDimension - 1, 0);
    std::ranges::iota(divisorsMap[0], 1);

    std::vector<std::pair<long long, long long>> visibles;

    for (int otherY = 0; otherY < input.size(); ++otherY) {
        for (int otherX = 0; otherX < input[otherY].size(); ++otherX) {
            if (input[otherY][otherX] == '.') { continue; }

            int distY = otherY - laserLoc.first.first;
            int distX = otherX - laserLoc.first.second;

            bool vis = true;
            if (std::abs(distY) == 1 or std::abs(distX) == 1) {}
            else {

                for (auto diviAtt : divisorsMap[std::abs(distY)]) {
                    if (std::ranges::any_of(divisorsMap[std::abs(distX)], [&](auto &&a) { return a == diviAtt; })) {
                        long long stepY = distY / diviAtt;
                        long long stepX = distX / diviAtt;

                        for (long long i = 1; i < diviAtt; ++i) {
                            if (input[laserLoc.first.first + (i * stepY)][laserLoc.first.second + (i * stepX)] == '#') {
                                vis = false;
                                break;
                            }
                        }
                        if (vis == false) { break; }
                    }
                }
            }
            if (vis) { visibles.emplace_back(otherY, otherX); }
        }
    }

    std::ranges::sort(visibles, [&](auto &&a, auto &&b) {
        auto resA = std::atan2(a.second - laserLoc.first.second, (a.first - laserLoc.first.first) * -1);
        if (resA < 0) { resA += (2 * std::numbers::pi); }
        auto resB = std::atan2(b.second - laserLoc.first.second, (b.first - laserLoc.first.first) * -1);
        if (resB < 0) { resB += (2 * std::numbers::pi); }

        return resA < resB;
    });
    auto &res = visibles.at(199);

    return res.second * 100 + res.first;
}
} // namespace AOC2019