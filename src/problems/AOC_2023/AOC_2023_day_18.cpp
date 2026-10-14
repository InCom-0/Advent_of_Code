#include <ankerl/unordered_dense.h>
#include <cassert>
#include <concepts>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incom_omni.hpp>
#include <utility>


namespace AOC2023 {

auto
day18_0(std::string dataFile) {
    auto anyOne_ctre = ctre::search<R"(.)">;
    auto d_ctre      = ctre::search<R"(\d+)">;
    auto nws_ctre    = ctre::search<R"([^ |(]+)">;

    auto res = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, anyOne_ctre, d_ctre, nws_ctre);
    for (auto &line : res) { line.at(2).pop_back(); }
    return res;
}


size_t
day18_1(std::string dataFile) {

    auto        input = day18_0(dataFile);
    std::string aa;

    auto convToDir = [](auto const &str) {
        static constexpr auto defRes = std::array<long long, 2uz>{0, 0};
        static constexpr auto dirs   = incstd::explorers::directions::get_dirChanges_2D();
        switch (str.front()) {
            case 'D': return dirs[0]; break;
            case 'U': return dirs[1]; break;
            case 'R': return dirs[2]; break;
            case 'L': return dirs[3]; break;
            default:  return defRes; break;
        }
    };

    long long yPos = 0ll;
    long long xPos = 0ll;

    long long yMin = 0ll, xMin = 0ll, yMax = 0ll, xMax = 0ll;

    incom::omni::MD_Vector<char, 2uz, '.'> area;
    area.expand_ifNecessary(0, 0);
    area[0ll, 0ll] = '#';

    size_t digged{};
    for (auto const &oneMove : input) {


        auto const dir        = convToDir(oneMove.front());
        size_t     stepsCount = std::stoull(oneMove[1]);

        while (--stepsCount < std::numeric_limits<size_t>::max()) {
            yPos += dir[0];
            xPos += dir[1];

            yMin = std::min(yMin, yPos);
            xMin = std::min(xMin, xPos);
            yMax = std::max(yMax, yPos);
            xMax = std::max(xMax, xPos);

            area.expand_ifNecessary(yPos, xPos);
            area[yPos, xPos] = '#';
            digged++;
        }
    }

    auto f_allowed = [&](std::array<size_t, 2uz> const &pos) {
        long long const yPos = static_cast<long long>(pos[0]) + yMin;
        long long const xPos = static_cast<long long>(pos[1]) + xMin;

        area.expand_ifNecessary(yPos, xPos);
        return area[yPos, xPos] == '.' ? true : false;
    };
    auto f_onFill = [&](auto) { digged++; };

    incstd::explorers::FloodFill ff(std::array<size_t, 2uz>{static_cast<unsigned long long>(yMax - yMin + 1),
                                                            static_cast<unsigned long long>(xMax - xMin + 1)},
                                    std::move(f_allowed), std::move(f_onFill));


    using Pos_t = std::array<long long, 2uz>;
    auto f_is_i = [&](Pos_t const &pos) { return area[pos[0], pos[1]] == '.'; };
    auto f_is_b = [&](Pos_t const &pos) { return area[pos[0], pos[1]] == '#'; };
    auto f_b_a  = [&](Pos_t const &pos) {
        area.expand_ifNecessary(pos[0], pos[1]);
        return;
    };

    if (auto seed = incstd::explorers::find_posInsideBordered(Pos_t{yMin, xMin}, Pos_t{yMax - yMin, xMax - xMin}, f_is_i, f_is_b, f_b_a)) {
        ff.execute_fill({static_cast<unsigned long long>(seed.value()[0] - yMin),
                         static_cast<unsigned long long>(seed.value()[1] - xMin)});
    }
    else { digged = 0uz; }

    return digged;
}


size_t
day18_2(std::string dataFile) {

    auto input = day18_0(dataFile);

    auto convToDir = [](auto const &c) {
        static constexpr auto defRes = std::array<long long, 2uz>{0, 0};
        static constexpr auto dirs   = incstd::explorers::directions::get_dirChanges_2D();
        switch (c) {
            case '0': return dirs[2]; break;
            case '1': return dirs[0]; break;
            case '2': return dirs[3]; break;
            case '3': return dirs[1]; break;
            default:  return defRes; break;
        }
    };

    long long yPos = 0ll;
    long long xPos = 0ll;

    long long yMin = 0ll, xMin = 0ll, yMax = 0ll, xMax = 0ll;


    std::vector<std::array<long long, 2uz>> verts;
    verts.reserve(input.size());

    for (auto const &oneMove : input) {

        size_t stepsCount{};
        std::istringstream(std::string_view(oneMove[2].begin() + 1, oneMove[2].end() - 1)) >> std::hex >> stepsCount;

        auto const dir = convToDir(*(oneMove[2].end() - 1));

        yPos += dir[0] * stepsCount;
        xPos += dir[1] * stepsCount;
        verts.push_back({yPos, xPos});
    }

    return incstd::polygons::compute_areaFromVerts<true>(verts);
}

} // namespace AOC2023