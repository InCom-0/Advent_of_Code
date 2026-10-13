#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/explorers.hpp>
#include <incstd/core/hashing.hpp>
#include <limits>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>


namespace AOC2023 {

struct day17_PL {
    std::array<size_t, 2ULL>    pos{};
    std::array<long long, 2ULL> toHere{};
    size_t                      sameToHereCount{};
    size_t                      cost{};
};

auto
day17_0(std::string dataFile) {
    auto nws_ctre = ctre::search<R"(.+)">;
    return incom::aoc::parseInputUsingCTRE::processFile(dataFile, nws_ctre).front();
}

size_t
day17_1(std::string dataFile) {

    auto input = day17_0(dataFile);

    constexpr auto fourDirs = incstd::explorers::directions::get_dirChanges_2D();
    auto           cmp      = [](auto &a, auto &b) { return a.cost > b.cost; };
    std::priority_queue<day17_PL, std::vector<day17_PL>, decltype(cmp)> q(cmp);
    q.push({{0, 0}, {0, 0}, 0uz, 0uz});

    auto const endPos = std::array<size_t, 2ULL>{input.size() - 1, input.front().size() - 1};

    ankerl::unordered_dense::set<std::pair<std::array<long long, 4uz>, size_t>, incstd::hashing::XXH3Hasher> visited;

    while (not q.empty()) {
        auto topEle = std::move(q.top());
        q.pop();

        if (topEle.pos == endPos) { return topEle.cost; }

        for (auto const &[yChng, xChng] : fourDirs) {
            auto const yNew = topEle.pos[0] + yChng;
            auto const xNew = topEle.pos[1] + xChng;

            if (yNew < input.size() && xNew < input.front().size()) {
                if (yChng * -1 == topEle.toHere[0] && xChng * -1 == topEle.toHere[1]) { continue; }
                else if (yChng == topEle.toHere[0] && xChng == topEle.toHere[1] && topEle.sameToHereCount > 2) {
                    continue;
                }
                else {
                    bool const tf = (yChng == topEle.toHere[0]) && (xChng == topEle.toHere[1]);
                    if (auto it =
                            visited.insert({{static_cast<long long>(yNew), static_cast<long long>(xNew), yChng, xChng},
                                            tf ? topEle.sameToHereCount + 1 : 1});
                        it.second) {
                        q.push({{yNew, xNew},
                                {yChng, xChng},
                                tf ? topEle.sameToHereCount + 1 : 1,
                                topEle.cost + (input.at(yNew).at(xNew) - 48)});
                    }
                }
            }
        }
    }


    return std::numeric_limits<size_t>::max();
}

size_t
day17_2(std::string dataFile) {
    auto input = day17_0(dataFile);

    constexpr auto fourDirs = incstd::explorers::directions::get_dirChanges_2D();
    auto           cmp      = [](auto &a, auto &b) { return a.cost > b.cost; };
    std::priority_queue<day17_PL, std::vector<day17_PL>, decltype(cmp)> q(cmp);
    q.push({{0, 0}, {0, 1}, 0uz, 0uz});
    q.push({{0, 0}, {1, 0}, 0uz, 0uz});

    auto const endPos = std::array<size_t, 2ULL>{input.size() - 1, input.front().size() - 1};

    ankerl::unordered_dense::set<std::pair<std::array<long long, 4uz>, size_t>, incstd::hashing::XXH3Hasher> visited;

    while (not q.empty()) {
        auto topEle = std::move(q.top());
        q.pop();

        if (topEle.pos == endPos && topEle.sameToHereCount >= 4uz) { return topEle.cost; }

        for (auto const &[yChng, xChng] : fourDirs) {
            auto const yNew = topEle.pos[0] + yChng;
            auto const xNew = topEle.pos[1] + xChng;

            if (yNew < input.size() && xNew < input.front().size()) {
                if (yChng * -1 == topEle.toHere[0] && xChng * -1 == topEle.toHere[1]) { continue; }
                else if (yChng == topEle.toHere[0] && xChng == topEle.toHere[1]) {
                    if (topEle.sameToHereCount > 9) { continue; }
                }
                else {
                    if (topEle.sameToHereCount < 4) { continue; }
                }

                bool const tf = (yChng == topEle.toHere[0]) && (xChng == topEle.toHere[1]);
                if (auto it =
                        visited.insert({{static_cast<long long>(yNew), static_cast<long long>(xNew), yChng, xChng},
                                        tf ? topEle.sameToHereCount + 1 : 1});
                    it.second) {
                    q.push({{yNew, xNew},
                            {yChng, xChng},
                            tf ? topEle.sameToHereCount + 1 : 1,
                            topEle.cost + (input.at(yNew).at(xNew) - 48)});
                }
            }
        }
    }


    return std::numeric_limits<size_t>::max();
}
} // namespace AOC2023