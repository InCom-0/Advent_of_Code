#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.hpp>
#include <incstd/core/buffers.hpp>
#include <incstd/core/explorers.hpp>
#include <limits>
#include <ranges>
#include <string>
#include <utility>


namespace AOC2023 {

constexpr auto DR = incstd::explorers::directions::get_dirChanges_2D();

enum class Dirs : size_t {
    S = 0,
    N,
    E,
    W,
};

using VH = std::array<bool, 4uz>;


constexpr auto
dirMapping(Dirs const &d) {
    return DR[std::to_underlying(d)];
}

void
reverse(Dirs &d) {
    d = Dirs{std::to_underlying(d) / 4};
}

constexpr auto
getNextDir(char const &mapChar, Dirs const &fromDir) {

    using enum Dirs;
    using ND = std::array<Dirs, 4>;

    constexpr ND dot{S, N, E, W};
    constexpr ND slash{{W, E, N, S}};
    constexpr ND backSlash{{E, W, S, N}};

    constexpr ND dash{{W, E, E, W}};
    constexpr ND dash2{{E, W, E, W}};

    constexpr ND vBar{{S, N, S, N}};
    constexpr ND vBar2{{S, N, N, S}};

    std::vector<Dirs> res;
    res.reserve(2uz);
    auto const undrl = std::to_underlying(fromDir);

    switch (mapChar) {
        case '.':  res.push_back(dot[undrl]); break;
        case '/':  res.push_back(slash[undrl]); break;
        case '\\': res.push_back(backSlash[undrl]); break;
        case '-':
            res.push_back(dash[undrl]);
            res.push_back(dash2[undrl]);
            break;
        case '|':
            res.push_back(vBar[undrl]);
            res.push_back(vBar2[undrl]);
            break;

        default: void();
    }
    return res;
}


auto
day16_0(std::string dataFile) {
    auto nws_ctre = ctre::search<R"(.+)">;
    return incom::aoc::parseInputUsingCTRE::processFile(dataFile, nws_ctre);
}


size_t
day16_1(std::string dataFile) {
    using enum Dirs;
    auto input = day16_0(dataFile).front();

    std::vector visited(input.size(), std::vector(input.front().size(), VH{false, false, false, false}));
    auto        recu = [&](this auto const &self, Dirs dir, size_t curY, size_t curX) -> void {
        auto const oneDr  = dirMapping(dir);
        curY             += oneDr.front();
        curX             += oneDr.back();
        if (curY < input.size() && curX < input.front().size()) {

            for (auto const nd : getNextDir(input[curY][curX], dir)) {
                if (not visited[curY][curX][std::to_underlying(nd)]) {
                    visited[curY][curX][std::to_underlying(nd)] = true;
                    self(nd, curY, curX);
                }
            }
        }
    };
    recu(E, 0uz, std::numeric_limits<size_t>::max());

    size_t accu{};
    for (auto const &visiLine : visited) {
        accu += std::ranges::fold_left(visiLine, 0uz, [](auto const init, auto const &oneArr) {
            return init + std::ranges::fold_left(oneArr, false, std::bit_or{});
        });
    }

    return accu;
}

size_t
day16_2(std::string dataFile) {
    using enum Dirs;
    auto input = day16_0(dataFile).front();


    std::vector visited(input.size(), std::vector(input.front().size(), VH{false, false, false, false}));
    auto        recu = [&](this auto const &self, Dirs dir, size_t curY, size_t curX) -> void {
        auto const oneDr  = dirMapping(dir);
        curY             += oneDr.front();
        curX             += oneDr.back();
        if (curY < input.size() && curX < input.front().size()) {

            for (auto const nd : getNextDir(input[curY][curX], dir)) {
                if (not visited[curY][curX][std::to_underlying(nd)]) {
                    visited[curY][curX][std::to_underlying(nd)] = true;
                    self(nd, curY, curX);
                }
            }
        }
    };

    size_t curMax{};
    auto   updateCurMax = [&](auto const &curVisited) {
        size_t accu{};
        for (auto const &visiLine : curVisited) {
            accu += std::ranges::fold_left(visiLine, 0uz, [](auto const init, auto const &oneArr) {
                return init + std::ranges::fold_left(oneArr, false, std::bit_or{});
            });
        }
        curMax = std::max(curMax, accu);
    };

    for (size_t row{}; row < input.size(); ++row) {
        visited = std::vector(input.size(), std::vector(input.front().size(), VH{false, false, false, false}));
        recu(W, row, input[row].size());
        updateCurMax(visited);

        visited = std::vector(input.size(), std::vector(input.front().size(), VH{false, false, false, false}));
        recu(E, row, std::numeric_limits<size_t>::max());
        updateCurMax(visited);
    }
    
    for (size_t col{}; col < input.front().size(); ++col) {
        visited = std::vector(input.size(), std::vector(input.front().size(), VH{false, false, false, false}));
        recu(S, std::numeric_limits<size_t>::max(), col);
        updateCurMax(visited);

        visited = std::vector(input.size(), std::vector(input.front().size(), VH{false, false, false, false}));
        recu(N, input.size(), col);
        updateCurMax(visited);
    }


    return curMax;
}
} // namespace AOC2023