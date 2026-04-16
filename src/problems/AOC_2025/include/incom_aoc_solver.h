#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <incstd/incstd_all.hpp>


namespace incom {
namespace aoc {

using namespace incom::standard;

namespace packing {
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
class BoxPacker_2D {
public:
    struct Pos {
        long long y = 0;
        long long x = 0;
    };
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

        auto
        operator<=>(Shape const &other) const = default;


        Shape(std::array<std::array<bool, SQSZ - 2>, SQSZ - 2> const &src) {
            for (auto r = 1uz; auto const &line : src) {
                for (auto c = 1uz; bool one : line) { m_matrix[r][c++] = one; }
                r++;
            }
        }
        Shape(std::array<std::array<bool, SQSZ>, SQSZ> const &src) : m_matrix(src) {}

        int
        is_emptyOrFilled() {
            size_t count = 0uz;
            for (int r = 0; r < SQSZ; ++r) {
                for (int c = 0; c < SQSZ; ++c) { count += m_matrix[r][c++]; }
                r++;
            }
            if (count == 0) { return -1; }
            else if (count == (SQSZ * SQSZ)) { return 1; }
            return 0;
        }

        size_t
        count_filled() {
            size_t count = 0uz;
            for (int r = 0; r < SQSZ; ++r) {
                for (int c = 0; c < SQSZ; ++c) { count += m_matrix[r][c++]; }
                r++;
            }
            return count;
        }

        size_t
        count_filledBorderLess() {
            size_t count = 0uz;
            for (int r = 1; r < (SQSZ - 1); ++r) {
                for (int c = 1; c < (SQSZ - 1); ++c) { count += m_matrix[r][c++]; }
                r++;
            }
            return count;
        }

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

        // ADL for hashing using XXH3Hasher
        friend constexpr void
        XXH3Hash(Shape const &input, XXH3_state_t *state) {
            XXH3_64bits_update(state, input.m_matrix.data(), sizeof(decltype(input.m_matrix)));
        }
    };


    using Shape_t   = Shape;
    using PastRes_t = std::pair<Pos, typename Shape_t::OverlayRes>; // Pos == [][] positions in 'm_shapes_alterns'
    using pastResMap_t =
        ankerl::unordered_dense::segmented_map<Shape_t, std::vector<PastRes_t>, incom::standard::hashing::XXH3Hasher>;


    // 1) Position (top left) in matrix, 2) What shape currently is at that position, 3) Possibilities
    std::vector<std::tuple<Pos, std::reference_wrapper<Shape_t>, std::reference_wrapper<std::vector<PastRes_t>>>>
        m_frontierTiles;

private:
    std::vector<std::vector<char>> m_area;
    Pos                            m_firstTilePos;

    std::vector<std::vector<Shape_t>> m_shapes_alterns;
    size_t                            m_shapesMaxOccupied;
    std::vector<size_t>               m_useableCount_perShape;
    std::vector<double>               m_shapesRatios_orig;
    FastPseudoRandom                  m_fprng;

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
                    if (r_loc >= 0 && r_loc <= (rows - SQSZ) && c_loc >= 0 && c_loc <= (cols - SQSZ)) {
                        res.push_back(Pos{.y = r_loc, .x = c_loc});
                    }
                }
            }
        }
        return res;
    }

    std::optional<Shape_t>
    get_windowAtPos(Pos const &shapePos) {
        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        if (shapePos.y >= 0 && shapePos.y <= (rows - SQSZ) && shapePos.x >= 0 && shapePos.x <= (cols - SQSZ)) {
            Shape_t res;
            for (int row = shapePos.y; row < shapePos.y + SQSZ; ++row) {
                for (int col = shapePos.x; col < (shapePos.x + SQSZ); ++col) {
                    res.m_matrix[row - shapePos.y][col - shapePos.x] = m_area[row][col];
                }
            }
            return res;
        }
        else { return std::nullopt; }
    }

    std::optional<std::vector<Shape_t>>
    get_windowsAtPos(std::vector<Pos> const &poss) {
        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        std::optional<std::vector<Shape_t>> res(std::vector<Shape_t>{});
        for (auto const &onePos : poss) {
            if (onePos.y >= 0 && onePos.y <= (rows - SQSZ) && onePos.x >= 0 && onePos.x <= (cols - SQSZ)) {
                res.value().emplace_back();
                for (int row = onePos.y; row < onePos.y + SQSZ; ++row) {
                    for (int col = onePos.x; col < (onePos.x + SQSZ); ++col) {
                        res.value().back().m_matrix[row - onePos.y][col - onePos.x] = m_area[row][col];
                    }
                }
            }
            else {
                res = std::nullopt;
                break;
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
    set_windowAtPos(Pos const &shapePos, Shape_t const &newShape) {
        if (not is_posValid(shapePos)) { return false; }
        for (long long r = shapePos.y; r < (shapePos.y + SQSZ); ++r) {
            for (long long c = shapePos.x; c < (shapePos.x + SQSZ); ++c) {
                m_area[r][c] = newShape.m_matrix[r - shapePos.y][c - shapePos.x];
            }
        }
        return true;
    }


    size_t
    erase_fromFrontier(std::vector<Pos> const &shapePoss) {
        auto const [ite_first, ite_last] = std::ranges::remove_if(m_frontierTiles, [&](auto &tpl) {
            return std::ranges::find_if(shapePoss, [&](Pos const &onePos) {
                       return ((std::get<0>(tpl).y == onePos.y) && (std::get<0>(tpl).x == onePos.x));
                   }) != shapePoss.end();
        });
        size_t const removed             = ite_last - ite_first;
        m_frontierTiles.erase(ite_first, ite_last);
        return removed;
    }

    size_t
    add_toFrontier(std::vector<Pos> const &poss) {
        size_t resCount = 0uz;
        for (auto const &onePos : poss) {
            auto window = get_windowAtPos(onePos);
            if (not window.has_value() || window.value().count_filledBorderLess() > m_shapesMaxOccupied) { continue; }

            auto possibsForWindow = getOrCompute_possibsFor(window.value());
            if (std::get<1>(possibsForWindow).size() > 0) {
                m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(onePos), possibsForWindow));
            }
            resCount++;
        }
        return resCount;
    }
    void
    add_toFrontier(std::vector<Pos> const &poss, std::vector<Shape_t> const &shps) {
        for (auto const &[oneShp, onePos] : std::views::zip(shps, poss)) {
            auto possibsForWindow = getOrCompute_possibsFor(oneShp);
            if (std::get<1>(possibsForWindow).size() > 0) {
                m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(oneShp), possibsForWindow));
            }
        }
    }


    auto
    compute_perShapeScoringAdjustments() {
        auto ratiosHlprView = std::views::transform(
            std::views::zip(m_useableCount_perShape, m_shapesRatios_orig),
            [&, sum = static_cast<double>(
                    std::ranges::fold_left_first(m_useableCount_perShape, std::plus{}).value_or(0))](auto const pr) {
                return std::pow(
                    (std::get<0>(pr) == 0uz ? std::numeric_limits<double>::max() : (sum / std::get<0>(pr))) *
                        std::get<1>(pr),
                    2);
            });

        return decltype(m_shapesRatios_orig)(ratiosHlprView.begin(), ratiosHlprView.end());
    }


public:
    // Construction
    BoxPacker_2D()                        = delete;
    BoxPacker_2D(BoxPacker_2D const &src) = default;
    BoxPacker_2D(BoxPacker_2D &&src)      = default;
    ~BoxPacker_2D()                       = default;

    BoxPacker_2D &
    operator=(BoxPacker_2D const &) = default;
    BoxPacker_2D &
    operator=(BoxPacker_2D &&) = default;

    BoxPacker_2D(size_t const area_ySize, size_t const area_xSize,
                 std::vector<std::vector<Shape_t>> const &shps_alterns, std::vector<size_t> const &shps_counts,
                 size_t const firstTile_yPos = 0uz, size_t const firstTile_xPos = 0uz,
                 pastResMap_t const &pastReslts = {})
        : m_useableCount_perShape(shps_counts),
          m_area(std::vector(area_ySize + 2, std::vector<char>(area_xSize + 2, 0))),
          m_firstTilePos(Pos{.y = static_cast<long long>(firstTile_yPos), .x = static_cast<long long>(firstTile_xPos)}),
          m_shapes_alterns(shps_alterns), m_pastComputed(pastReslts) {

        std::ranges::fill(m_area.front(), 1);
        for (auto &line : std::views::take(m_area, m_area.size() - 1) | std::views::drop(1)) {
            line.front() = 1;
            line.back()  = 1;
        }
        std::ranges::fill(m_area.back(), 1);

        m_useableCount_perShape.resize(m_shapes_alterns.size(), 0uz);
        auto ratiosHlprView = std::views::transform(
            m_useableCount_perShape,
            [sum = static_cast<double>(
                 std::ranges::fold_left_first(m_useableCount_perShape, std::plus{}).value_or(nextafter(0.0, 1.0)))](
                size_t oneCount) { return oneCount / sum; });

        m_shapesRatios_orig = decltype(m_shapesRatios_orig)(ratiosHlprView.begin(), ratiosHlprView.end());

        m_shapesMaxOccupied =
            std::ranges::fold_left_first(
                std::views::transform(m_shapes_alterns,
                                      [](auto &vecOfAlterns) { return vecOfAlterns.front().count_filledBorderLess(); }),
                [](size_t a, size_t b) { return std::max(a, b); })
                .value_or(0uz);

        auto const ftPos     = Pos{.y = static_cast<long long>(std::min(firstTile_yPos, area_ySize - SQSZ)),
                                   .x = static_cast<long long>(std::min(firstTile_xPos, area_xSize - SQSZ))};
        auto       firstTile = get_windowAtPos(ftPos).value();

        m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(ftPos), getOrCompute_possibsFor(firstTile)));
    }

    BoxPacker_2D(size_t const &area_ySize, size_t const &area_xSize,
                 std::vector<std::array<std::array<bool, SQSZ - 2>, SQSZ - 2>> const &shps,
                 std::vector<size_t> const &shps_counts, size_t const firstTile_yPos = 0uz,
                 size_t const firstTile_xPos = 0uz, pastResMap_t const &pastReslts = {})
        : BoxPacker_2D(area_ySize, area_xSize,
                       std::views::transform(
                           shps, [](auto const &smallerShp) { return Shape_t(smallerShp).compute_alternsRotFlip(); }) |
                           std::ranges::to<std::vector>(),
                       shps_counts, firstTile_yPos, firstTile_xPos, pastReslts) {}

    // 'Primes' random number generator used by the solver instance with a seed based on hash of the solver state
    // Returns the the seed it used
    size_t
    prime_fprng() {
        size_t const res = hash_stateOfSelf();
        set_pseudoRandomSeed(res);
        return res;
    }

    size_t
    get_useableShapeCountRemaining() {
        return std::ranges::fold_left_first(m_useableCount_perShape, std::plus()).value_or(0uz);
    }
    size_t
    get_pastResSize() {
        return m_pastComputed.size();
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

    BoxPacker_2D
    clone_keepShapeData(std::vector<size_t> const &shps_counts) {
        return BoxPacker_2D(m_area.size(), m_area.size() > 0 ? m_area.front().size() : 0uz, m_shapes_alterns,
                            shps_counts, m_firstTilePos.y, m_firstTilePos.x, m_pastComputed);
    }
    BoxPacker_2D
    clone_keepShapeData(size_t const area_ySize, size_t const area_xSize, std::vector<size_t> const &shps_counts) {
        return BoxPacker_2D(area_ySize, area_xSize, m_shapes_alterns, shps_counts, m_firstTilePos.y, m_firstTilePos.x,
                            {});
    }

    void
    reset_allButNotPastComputed(std::vector<size_t> const &shps_counts) {
        reset_area();
        reset_frontier();
        reset_useableShapeCounts(shps_counts);
    }

    void
    reset_allButNotPastComputed(size_t area_ySize, size_t area_xSize, std::vector<size_t> const &shps_counts) {
        reset_area(area_ySize, area_xSize);
        reset_frontier();
        reset_useableShapeCounts(shps_counts);
    }

    void
    reset_allButNotPastComputed(size_t area_ySize, size_t area_xSize, std::vector<size_t> const &shps_counts,
                                Pos const &p) {
        reset_area(area_ySize, area_xSize);
        reset_frontier(p);
        reset_useableShapeCounts(shps_counts);
    }

    void
    reset_area() {
        std::ranges::fill(m_area.front(), 1);
        for (auto &line : std::views::take(m_area, m_area.size() - 1) | std::views::drop(1)) {
            std::ranges::fill(line, 0);
            line.front() = 1;
            line.back()  = 1;
        }
        std::ranges::fill(m_area.back(), 1);
    }

    void
    reset_area(size_t area_ySize, size_t area_xSize) {
        m_area.resize(area_ySize);
        for (auto &areaLine : m_area) { areaLine.resize(area_xSize); }
        reset_area();
    }

    void
    reset_frontier() {
        auto firstTile = get_windowAtPos(m_firstTilePos).value();
        m_frontierTiles.clear();
        m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(m_firstTilePos), getOrCompute_possibsFor(firstTile)));
    }

    void
    reset_frontier(Pos const &p) {
        auto const ftPos =
            Pos{.y = static_cast<long long>(std::min(p.y, m_area.size() - SQSZ)),
                .x = static_cast<long long>(std::min(p.y, (m_area.size() > 0 ? m_area.front().size() : 0) - SQSZ))};
        m_firstTilePos = ftPos;
        reset_frontier();
    }

    void
    reset_useableShapeCounts(std::vector<size_t> const &shps_counts) {
        m_useableCount_perShape = shps_counts;
        m_useableCount_perShape.resize(m_shapes_alterns.size(), 0);

        auto ratiosHlprView = std::views::transform(
            m_useableCount_perShape,
            [sum = static_cast<double>(
                 std::ranges::fold_left_first(m_useableCount_perShape, std::plus{}).value_or(nextafter(0.0, 1.0)))](
                size_t oneCount) { return oneCount / sum; });

        m_shapesRatios_orig = decltype(m_shapesRatios_orig)(ratiosHlprView.begin(), ratiosHlprView.end());
    }

    void
    reset_pastComputed() {
        m_pastComputed.clear();
    }


    std::tuple<Shape_t &, std::vector<PastRes_t> &>
    getOrCompute_possibsFor(Shape_t const &tile) {
        auto insRes = m_pastComputed.insert({tile, std::vector<PastRes_t>{}});
        if (insRes.second) {
            auto               &vpr = insRes.first->second;
            std::vector<double> lastSORs(m_shapes_alterns.size(), std::numeric_limits<double>::max());

            auto allowed = [&](PastRes_t const &toCheck) -> bool {
                if (lastSORs.at(toCheck.first.y) >= toCheck.second.surfaceOpened_relative &&
                    toCheck.second.pointsOverlaid == 0uz) {
                    lastSORs.at(toCheck.first.y) = toCheck.second.surfaceOpened_relative;
                    return true;
                }
                else { return false; }
            };

            for (long long shpID = 0; shpID < m_shapes_alterns.size(); ++shpID) {
                for (long long alternID = 0; alternID < m_shapes_alterns.at(shpID).size(); ++alternID) {
                    auto rs = PastRes_t{Pos{shpID, alternID},
                                        tile.compute_overlayWith(m_shapes_alterns.at(shpID).at(alternID))};
                    if (allowed(rs)) { vpr.push_back(rs); }
                }
            }
            std::ranges::sort(vpr, [](auto const &l, auto const &r) -> bool {
                double const soDif = r.second.surfaceOpened_relative - l.second.surfaceOpened_relative;
                if (soDif == 0.0) { return l.second.pointsAdded > r.second.pointsAdded; }
                else { return std::abs(soDif) + soDif; }
            });
        }
        return std::tie(insRes.first->first, insRes.first->second);
    }


    std::vector<std::tuple<Shape_t &, std::vector<PastRes_t> &>>
    getOrCompute_possibsFor(std::vector<Shape_t> const &tiles) {
        std::vector<std::tuple<Shape_t &, std::vector<PastRes_t> &>> res;
        for (auto const &tile : tiles) { res.push_back(getOrCompute_possibsFor(tile)); }
        return res;
    }

    std::string
    get_areaState() {
        std::string toPrint{};
        for (auto const &line : m_area) {
            constexpr std::array<char, 2> map{46, 35};

            auto r = std::views::transform(line, [&](char oneCh) -> char { return map[oneCh]; });
            toPrint.append(std::format("{:s}\n", r));
        }
        return toPrint;
    }


    std::pair<std::vector<std::vector<std::array<size_t, 4>>>, std::vector<size_t>>
    calculate_nextStepsToConsider() {

        std::pair<std::vector<std::vector<std::array<size_t, 4>>>, std::vector<size_t>> res{
            std::vector<std::vector<std::array<size_t, 4>>>(m_shapes_alterns.size()),
            std::views::iota(0uz, m_shapes_alterns.size()) | std::ranges::to<std::vector>()};

        std::vector<double> lastSORs(m_shapes_alterns.size(), std::numeric_limits<double>::max());

        // This 'wierd' adjustment is to make sure the solver selects shapes more evenly
        auto const perShape_adjustments = compute_perShapeScoringAdjustments();

        for (size_t ft_i = 0uz; auto const &oneFT : m_frontierTiles) {

            std::vector<char> tracker(m_shapes_alterns.size(), 0);
            for (size_t alt_i = 0uz; alt_i < std::get<2>(oneFT).get().size(); ++alt_i) {
                auto const &[alternsPos, overlay] = std::get<2>(oneFT).get().at(alt_i);
                auto const adjSOR = overlay.surfaceOpened_relative * perShape_adjustments.at(alternsPos.y);

                if (std::ranges::fold_left_first(tracker, std::bit_and{}).value_or(0) == 1) { break; }
                else if (m_useableCount_perShape.at(alternsPos.y) == 0 || adjSOR > lastSORs.at(alternsPos.y)) {
                    continue;
                }
                else if (adjSOR < lastSORs.at(alternsPos.y)) {
                    lastSORs.at(alternsPos.y) = adjSOR;
                    res.first.at(alternsPos.y).clear();
                }
                tracker.at(alternsPos.y) = 1;
                res.first.at(alternsPos.y)
                    .push_back({ft_i, alt_i, static_cast<size_t>(alternsPos.y), static_cast<size_t>(alternsPos.x)});
            }
            ft_i++;
        }

        std::ranges::sort(res.second, [&](auto &a, auto &b) { return lastSORs[a] < lastSORs[b]; });
        return res;
    }

    std::vector<Pos>
    calculate_windowBorderIndices(Pos const &atPos) {
        std::vector<Pos> res;
        for (long long c = atPos.x; c < (atPos.x + SQSZ); ++c) { res.emplace_back(atPos.y, c); }
        for (long long r = atPos.y + 1; r < (atPos.y + SQSZ - 1); ++r) {
            res.emplace_back(r, atPos.x);
            res.emplace_back(r, atPos.x + SQSZ - 1);
        }
        for (long long c = atPos.x; c < (atPos.x + SQSZ); ++c) { res.emplace_back(atPos.y + SQSZ - 1, c); }
        return res;
    }

    bool
    relPos_insideWindowAtPos(Pos const &p, Pos const &windowAtPos) {
        std::optional<Pos> res{p.y - windowAtPos.y, p.x - windowAtPos.x};
        if (res.y < 0 || res.y > SQSZ - 1 || res.x < 0 || res.x > SQSZ - 1) { res = std::nullopt; }
        return res;
    }

    std::optional<Pos>
    relPos_insideWindowAtPos_minus1(Pos const &p, Pos const &windowAtPos) {
        std::optional<Pos> res{Pos{p.y - windowAtPos.y, p.x - windowAtPos.x}};
        if (res.value().y < 1 || res.value().y > SQSZ - 2 || res.value().x < 1 || res.value().x > SQSZ - 2) {
            res = std::nullopt;
        }
        return res;
    }


    size_t
    calculate_unfillableCount(Pos const &atPos, std::vector<Pos> const &surrPoss,
                              std::vector<std::tuple<Shape_t &, std::vector<PastRes_t> &>> const &possibsForWindows) {

        size_t     res     = 0uz;
        // auto const borders = calculate_windowBorderIndices(atPos);

        for (long long r = atPos.y; r < atPos.y + SQSZ; ++r) {
            for (long long c = atPos.x; c < atPos.x + SQSZ; ++c) {
                if (m_area.at(r).at(c) != 0) { continue; }
                auto const exe = [&]() -> bool {
                    for (auto const &[sp, pfw] : std::views::zip(surrPoss, possibsForWindows)) {
                        if (auto insidePos = relPos_insideWindowAtPos_minus1(Pos{r, c}, sp)) {
                            for (PastRes_t const &pr : std::get<1>(pfw)) {
                                if (pr.second.res_shp.m_matrix.at(insidePos.value().y).at(insidePos.value().x) ==
                                        true &&
                                    m_useableCount_perShape.at(pr.first.y) > 0) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                };
                res += exe();
            }
        }

        return res;
    }


    std::optional<std::tuple<Pos, PastRes_t>>
    solve_oneStep() {
        auto toConsider_pair = calculate_nextStepsToConsider();

        std::vector<std::pair<std::array<unsigned long long, 4>, size_t>> toReconsider;

        std::tuple<Pos, PastRes_t> res{};
        std::vector<Pos>           surrPoss{};

        auto exec = [&]() -> std::optional<bool> {
            for (auto selETC : toConsider_pair.second) {
                // Randomly rotate before before so that we start with a random shape alternative per shape
                std::ranges::rotate(toConsider_pair.first.at(selETC),
                                    toConsider_pair.first.at(selETC).begin() +
                                        m_fprng.pseudoRandom_0_to(toConsider_pair.first.at(selETC).size() == 0
                                                                      ? 0
                                                                      : toConsider_pair.first.at(selETC).size() - 1));

                for (auto const &eleAlt : toConsider_pair.first.at(selETC)) {
                    res = {std::get<0>(m_frontierTiles.at(eleAlt.at(0))),
                           std::get<2>(m_frontierTiles.at(eleAlt.at(0))).get().at(eleAlt.at(1))};

                    auto const priorState = get_windowAtPos(std::get<0>(res));
                    set_windowAtPos(std::get<0>(res), std::get<1>(res).second.res_shp);

                    surrPoss                 = get_surrOverlappingPoss(std::get<0>(res));
                    auto   possibsForWindows = getOrCompute_possibsFor(get_windowsAtPos(surrPoss).value());
                    size_t unfillableCount   = calculate_unfillableCount(std::get<0>(res), surrPoss, possibsForWindows);
                    if (unfillableCount == 0) { return true; }
                    else {
                        set_windowAtPos(std::get<0>(res), priorState.value());
                        toReconsider.push_back({eleAlt, unfillableCount});
                    }
                }
            }
            // Need to reconsider as we can't do 'gapless placement'

            std::ranges::sort(toReconsider, [](auto &a, auto &b) { return a.second < b.second; });
            for (auto const &recons_ele : toReconsider) {
                res = {std::get<0>(m_frontierTiles.at(recons_ele.first.at(0))),
                       std::get<2>(m_frontierTiles.at(recons_ele.first.at(0))).get().at(recons_ele.first.at(1))};
                set_windowAtPos(std::get<0>(res), std::get<1>(res).second.res_shp);
                surrPoss = get_surrOverlappingPoss(std::get<0>(res));
                return false;
            }

            return std::nullopt; // This should never really happen
        };

        if (not exec()) { return std::nullopt; };
        erase_fromFrontier(surrPoss);
        add_toFrontier(surrPoss);

        // We used one
        m_useableCount_perShape[std::get<1>(res).first.y]--;
        return res;
    }
    std::vector<std::tuple<Pos, PastRes_t>>
    solve_XSteps(size_t numOfSteps = std::numeric_limits<size_t>::max()) {
        std::vector<std::tuple<Pos, PastRes_t>> res;
        while (numOfSteps-- > 0) {
            if (auto oneStepRes = solve_oneStep()) { res.push_back(std::move(oneStepRes.value())); }
            else { break; }
        }
        return res;
    }
};
} // namespace packing


/*
Elimination-first solver for small systems of sum equations over non-negative integers.

Model:
- Each equation is a sum of named variables with implicit coefficient 1.
- Variables are restricted to non-negative integer values.
- Repeating the same variable inside one equation is allowed and acts as a larger coefficient.

Usage example:

    incom::aoc::solver s;
    s.build().input('a').input('c').input('d').equals(7);
    s.build().input('d').input('e').equals(5);

    auto best = s.solve();      // minimum-sum solution, if one exists
    auto all  = s.solve_all();  // every solution, ordered by total sum

The implementation first reduces the equation system into pivot variables expressed as affine functions of
free variables. Search then happens only over the free variables rather than over every original variable.
This is much more effective for underdetermined systems such as 13 variables constrained by 10 equations.
*/
class solver {
private:
    struct rational {
        long long numerator   = 0;
        long long denominator = 1;

        rational() = default;

        rational(long long numeratorIn, long long denominatorIn = 1)
            : numerator(numeratorIn), denominator(denominatorIn) {
            assert(denominator != 0);
            normalize();
        }

        [[nodiscard]] bool
        is_zero() const {
            return numerator == 0;
        }

        [[nodiscard]] bool
        is_integer() const {
            return denominator == 1;
        }

        [[nodiscard]] long long
        as_integer() const {
            assert(is_integer());
            return numerator;
        }

        void
        normalize() {
            if (denominator < 0) {
                numerator   = -numerator;
                denominator = -denominator;
            }

            auto const divisor = std::gcd(numerator, denominator);
            if (divisor != 0) {
                numerator   /= divisor;
                denominator /= divisor;
            }
        }
    };

    friend rational
    operator+(rational lhs, rational const &rhs) {
        lhs.numerator    = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
        lhs.denominator *= rhs.denominator;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator-(rational lhs, rational const &rhs) {
        lhs.numerator    = lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
        lhs.denominator *= rhs.denominator;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator-(rational value) {
        return rational(-value.numerator, value.denominator);
    }

    friend rational
    operator*(rational lhs, rational const &rhs) {
        lhs.numerator   *= rhs.numerator;
        lhs.denominator *= rhs.denominator;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator*(rational lhs, long long rhs) {
        lhs.numerator *= rhs;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator*(long long lhs, rational rhs) {
        rhs.numerator *= lhs;
        rhs.normalize();
        return rhs;
    }

    friend rational
    operator/(rational lhs, rational const &rhs) {
        assert(rhs.numerator != 0);
        lhs.numerator   *= rhs.denominator;
        lhs.denominator *= rhs.numerator;
        lhs.normalize();
        return lhs;
    }

    friend bool
    operator==(rational const &lhs, rational const &rhs) {
        return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
    }

    friend bool
    operator<(rational const &lhs, rational const &rhs) {
        return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
    }

    friend bool
    operator>(rational const &lhs, rational const &rhs) {
        return rhs < lhs;
    }

    friend bool
    operator<=(rational const &lhs, rational const &rhs) {
        return not(rhs < lhs);
    }

    friend bool
    operator>=(rational const &lhs, rational const &rhs) {
        return not(lhs < rhs);
    }

    // One linear equation of the form x1 + x2 + ... + xn = rhs.
    struct equation {
        std::vector<size_t> variableIds;
        int                 rhs      = 0;
        bool                isClosed = false;
    };

    struct affine_expression {
        rational              constant{};
        std::vector<rational> freeCoefficients;
    };

    // Preprocessed information extracted from elimination.
    struct prepared_problem {
        std::vector<size_t>            variableOrder;
        std::vector<int>               staticUpperBounds;
        std::vector<int>               freeIndexByVariable;
        std::vector<size_t>            pivotVariables;
        std::vector<affine_expression> variableExpressions;
        rational                       objectiveConstant{};
        std::vector<rational>          objectiveWeights;
        bool                           isValid = true;
    };

public:
    // Concrete assignment returned by solve()/solve_all().
    struct solution {
        std::map<std::string, int> values;
        int                        sum = 0;

        // Convenience lookup by variable name. Asserts if the variable was never part of the system.
        [[nodiscard]] int
        value_of(std::string_view variableName) const {
            auto found = values.find(std::string(variableName));
            assert(found != values.end());
            return found->second;
        }

        // Convenience overload for one-character variable names.
        [[nodiscard]] int
        value_of(char variableName) const {
            return value_of(std::string_view(&variableName, 1));
        }
    };

    // Fluent builder used to define one equation term-by-term.
    class equation_builder {
    public:
        equation_builder(solver &owner, size_t equationId) : m_owner(owner), m_equationId(equationId) {}

        // Adds one occurrence of a variable to the current equation.
        // Calling input("x") twice means 2*x in the final equation.
        equation_builder &
        input(std::string_view variableName) {
            assert(not variableName.empty());
            auto &equation = m_owner.m_equations.at(m_equationId);
            assert(not equation.isClosed);
            equation.variableIds.push_back(m_owner.get_orCreateVariableId(variableName));
            return *this;
        }

        // Convenience overload for one-character variable names.
        equation_builder &
        input(char variableName) {
            return input(std::string_view(&variableName, 1));
        }

        // Finalizes the current equation as sum(inputs...) = rhs.
        solver &
        equals(int rhs) {
            auto &equation = m_owner.m_equations.at(m_equationId);
            assert(not equation.isClosed);
            assert(rhs >= 0);
            equation.rhs      = rhs;
            equation.isClosed = true;
            return m_owner;
        }

    private:
        solver &m_owner;
        size_t  m_equationId;
    };

    // Starts definition of a new equation.
    [[nodiscard]] equation_builder
    build() {
        m_equations.push_back(equation{});
        return equation_builder(*this, m_equations.size() - 1);
    }

    // Returns one solution with minimum total variable sum, if the system is solvable.
    [[nodiscard]] std::optional<solution>
    solve() const {
        return solve_minSum();
    }

    // Same as solve(); kept as the explicit name for callers that want to emphasize the objective.
    [[nodiscard]] std::optional<solution>
    solve_minSum() const {
        auto prepared = prepare_problem();
        if (not prepared.isValid) { return std::nullopt; }

        std::vector<int>        assignedFreeValues(prepared.variableOrder.size(), 0);
        std::vector<bool>       assignedFreeMask(prepared.variableOrder.size(), false);
        std::optional<solution> bestSolution;
        std::optional<int>      bestSum;

        search_minSum(prepared, assignedFreeValues, assignedFreeMask, 0, bestSolution, bestSum);
        return bestSolution;
    }

    // Enumerates every non-negative integer solution and returns them ordered by total sum.
    [[nodiscard]] std::vector<solution>
    solve_all() const {
        auto prepared = prepare_problem();
        if (not prepared.isValid) { return {}; }

        std::vector<int>      assignedFreeValues(prepared.variableOrder.size(), 0);
        std::vector<bool>     assignedFreeMask(prepared.variableOrder.size(), false);
        std::vector<solution> allSolutions;

        search_all(prepared, assignedFreeValues, assignedFreeMask, 0, allSolutions);

        std::ranges::sort(allSolutions, [](solution const &lhs, solution const &rhs) {
            if (lhs.sum != rhs.sum) { return lhs.sum < rhs.sum; }
            return lhs.values < rhs.values;
        });

        return allSolutions;
    }

private:
    std::vector<equation>                   m_equations;
    std::vector<std::string>                m_variableNames;
    std::unordered_map<std::string, size_t> m_variableIdByName;

    // Interns a variable name into a compact numeric id used by the search code.
    [[nodiscard]] size_t
    get_orCreateVariableId(std::string_view variableName) {
        auto variableNameStr = std::string(variableName);
        if (auto found = m_variableIdByName.find(variableNameStr); found != m_variableIdByName.end()) {
            return found->second;
        }

        auto const newId = m_variableNames.size();
        m_variableNames.push_back(variableNameStr);
        m_variableIdByName.emplace(std::move(variableNameStr), newId);
        return newId;
    }

    // Converts the equation system into exact row-reduced echelon form using rational arithmetic.
    void
    reduce_to_rref(std::vector<std::vector<rational>> &matrix, std::vector<int> &pivotRowByVariable) const {
        size_t pivotRow = 0;
        for (size_t columnId = 0; columnId < pivotRowByVariable.size() && pivotRow < matrix.size(); ++columnId) {
            auto candidateRow = pivotRow;
            while (candidateRow < matrix.size() && matrix.at(candidateRow).at(columnId).is_zero()) { candidateRow++; }
            if (candidateRow == matrix.size()) { continue; }

            std::swap(matrix.at(pivotRow), matrix.at(candidateRow));

            auto const pivotValue = matrix.at(pivotRow).at(columnId);
            for (auto &cell : matrix.at(pivotRow)) { cell = cell / pivotValue; }

            for (size_t rowId = 0; rowId < matrix.size(); ++rowId) {
                if (rowId == pivotRow || matrix.at(rowId).at(columnId).is_zero()) { continue; }

                auto const factor = matrix.at(rowId).at(columnId);
                for (size_t innerColumnId = columnId; innerColumnId < matrix.at(rowId).size(); ++innerColumnId) {
                    matrix.at(rowId).at(innerColumnId) =
                        matrix.at(rowId).at(innerColumnId) - factor * matrix.at(pivotRow).at(innerColumnId);
                }
            }

            pivotRowByVariable.at(columnId) = static_cast<int>(pivotRow);
            pivotRow++;
        }
    }

    // Reduces the original equations to row-reduced echelon form and extracts pivot-variable expressions
    // in terms of the free variables. Search then only needs to enumerate the free variables.
    [[nodiscard]] prepared_problem
    prepare_problem() const {
        prepared_problem result;
        if (m_equations.empty() || m_variableNames.empty()) {
            result.isValid = false;
            return result;
        }

        std::vector<int>                   occurrences(m_variableNames.size(), 0);
        int                                globalUpperBound = 0;
        std::vector<std::vector<rational>> reducedMatrix(m_equations.size(),
                                                         std::vector<rational>(m_variableNames.size() + 1, rational{}));

        for (size_t rowId = 0; rowId < m_equations.size(); ++rowId) {
            auto const &equation = m_equations.at(rowId);
            if (not equation.isClosed || equation.variableIds.empty() || equation.rhs < 0) {
                result.isValid = false;
                return result;
            }

            globalUpperBound = std::max(globalUpperBound, equation.rhs);
        }

        result.staticUpperBounds = std::vector<int>(m_variableNames.size(), globalUpperBound);

        for (size_t rowId = 0; rowId < m_equations.size(); ++rowId) {
            auto const &equation = m_equations.at(rowId);

            std::vector<int> localCoefficients(m_variableNames.size(), 0);
            for (auto variableId : equation.variableIds) {
                localCoefficients.at(variableId)++;
                occurrences.at(variableId)++;
            }

            for (size_t variableId = 0; variableId < localCoefficients.size(); ++variableId) {
                auto const coefficient = localCoefficients.at(variableId);
                if (coefficient == 0) { continue; }

                auto const maxValue                     = equation.rhs / coefficient;
                result.staticUpperBounds.at(variableId) = std::min(result.staticUpperBounds.at(variableId), maxValue);
                reducedMatrix.at(rowId).at(variableId)  = rational(coefficient);
            }

            reducedMatrix.at(rowId).back() = rational(equation.rhs);
        }

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            if (result.staticUpperBounds.at(variableId) < 0 || occurrences.at(variableId) == 0) {
                result.isValid = false;
                return result;
            }
        }

        auto pivotRowByVariable = std::vector<int>(m_variableNames.size(), -1);
        reduce_to_rref(reducedMatrix, pivotRowByVariable);

        for (auto const &row : reducedMatrix) {
            auto allCoefficientsZero = true;
            for (size_t columnId = 0; columnId < m_variableNames.size(); ++columnId) {
                if (not row.at(columnId).is_zero()) {
                    allCoefficientsZero = false;
                    break;
                }
            }

            if (allCoefficientsZero && not row.back().is_zero()) {
                result.isValid = false;
                return result;
            }
        }

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            if (pivotRowByVariable.at(variableId) == -1) { result.variableOrder.push_back(variableId); }
            else { result.pivotVariables.push_back(variableId); }
        }

        std::ranges::sort(result.variableOrder, [&](size_t lhs, size_t rhs) {
            if (result.staticUpperBounds.at(lhs) != result.staticUpperBounds.at(rhs)) {
                return result.staticUpperBounds.at(lhs) < result.staticUpperBounds.at(rhs);
            }
            if (occurrences.at(lhs) != occurrences.at(rhs)) { return occurrences.at(lhs) > occurrences.at(rhs); }
            return m_variableNames.at(lhs) < m_variableNames.at(rhs);
        });

        result.freeIndexByVariable = std::vector<int>(m_variableNames.size(), -1);
        for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
            result.freeIndexByVariable.at(result.variableOrder.at(freeIndex)) = static_cast<int>(freeIndex);
        }

        result.variableExpressions = std::vector<affine_expression>(m_variableNames.size());
        result.objectiveWeights    = std::vector<rational>(result.variableOrder.size(), rational{});
        for (auto &expression : result.variableExpressions) {
            expression.freeCoefficients = std::vector<rational>(result.variableOrder.size(), rational{});
        }

        for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
            auto const variableId = result.variableOrder.at(freeIndex);
            result.variableExpressions.at(variableId).freeCoefficients.at(freeIndex) = rational(1);
        }

        for (auto pivotVariableId : result.pivotVariables) {
            auto const pivotRow   = static_cast<size_t>(pivotRowByVariable.at(pivotVariableId));
            auto      &expression = result.variableExpressions.at(pivotVariableId);

            expression.constant = reducedMatrix.at(pivotRow).back();
            for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
                auto const freeVariableId                 = result.variableOrder.at(freeIndex);
                expression.freeCoefficients.at(freeIndex) = -reducedMatrix.at(pivotRow).at(freeVariableId);
            }
        }

        for (auto const &expression : result.variableExpressions) {
            result.objectiveConstant = result.objectiveConstant + expression.constant;
            for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
                result.objectiveWeights.at(freeIndex) =
                    result.objectiveWeights.at(freeIndex) + expression.freeCoefficients.at(freeIndex);
            }
        }

        return result;
    }

    // Computes the best possible objective value reachable from the current partial assignment.
    // Unassigned free variables are given whichever endpoint would help the objective most.
    [[nodiscard]] bool
    is_feasible(prepared_problem const &prepared, std::vector<int> const &assignedFreeValues,
                std::vector<bool> const &assignedFreeMask, std::optional<int> bestSum) const {
        auto optimisticObjective = prepared.objectiveConstant;
        for (size_t freeIndex = 0; freeIndex < prepared.variableOrder.size(); ++freeIndex) {
            if (assignedFreeMask.at(freeIndex)) {
                optimisticObjective =
                    optimisticObjective + prepared.objectiveWeights.at(freeIndex) * assignedFreeValues.at(freeIndex);
                continue;
            }

            if (prepared.objectiveWeights.at(freeIndex) < rational{}) {
                optimisticObjective =
                    optimisticObjective + prepared.objectiveWeights.at(freeIndex) *
                                              prepared.staticUpperBounds.at(prepared.variableOrder.at(freeIndex));
            }
        }

        if (bestSum.has_value() && optimisticObjective >= rational(bestSum.value())) { return false; }

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            auto valueMin = prepared.variableExpressions.at(variableId).constant;
            auto valueMax = prepared.variableExpressions.at(variableId).constant;

            for (size_t freeIndex = 0; freeIndex < prepared.variableOrder.size(); ++freeIndex) {
                auto const coefficient = prepared.variableExpressions.at(variableId).freeCoefficients.at(freeIndex);
                if (coefficient.is_zero()) { continue; }

                if (assignedFreeMask.at(freeIndex)) {
                    auto const contribution = coefficient * assignedFreeValues.at(freeIndex);
                    valueMin                = valueMin + contribution;
                    valueMax                = valueMax + contribution;
                    continue;
                }

                auto const freeUpperBound = prepared.staticUpperBounds.at(prepared.variableOrder.at(freeIndex));
                if (coefficient > rational{}) { valueMax = valueMax + coefficient * freeUpperBound; }
                else { valueMin = valueMin + coefficient * freeUpperBound; }
            }

            if (valueMax < rational{}) { return false; }
            if (valueMin > rational(prepared.staticUpperBounds.at(variableId))) { return false; }
        }

        return true;
    }

    // Evaluates the reduced solution representation for a complete free-variable assignment.
    [[nodiscard]] std::optional<solution>
    materialize_solution(prepared_problem const &prepared, std::vector<int> const &assignedFreeValues) const {
        solution result;
        int      sum = 0;

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            auto value = prepared.variableExpressions.at(variableId).constant;
            for (size_t freeIndex = 0; freeIndex < prepared.variableOrder.size(); ++freeIndex) {
                value = value + prepared.variableExpressions.at(variableId).freeCoefficients.at(freeIndex) *
                                    assignedFreeValues.at(freeIndex);
            }

            if (not value.is_integer()) { return std::nullopt; }

            auto const integerValue = value.as_integer();
            if (integerValue < 0 || integerValue > prepared.staticUpperBounds.at(variableId)) { return std::nullopt; }

            result.values.emplace(m_variableNames.at(variableId), static_cast<int>(integerValue));
            sum += static_cast<int>(integerValue);
        }

        result.sum = sum;
        return result;
    }

    [[nodiscard]] bool
    is_better_solution(solution const &candidate, std::optional<solution> const &bestSolution) const {
        if (not bestSolution.has_value()) { return true; }
        if (candidate.sum != bestSolution->sum) { return candidate.sum < bestSolution->sum; }
        return candidate.values < bestSolution->values;
    }

    // Depth-first search over free variables only. Pivot variables are reconstructed afterwards.
    void
    search_minSum(prepared_problem const &prepared, std::vector<int> &assignedFreeValues,
                  std::vector<bool> &assignedFreeMask, size_t depth, std::optional<solution> &bestSolution,
                  std::optional<int> &bestSum) const {
        if (not is_feasible(prepared, assignedFreeValues, assignedFreeMask, bestSum)) { return; }

        if (depth == prepared.variableOrder.size()) {
            auto candidate = materialize_solution(prepared, assignedFreeValues);
            if (candidate.has_value() && is_better_solution(candidate.value(), bestSolution)) {
                bestSolution = candidate;
                bestSum      = bestSolution->sum;
            }
            return;
        }

        auto const variableId = prepared.variableOrder.at(depth);
        auto const freeIndex  = static_cast<size_t>(prepared.freeIndexByVariable.at(variableId));
        auto const upperBound = prepared.staticUpperBounds.at(variableId);

        if (prepared.objectiveWeights.at(freeIndex) < rational{}) {
            for (int candidateValue = upperBound; candidateValue >= 0; --candidateValue) {
                assignedFreeValues.at(freeIndex) = candidateValue;
                assignedFreeMask.at(freeIndex)   = true;
                search_minSum(prepared, assignedFreeValues, assignedFreeMask, depth + 1, bestSolution, bestSum);
                assignedFreeMask.at(freeIndex) = false;
            }
        }
        else {
            for (int candidateValue = 0; candidateValue <= upperBound; ++candidateValue) {
                assignedFreeValues.at(freeIndex) = candidateValue;
                assignedFreeMask.at(freeIndex)   = true;
                search_minSum(prepared, assignedFreeValues, assignedFreeMask, depth + 1, bestSolution, bestSum);
                assignedFreeMask.at(freeIndex) = false;
            }
        }

        assignedFreeValues.at(freeIndex) = 0;
    }

    // Enumerates all feasible assignments for the free variables and reconstructs the pivot variables.
    void
    search_all(prepared_problem const &prepared, std::vector<int> &assignedFreeValues,
               std::vector<bool> &assignedFreeMask, size_t depth, std::vector<solution> &allSolutions) const {
        if (not is_feasible(prepared, assignedFreeValues, assignedFreeMask, std::nullopt)) { return; }

        if (depth == prepared.variableOrder.size()) {
            auto candidate = materialize_solution(prepared, assignedFreeValues);
            if (candidate.has_value()) { allSolutions.push_back(candidate.value()); }
            return;
        }

        auto const variableId = prepared.variableOrder.at(depth);
        auto const freeIndex  = static_cast<size_t>(prepared.freeIndexByVariable.at(variableId));
        auto const upperBound = prepared.staticUpperBounds.at(variableId);

        for (int candidateValue = 0; candidateValue <= upperBound; ++candidateValue) {
            assignedFreeValues.at(freeIndex) = candidateValue;
            assignedFreeMask.at(freeIndex)   = true;
            search_all(prepared, assignedFreeValues, assignedFreeMask, depth + 1, allSolutions);
            assignedFreeMask.at(freeIndex) = false;
        }

        assignedFreeValues.at(freeIndex) = 0;
    }
};


} // namespace aoc
} // namespace incom
