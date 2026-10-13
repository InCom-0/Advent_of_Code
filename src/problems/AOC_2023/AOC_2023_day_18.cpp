#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incom_omni.hpp>


namespace AOC2023 {

template <typename T>
concept TupleLike2 = requires {
    typename std::tuple_size<std::remove_cvref_t<T>>::type;
    requires std::tuple_size_v<std::remove_cvref_t<T>> == 2;
};

template <typename C>
concept VertContainer2D = std::ranges::random_access_range<C> && std::ranges::sized_range<C> &&
                          requires(C const &c, std::ranges::range_reference_t<C> p, size_t i) {
                              { c.size() } -> std::convertible_to<size_t>;
                              { c[i] };
                              { c.front() };
                              { c.back() };
                              requires TupleLike2<decltype(p)>;
                              { static_cast<__int128>(std::get<0>(p)) };
                              { static_cast<__int128>(std::get<1>(p)) };
                          };


template <bool AllBorders = false>
size_t
compute_areaFromVerts(VertContainer2D auto const &verts) {
    if (verts.size() < 3uz) { return 0uz; }

    auto abs_i128 = [](__int128 val) -> __int128 { return val < 0 ? -val : val; };
    auto abs_u64  = [](long long val) -> unsigned long long {
        if (val >= 0ll) { return static_cast<unsigned long long>(val); }
        return static_cast<unsigned long long>(-(val + 1ll)) + 1ull;
    };

    __int128 twiceArea{};
    __int128 boundaryPoints{};

    for (size_t i = 1uz; i < verts.size(); ++i) {
        auto const &curr = verts[i - 1uz];
        auto const &next = verts[i];

        twiceArea += static_cast<__int128>(std::get<0>(curr)) * static_cast<__int128>(std::get<1>(next)) -
                     static_cast<__int128>(std::get<1>(curr)) * static_cast<__int128>(std::get<0>(next));

        if constexpr (AllBorders) {
            auto const dY   = static_cast<long long>(std::get<0>(next) - std::get<0>(curr));
            auto const dX   = static_cast<long long>(std::get<1>(next) - std::get<1>(curr));
            boundaryPoints += static_cast<__int128>(std::gcd(abs_u64(dY), abs_u64(dX)));
        }
    }

    if (verts.back() != verts.front()) {
        auto const &last  = verts.back();
        auto const &first = verts.front();

        twiceArea += static_cast<__int128>(std::get<0>(last)) * static_cast<__int128>(std::get<1>(first)) -
                     static_cast<__int128>(std::get<1>(last)) * static_cast<__int128>(std::get<0>(first));

        if constexpr (AllBorders) {
            auto const dY   = static_cast<long long>(std::get<0>(first) - std::get<0>(last));
            auto const dX   = static_cast<long long>(std::get<1>(first) - std::get<1>(last));
            boundaryPoints += static_cast<__int128>(std::gcd(abs_u64(dY), abs_u64(dX)));
        }
    }
    if constexpr (AllBorders) { return static_cast<size_t>((abs_i128(twiceArea) + boundaryPoints) / 2 + 1); }
    else { return static_cast<size_t>(abs_i128(twiceArea) / 2); }
}


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

    auto findSeed = [&]() -> std::optional<std::array<long long, 2uz>> {
        std::optional<std::array<long long, 2uz>> res{};

        for (long long row = yMin + 1; row < yMax; ++row) {
            size_t crossCount{};
            for (long long col = xMin; col < xMax; ++col) {
                area.expand_ifNecessary(row, col);

                if (area[row, col] != '.') { crossCount++; }
                else if (area[row, col] == '.' && crossCount % 2) {
                    auto const c_bu = col;
                    for (long long colVer = ++col; colVer < xMax; ++colVer, ++col) {
                        area.expand_ifNecessary(row, colVer);
                        if (area[row, colVer] == '#') {
                            res = {row, c_bu};
                            goto RET;
                        }
                    }
                }
            }
        }
    RET:
        return res;
    };


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

    size_t res{};
    if (auto seed = findSeed()) {
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