#pragma once

#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <array>
#include <incstd/core/hashing.hpp>
#include <incstd/core/matrix.hpp>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <tuple>
#include <utility>

#include <incom_commons.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include <incstd/incstd_all.hpp>


namespace incom {
namespace aoc {
namespace solvers {
namespace tetris {

namespace {
struct FastPseudoRandom {
    uint64_t m_state = 0x9e3779b97f4a7c15ull;

    FastPseudoRandom() = default;

    explicit FastPseudoRandom(uint64_t seed) { setSeed(seed); }

    void
    setSeed(uint64_t seed) {
        m_state = seed;
        if (m_state == 0ull) { m_state = 0x9e3779b97f4a7c15ull; }
    }

    uint64_t
    nextRandomWord() {
        uint64_t z = (m_state += 0x9e3779b97f4a7c15ull);
        z          = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
        z          = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
        return z ^ (z >> 31);
    }

    static uint64_t
    multiplyHigh64(uint64_t lhs, uint64_t rhs) {
#if defined(_MSC_VER) && ! defined(__clang__)
        uint64_t high = 0;
        _umul128(lhs, rhs, &high);
        return high;
#else
        return static_cast<uint64_t>((static_cast<unsigned __int128>(lhs) * rhs) >> 64);
#endif
    }

    size_t
    pseudoRandom_0_to(size_t maxInclusive) {
        if (maxInclusive == std::numeric_limits<size_t>::max()) { return static_cast<size_t>(nextRandomWord()); }

        auto const bound = static_cast<uint64_t>(maxInclusive) + 1ull;
        return static_cast<size_t>(multiplyHigh64(nextRandomWord(), bound));
    }
};
} // namespace


template <size_t SQSZ>
requires(SQSZ > 2)
struct Shape {
    struct OverlayRes {
        Shape res_shp;

        size_t pointsAdded;
        size_t pointsOverlaid;
        size_t bordersTouching;
        size_t bordersNotTouching;

        double surfaceCovered_relative;
        double surfaceOpened_relative;
    };

    std::array<std::array<bool, SQSZ>, SQSZ> m_matrix = {};

    // Construction
    Shape()                 = default;
    Shape(Shape const &src) = default;
    Shape(Shape &&src)      = default;
    ~Shape()                = default;

    Shape &
    operator=(Shape const &) = default;
    Shape &
    operator=(Shape &&) = default;

    bool
    operator==(Shape const &other) const = default;


    Shape(std::array<std::array<bool, SQSZ - 2>, SQSZ - 2> const &src) {
        for (auto r = 1uz; auto const &line : src) {
            for (auto c = 1uz; bool one : line) { m_matrix[r][c++] = one; }
            r++;
        }
    }
    Shape(std::array<std::array<bool, SQSZ>, SQSZ> const &src) : m_matrix(src) {}

    OverlayRes
    compute_overlayWith(Shape const &other) const {
        OverlayRes res{{}, 0uz, 0uz, 0uz};
        for (size_t r = 0; r < SQSZ; ++r) {
            for (size_t c = 0; c < SQSZ; ++c) {
                res.pointsOverlaid         += (m_matrix[r][c] and other.m_matrix[r][c]);
                res.pointsAdded            += (not m_matrix[r][c]) and other.m_matrix[r][c];
                res.res_shp.m_matrix[r][c]  = m_matrix[r][c] | other.m_matrix[r][c];
            }
        }
        for (size_t r = 1; r < SQSZ - 1; ++r) {
            for (size_t c = 1; c < SQSZ - 1; ++c) {
                if (other.m_matrix[r][c]) {
                    res.bordersTouching += m_matrix[r - 1][c] & (not other.m_matrix[r - 1][c]);
                    res.bordersTouching += m_matrix[r][c - 1] & (not other.m_matrix[r][c - 1]);
                    res.bordersTouching += m_matrix[r][c + 1] & (not other.m_matrix[r][c + 1]);
                    res.bordersTouching += m_matrix[r + 1][c] & (not other.m_matrix[r + 1][c]);

                    res.bordersNotTouching += (not m_matrix[r - 1][c]) & (not other.m_matrix[r - 1][c]);
                    res.bordersNotTouching += (not m_matrix[r][c - 1]) & (not other.m_matrix[r][c - 1]);
                    res.bordersNotTouching += (not m_matrix[r][c + 1]) & (not other.m_matrix[r][c + 1]);
                    res.bordersNotTouching += (not m_matrix[r + 1][c]) & (not other.m_matrix[r + 1][c]);
                }
            }
        }

        res.surfaceCovered_relative = (res.bordersTouching / static_cast<double>(res.pointsAdded));
        res.surfaceOpened_relative  = (res.bordersNotTouching / static_cast<double>(res.pointsAdded));

        return res;
    }

    std::vector<Shape>
    compute_alternsRotFlip() const {
        namespace incmatrix = incom::standard::matrix;

        auto                                                                                m_matrix_cpy = m_matrix;
        ankerl::unordered_dense::set<decltype(m_matrix_cpy), standard::hashing::XXH3Hasher> hlprMP;

        hlprMP.insert(m_matrix_cpy);
        for (int rot_i = 0; rot_i < 3; ++rot_i) {
            incmatrix::matrixRotateLeft(m_matrix_cpy);
            hlprMP.insert(m_matrix_cpy);
        }

        // Flip vertically
        for (size_t i = 0uz; i < (m_matrix_cpy.size() / 2); ++i) {
            std::swap(m_matrix_cpy.at(i), m_matrix_cpy.at(m_matrix_cpy.size() - 1 - i));
        }

        hlprMP.insert(m_matrix_cpy);
        for (int rot_i = 0; rot_i < 3; ++rot_i) {
            incmatrix::matrixRotateLeft(m_matrix_cpy);
            hlprMP.insert(m_matrix_cpy);
        }

        return std::vector<Shape>(hlprMP.begin(), hlprMP.end());
    }
};

// ADL for hashing using XXH3Hasher
template <size_t SQSZ>
constexpr void
XXH3Hash(Shape<SQSZ> const &input, XXH3_state_t *state) {
    XXH3_64bits_update(state, input.m_matrix.data(), sizeof(decltype(input.m_matrix)));
}

template <size_t SQSZ>
class Solver {
public:
    struct Pos {
        long long y = 0;
        long long x = 0;
    };

    using Shape_t   = Shape<SQSZ>;
    using PastRes_t = std::pair<Pos, typename Shape_t::OverlayRes>; // Pos == [][] positions in 'm_shapes_alterns'
    using pastResMap_t =
        ankerl::unordered_dense::segmented_map<Shape_t, std::vector<PastRes_t>, incom::standard::hashing::XXH3Hasher>;


    // 1) Position (top left) in matrix, 2) What shape currently is at that position, 3) Possibilities
    std::vector<std::tuple<Pos, Shape_t &, std::vector<PastRes_t> &>> m_frontierTiles;
    std::vector<size_t>                                               m_useableCount_perShape;


private:
    std::vector<std::vector<char>> m_area;
    Pos const                      firstTilePos;

    std::vector<std::vector<Shape_t>> const m_shapes_alterns;
    FastPseudoRandom                        m_fprng;

    // Memoization of what 'shapes_alterns'
    pastResMap_t m_pastComputed;

    void
    set_pseudoRandomSeed(uint64_t seed) {
        m_fprng.setSeed(seed);
    }


    std::size_t
    hash_stateOfSelf() {
        XXH3_state_t *state = XXH3_createState();
        XXH3_64bits_reset_withSeed(state, 0);

        size_t const m_area_ySz = m_area.size();
        size_t const m_area_xSz = m_area.empty() ? 0uz : m_area.front().size();
        XXH3_64bits_update(state, &m_area_ySz, sizeof(size_t));
        XXH3_64bits_update(state, &m_area_xSz, sizeof(size_t));

        if (m_frontierTiles.size() > 0) {
            XXH3_64bits_update(state, &std::get<0>(m_frontierTiles.front()).y, sizeof(long long));
            XXH3_64bits_update(state, &std::get<0>(m_frontierTiles.front()).x, sizeof(long long));
        }

        for (auto const &alternsLine : m_shapes_alterns) {
            for (auto const &shp : alternsLine) { XXH3Hash(shp, state); }
        }
        // XXH3_64bits_update(state, m_useableCount_perShape.data(),
        //                    sizeof(typename std::remove_cvref_t<decltype(m_useableCount_perShape)>::value_type) *
        //                        m_useableCount_perShape.size());

        XXH64_hash_t result = XXH3_64bits_digest(state);
        XXH3_freeState(state);
        return result;
    }

    std::optional<std::tuple<Shape_t &, std::vector<PastRes_t> &>>
    get_possibsFor(Shape_t const &tile) {
        if (auto found = m_pastComputed.find(tile); found != m_pastComputed.end()) {
            return std::tie(found->first, found->second);
        }
        return std::nullopt;
    }

    std::tuple<Shape_t &, std::vector<PastRes_t> &>
    compute_possibsFor(Shape_t const &tile) {
        std::vector<PastRes_t> resToMap;
        auto allowed = [](PastRes_t const &toCheck) -> bool { return (toCheck.second.pointsOverlaid == 0uz); };

        for (long long shpID = 0; shpID < m_shapes_alterns.size(); ++shpID) {
            for (long long alternID = 0; alternID < m_shapes_alterns.at(shpID).size(); ++alternID) {
                auto rs =
                    PastRes_t{Pos{shpID, alternID}, tile.compute_overlayWith(m_shapes_alterns.at(shpID).at(alternID))};
                if (allowed(rs)) { resToMap.push_back(rs); }
            }
        }
        std::ranges::sort(resToMap, [](auto const &l, auto const &r) -> bool {
            double const soDif = r.second.surfaceOpened_relative - l.second.surfaceOpened_relative;
            if (soDif == 0.0) { return l.second.pointsAdded > r.second.pointsAdded; }
            else { return std::abs(soDif) + soDif; }
        });
        auto insRes = m_pastComputed.insert({tile, resToMap});
        return std::tie(insRes.first->first, insRes.first->second);
    }

    std::vector<Pos>
    get_surrOverlappingPoss(Pos const &shapePos) {
        std::vector<Pos> res;

        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        constexpr long long SQSZcpy = static_cast<long long>(SQSZ);

        // shapePos needs to be the Pos of some valid window in our area
        if (shapePos.y <= (rows - SQSZ) && shapePos.x <= (cols - SQSZ)) {
            for (int rc = -(SQSZcpy - 2); rc < (SQSZcpy - 1); ++rc) {
                for (int cc = -(SQSZcpy - 2); cc < (SQSZcpy - 1); ++cc) {
                    long long const r_loc = shapePos.y + rc;
                    long long const c_loc = shapePos.x + cc;
                    // if (rc != 0 && cc != 0) {
                    //     if (r_loc >= 0 && r_loc <= (rows - SQSZ) && c_loc >= 0 && c_loc <= (cols - SQSZ)) {
                    //         res.push_back(Pos{.y = r_loc, .x = c_loc});
                    //     }
                    // }
                    if (r_loc >= 0 && r_loc <= (rows - SQSZ) && c_loc >= 0 && c_loc <= (cols - SQSZ)) {
                        res.push_back(Pos{.y = r_loc, .x = c_loc});
                    }
                }
            }
        }
        return res;
    }

    std::expected<Shape_t, int>
    get_windowAtPos(Pos const &shapePos) {
        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        std::expected<Shape_t, int> res{std::unexpected(0)};

        if (shapePos.y >= 0 && shapePos.y <= (rows - SQSZ) && shapePos.x >= 0 && shapePos.x <= (cols - SQSZ)) {
            res             = Shape_t{};
            Shape_t &shpRes = res.value();

            for (int row = shapePos.y; row < shapePos.y + SQSZ; ++row) {
                for (int col = shapePos.x; col < (shapePos.x + SQSZ); ++col) {
                    shpRes.m_matrix[row - shapePos.y][col - shapePos.x] = m_area[row][col];
                }
            }
        }
        return res;
    }

    bool
    is_posValid(Pos const &p) {
        const long long py = p.y;
        const long long px = p.x;
        if (py < 0ll || py > (static_cast<long long>(m_area.size()) - SQSZ) || px < 0ll ||
            px > (m_area.size() == 0 ? 0 : m_area.front().size() - SQSZ)) {
            return false;
        }
        return true;
    }

    bool
    set_windowAtPos(Pos const &shapePos, PastRes_t const &pr) {
        if (not is_posValid(shapePos)) { return false; }
        for (long long r = shapePos.y; r < (shapePos.y + SQSZ); ++r) {
            for (long long c = shapePos.x; c < (shapePos.x + SQSZ); ++c) {
                m_area[r][c] = pr.second.res_shp.m_matrix[r - shapePos.y][c - shapePos.x];
            }
        }
        return true;
    }


    size_t
    erase_fromFrontier(std::vector<Pos> const &shapePoss) {

        auto const [ite_first, ite_last] = std::ranges::remove_if(m_frontierTiles, [&](auto &tpl) { return true; });
        size_t const removed             = ite_last - ite_first;
        m_frontierTiles.erase(ite_first, ite_last);
        return removed;
    }

    size_t
    add_toFrontier(std::vector<Pos> const &shapePoss) {
        size_t resCount = 0uz;
        for (auto const &onePos : shapePoss) {
            auto window = get_windowAtPos(onePos);
            if (not window.has_value()) { continue; }

            auto possibsForWindow = getOrCompute_possibsFor(window.value());
            if (std::get<1>(possibsForWindow).size() > 0) {
                m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(onePos), possibsForWindow));
            }
            resCount++;
        }
        return resCount;
    }


public:
    Solver() {}
    Solver(size_t const area_ySize, size_t const area_xSize, std::vector<std::vector<Shape_t>> const &shps_alterns,
           size_t const firstTile_yPos = 0uz, size_t const firstTile_xPos = 0uz, pastResMap_t const &pastReslts = {})
        : m_area(std::vector(area_ySize + 2, std::vector<char>(area_xSize + 2, 0))),
          firstTilePos(Pos{.y = static_cast<long long>(firstTile_yPos), .x = static_cast<long long>(firstTile_xPos)}),
          m_shapes_alterns(shps_alterns), m_pastComputed(pastReslts) {

        std::ranges::fill(m_area.front(), 1);
        for (auto &line : std::views::take(m_area, m_area.size() - 1) | std::views::drop(1)) {
            line.front() = 1;
            line.back()  = 1;
        }
        std::ranges::fill(m_area.back(), 1);

        auto const ftPos     = Pos{.y = static_cast<long long>(std::min(firstTile_yPos, area_ySize - SQSZ)),
                                   .x = static_cast<long long>(std::min(firstTile_xPos, area_xSize - SQSZ))};
        auto       firstTile = get_windowAtPos(ftPos).value();

        m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(ftPos), getOrCompute_possibsFor(firstTile)));
    }

    Solver(size_t const &area_ySize, size_t const &area_xSize,
           std::vector<std::array<std::array<bool, SQSZ - 2>, SQSZ - 2>> const &shps, size_t const firstTile_yPos = 0uz,
           size_t const firstTile_xPos = 0uz, pastResMap_t const &pastReslts = {})
        : Solver(area_ySize, area_xSize,
                 std::views::transform(
                     shps, [](auto const &smallerShp) { return Shape_t(smallerShp).compute_alternsRotFlip(); }) |
                     std::ranges::to<std::vector>(),
                 firstTile_yPos, firstTile_xPos, pastReslts) {}

    // 'Primes' random number generator used by the solver instance with a seed based on hash of the solver state
    // Returns the the seed it used
    size_t
    prime_fprng() {
        size_t const res = hash_stateOfSelf();
        set_pseudoRandomSeed(res);
        return res;
    }

    size_t
    get_pseudoRandom_0_to(size_t maxInclusive) {
        return m_fprng.pseudoRandom_0_to(maxInclusive);
    }

    // Returns true if this shape is actually new, False if it is the same as some other existing shape
    bool
    add_shape(Shape_t const &toAdd) {
        return true;
    }

    Solver
    clone_keepShapeData() {
        return Solver(m_area.size(), m_area.size() > 0 ? m_area.front().size() : 0uz, m_shapes_alterns, firstTilePos.y,
                      firstTilePos.x, m_pastComputed);
    }
    Solver
    clone_keepShapeData(size_t const area_ySize, size_t const area_xSize) {
        return Solver(area_ySize, area_xSize, m_shapes_alterns, firstTilePos.y, firstTilePos.x, m_pastComputed);
    }


    std::tuple<Shape_t &, std::vector<PastRes_t> &>
    getOrCompute_possibsFor(Shape_t const &tile) {
        if (auto comp = get_possibsFor(tile); comp.has_value()) { return comp.value(); }
        return compute_possibsFor(tile);
    }

    void
    print_areaState() {
        for (auto const &line : m_area) {

            auto        r = std::views::transform(line, [](char oneCh) -> char {
                if (oneCh == 0) { return 46; }
                return 35;
            });
            std::string a = std::format("{:s}\n", r);
            std::cout << a;
        }
    }


    std::optional<std::tuple<Pos, PastRes_t &>>
    solve_oneStep(size_t const &tolerance = 0uz) {
        std::vector<std::array<size_t, 4>> toConsider;
        double                             lastSOR{std::numeric_limits<double>::max()};
        size_t                             maxUseableCount = 0uz;

        for (size_t ft_i = 0uz; auto const &oneFT : m_frontierTiles) {
            typename Shape_t::OverlayRes ora{};
            for (size_t alt_i = 0uz; auto const &[alternsPos, overlay] : std::get<2>(oneFT)) {
                if (overlay.surfaceOpened_relative > lastSOR) { break; }
                if (overlay.surfaceOpened_relative < lastSOR) {
                    maxUseableCount = 0uz;
                    toConsider.clear();
                    lastSOR = overlay.surfaceOpened_relative;
                }
                maxUseableCount = std::max(maxUseableCount, m_useableCount_perShape.at(alternsPos.y));
                toConsider.push_back(
                    {ft_i, alt_i++, static_cast<size_t>(alternsPos.y), static_cast<size_t>(alternsPos.x)});
            }
            ft_i++;
        }
        //  There are none viable overlays ... can't solve any more
        if (toConsider.empty()) {
             return std::nullopt; }
        if (maxUseableCount == 0uz) {
             return std::nullopt; }


        auto [last, end] = std::ranges::remove_if(toConsider, [&](auto const &item) {
            return m_useableCount_perShape.at(item[2]) == 0
                       ? true
                       : (m_useableCount_perShape.at(item[2]) + tolerance) < maxUseableCount;
        });
        toConsider.erase(last, end);

        auto const selectedID = m_fprng.pseudoRandom_0_to(toConsider.size() - 1);

        std::tuple<Pos, PastRes_t &> res{
            std::get<0>(m_frontierTiles.at(toConsider.at(selectedID).at(0))),
            std::get<2>(m_frontierTiles.at(toConsider.at(selectedID).at(0))).at(toConsider.at(selectedID).at(1))};


        auto const surrPoss = get_surrOverlappingPoss(std::get<0>(res));
        erase_fromFrontier(surrPoss);
        set_windowAtPos(std::get<0>(res), std::get<1>(res));
        add_toFrontier(surrPoss);

        // We used one
        m_useableCount_perShape[std::get<1>(res).first.y]--;
        return res;
    }
    std::vector<std::tuple<Pos, Shape_t &>>
    solve_XSteps(size_t numOfSteps = std::numeric_limits<size_t>::max(), size_t const &tolerance = 0uz) {
        std::vector<std::tuple<Pos, Shape_t>> res;
        while (numOfSteps-- > 0) {
            if (auto oneStepRes = solve_oneStep(tolerance)) {
                res.push_back(
                    {std::get<0>(oneStepRes.value()),
                     m_shapes_alterns.at(std::get<1>(oneStepRes.value()).y).at(std::get<1>(oneStepRes.value()).x)});
            }
            else { break; }
        }
        return res;
    }
};
} // namespace tetris


} // namespace solvers
} // namespace aoc
} // namespace incom