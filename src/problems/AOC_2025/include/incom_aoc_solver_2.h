#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
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

namespace packing_2 {
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

        auto
        operator<=>(Pos const &) const = default;
    };

    struct Shape {
        struct OverlayRes {
            size_t pointsAdded;
            size_t pointsOverlaid;
            size_t bordersTouching;
            size_t bordersNotTouching;

            double surfaceCovered_relative;
            double surfaceOpened_relative;

            Shape res_shp;
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

    struct PastRes_t_NEW {
        struct AltID {
            size_t shpID;
            size_t altrnID;
        };

        AltID             ids;
        Shape::OverlayRes olayRes;
    };
    using pastResMap_t_NEW =
        ankerl::unordered_dense::segmented_map<Shape, std::vector<PastRes_t_NEW>, incom::standard::hashing::XXH3Hasher>;

    using PastRes_t = std::pair<Pos, typename Shape::OverlayRes>; // Pos == [][] positions in 'm_shapes_alterns'
    using pastResMap_t =
        ankerl::unordered_dense::segmented_map<Shape, std::vector<PastRes_t>, incom::standard::hashing::XXH3Hasher>;


    // 1) Position (top left) in matrix, 2) What shape currently is at that position, 3) Possibilities

    struct FrontierPerSA {
        Pos           p;
        PastRes_t_NEW olPossib;
    };

    // By shape ID
    std::vector<std::vector<FrontierPerSA>> m_frontierPossibs;

    std::vector<std::tuple<Pos, std::reference_wrapper<Shape>, std::reference_wrapper<std::vector<PastRes_t>>>>
        m_frontierTiles;

private:
    std::vector<std::vector<char>> m_area;
    Pos                            m_firstTilePos;

    std::vector<std::vector<Shape>> m_shapes_alterns;
    size_t                          m_shapesMaxOccupied;
    std::vector<size_t>             m_useableCount_perShape;
    std::vector<double>             m_shapesRatios_orig;
    FastPseudoRandom                m_fprng;

    // Memoization of what 'shapes_alterns'
    pastResMap_t_NEW m_pastComputed;

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

    std::optional<Shape>
    get_windowAtPos(Pos const &shapePos) {
        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        if (shapePos.y >= 0 && shapePos.y <= (rows - SQSZ) && shapePos.x >= 0 && shapePos.x <= (cols - SQSZ)) {
            Shape res;
            for (int row = shapePos.y; row < shapePos.y + SQSZ; ++row) {
                for (int col = shapePos.x; col < (shapePos.x + SQSZ); ++col) {
                    res.m_matrix[row - shapePos.y][col - shapePos.x] = m_area[row][col];
                }
            }
            return res;
        }
        else { return std::nullopt; }
    }

    std::optional<std::vector<Shape>>
    get_windowsAtPos(std::vector<Pos> const &poss) {
        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        std::optional<std::vector<Shape>> res(std::vector<Shape>{});
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
    set_windowAtPos(Pos const &shapePos, Shape const &newShape) {
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
    add_toFrontier(std::vector<Pos> const &poss, std::vector<Shape> const &shps) {
        for (auto const &[oneShp, onePos] : std::views::zip(shps, poss)) {
            auto possibsForWindow = getOrCompute_possibsFor(oneShp);
            if (std::get<1>(possibsForWindow).size() > 0) {
                m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(oneShp), possibsForWindow));
            }
        }
    }


    std::vector<double>
    compute_perShapeScoringAdjustments() {
        auto ratiosHlprView = std::views::transform(
            std::views::zip(m_useableCount_perShape, m_shapesRatios_orig),
            [sum = static_cast<double>(std::ranges::fold_left_first(m_useableCount_perShape, std::plus{}).value_or(0))](
                auto const pr) {
                return (std::get<0>(pr) == 0uz ? std::numeric_limits<double>::max() : (sum / std::get<0>(pr))) *
                       std::get<1>(pr);
            });

        return std::vector<double>(ratiosHlprView.begin(), ratiosHlprView.end());
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

    BoxPacker_2D(size_t const area_ySize, size_t const area_xSize, std::vector<std::vector<Shape>> const &shps_alterns,
                 std::vector<size_t> const &shps_counts, size_t const firstTile_yPos = 0uz,
                 size_t const firstTile_xPos = 0uz, pastResMap_t_NEW const &pastReslts = {})
        : m_useableCount_perShape(shps_counts),
          m_area(std::vector(area_ySize + 2, std::vector<char>(area_xSize + 2, 0))),
          m_firstTilePos(Pos{.y = static_cast<long long>(firstTile_yPos), .x = static_cast<long long>(firstTile_xPos)}),
          m_shapes_alterns(shps_alterns), m_pastComputed(pastReslts), m_frontierPossibs(shps_alterns.size()) {

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

        auto const ftPos           = Pos{.y = static_cast<long long>(std::min(firstTile_yPos, area_ySize - SQSZ)),
                                         .x = static_cast<long long>(std::min(firstTile_xPos, area_xSize - SQSZ))};
        auto       firstTilePossib = getOrCompute_possibsFor(get_windowAtPos(ftPos).value());

    }

    BoxPacker_2D(size_t const &area_ySize, size_t const &area_xSize,
                 std::vector<std::array<std::array<bool, SQSZ - 2>, SQSZ - 2>> const &shps,
                 std::vector<size_t> const &shps_counts, size_t const firstTile_yPos = 0uz,
                 size_t const firstTile_xPos = 0uz, pastResMap_t_NEW const &pastReslts = {})
        : BoxPacker_2D(area_ySize, area_xSize,
                       std::views::transform(
                           shps, [](auto const &smallerShp) { return Shape(smallerShp).compute_alternsRotFlip(); }) |
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
    get_areaEmptyCount() {
        size_t res{};
        for (auto const &line : m_area) {
            for (char ch : line) { res += (ch == 0); }
        }
        return res;
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
    add_shape(Shape const &toAdd) {
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

        for (auto &ft : m_frontierPossibs) { ft.clear(); }

        auto const ftPos =
            Pos{.y = static_cast<long long>(m_firstTilePos.y), .x = static_cast<long long>(m_firstTilePos.x)};
        auto firstTilePossib = getOrCompute_possibsFor(get_windowAtPos(ftPos).value());

        add_toFrontierPossibs(ftPos, firstTilePossib);

        // m_frontierTiles.push_back(std::tuple_cat(std::make_tuple(m_firstTilePos),
        // getOrCompute_possibsFor(firstTile)));
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


    std::tuple<Shape &, std::vector<PastRes_t_NEW> &>
    getOrCompute_possibsFor(Shape const &tile) {
        auto insRes = m_pastComputed.insert({tile, std::vector<PastRes_t_NEW>{}});
        if (insRes.second) {
            auto &vpr = insRes.first->second;

            for (long long shpID = 0; shpID < m_shapes_alterns.size(); ++shpID) {
                for (long long alternID = 0; alternID < m_shapes_alterns.at(shpID).size(); ++alternID) {
                    auto rs =
                        PastRes_t_NEW{.ids{static_cast<size_t>(shpID), static_cast<size_t>(alternID)},
                                      .olayRes = tile.compute_overlayWith(m_shapes_alterns.at(shpID).at(alternID))};
                    if (rs.olayRes.pointsOverlaid == 0uz) { vpr.push_back(rs); }
                }
            }
            std::ranges::sort(vpr, [](auto const &l, auto const &r) -> bool {
                double const soDif = l.olayRes.surfaceOpened_relative - r.olayRes.surfaceOpened_relative;
                if (soDif == 0.0) { return l.olayRes.pointsAdded > r.olayRes.pointsAdded; }
                else { return soDif < 0; }
            });
        }
        return std::tie(insRes.first->first, insRes.first->second);
    }


    std::vector<std::tuple<Shape &, std::vector<PastRes_t_NEW> &>>
    getOrCompute_possibsFor(std::vector<Shape> const &tiles) {
        std::vector<std::tuple<Shape &, std::vector<PastRes_t_NEW> &>> res;
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
                              std::vector<std::tuple<Shape &, std::vector<PastRes_t_NEW> &>> const &possibsForWindows) {

        size_t res = 0uz;
        // auto const borders = calculate_windowBorderIndices(atPos);

        for (long long r = atPos.y; r < atPos.y + SQSZ; ++r) {
            for (long long c = atPos.x; c < atPos.x + SQSZ; ++c) {
                if (m_area.at(r).at(c) != 0) { continue; }
                auto const exe = [&]() -> bool {
                    for (auto const &[sp, pfw] : std::views::zip(surrPoss, possibsForWindows)) {
                        if (auto insidePos = relPos_insideWindowAtPos_minus1(Pos{r, c}, sp)) {
                            for (PastRes_t_NEW const &pr : std::get<1>(pfw)) {
                                if (pr.olayRes.res_shp.m_matrix.at(insidePos.value().y).at(insidePos.value().x) ==
                                        true &&
                                    m_useableCount_perShape.at(pr.ids.shpID) > 0) {
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

    std::pair<std::vector<std::optional<FrontierPerSA>>, std::vector<std::optional<std::pair<FrontierPerSA, size_t>>>>
    get_possibs() {
        auto res =
            std::make_pair(std::vector<std::optional<FrontierPerSA>>(m_shapes_alterns.size()),
                           std::vector<std::optional<std::pair<FrontierPerSA, size_t>>>(m_shapes_alterns.size()));


        for (size_t shpID = 0uz; shpID < m_shapes_alterns.size(); ++shpID) {
            if (m_useableCount_perShape.at(shpID) == 0uz) { continue; }
            for (size_t fpsaID = 0uz; fpsaID < m_frontierPossibs.at(shpID).size(); ++fpsaID) {
                FrontierPerSA const &one = m_frontierPossibs.at(shpID).at(fpsaID);

                auto const priorState = get_windowAtPos(one.p);
                set_windowAtPos(one.p, one.olPossib.olayRes.res_shp);

                auto const surrPoss          = get_surrOverlappingPoss(one.p);
                auto const possibsForWindows = getOrCompute_possibsFor(get_windowsAtPos(surrPoss).value());

                size_t unfillCount = calculate_unfillableCount(one.p, surrPoss, possibsForWindows);

                if (unfillCount == 0uz) {
                    res.first.at(shpID) = one;
                    set_windowAtPos(one.p, priorState.value());
                    break;
                }
                else {
                    auto &curUF = res.second.at(shpID);
                    if (not curUF || curUF.value().second > unfillCount) { curUF = {one, unfillCount}; }
                }
                set_windowAtPos(one.p, priorState.value());
            }
        }

        return res;
    }


    std::optional<FrontierPerSA>
    solve_oneStep() {
        if (m_frontierPossibs.size() == 0) { return std::nullopt; }

        auto [nonUnfilledPoss, unfilledPoss] = get_possibs();

        // Adjust SOR
        auto const PS_scoreAdj = compute_perShapeScoringAdjustments();
        for (size_t i = 0; i < PS_scoreAdj.size(); ++i) {
            if (nonUnfilledPoss.at(i)) {
                nonUnfilledPoss.at(i).value().olPossib.olayRes.surfaceOpened_relative *= PS_scoreAdj.at(i);
            }
            if (unfilledPoss.at(i)) {
                unfilledPoss.at(i).value().first.olPossib.olayRes.surfaceOpened_relative *= PS_scoreAdj.at(i);
            }
        }

        auto selectOne = [&]() -> std::optional<FrontierPerSA> {
            std::ranges::sort(nonUnfilledPoss, [](auto &l, auto &r) {
                if (not l) { return false; }
                else if (not r) { return true; }
                else {
                    return l.value().olPossib.olayRes.surfaceOpened_relative <
                           r.value().olPossib.olayRes.surfaceOpened_relative;
                }
            });

            // There is some nonUnfilled available
            if (nonUnfilledPoss.front()) { return nonUnfilledPoss.front(); }
            // There are only unfilled ones
            else {
                std::ranges::sort(unfilledPoss, [](auto &l, auto &r) {
                    if (not l) { return false; }
                    else if (not r) { return true; }
                    else if (l.value().second != r.value().second) {
                        return l.value().first.olPossib.olayRes.surfaceOpened_relative <
                               r.value().first.olPossib.olayRes.surfaceOpened_relative;
                    }
                    else { return l.value().second < r.value().second; }
                });

                if (not unfilledPoss.front()) { return std::nullopt; }
                return unfilledPoss.front().value().first;
            }
        };

        std::optional<FrontierPerSA> selected = selectOne();
        if (not selected) { return std::nullopt; }

        set_windowAtPos(selected.value().p, selected.value().olPossib.olayRes.res_shp);

        auto const surrPoss          = get_surrOverlappingPoss(selected.value().p);
        auto const possibsForWindows = getOrCompute_possibsFor(get_windowsAtPos(surrPoss).value());

        erase_fromFrontier(surrPoss);
        add_toFrontierPossibs(surrPoss, possibsForWindows);

        // We used one
        m_useableCount_perShape[selected->olPossib.ids.shpID]--;
        return selected;
    }
    std::vector<FrontierPerSA>
    solve_XSteps(size_t numOfSteps = std::numeric_limits<size_t>::max()) {
        std::vector<std::tuple<Pos, PastRes_t>> res;
        while (numOfSteps-- > 0) {
            if (auto oneStepRes = solve_oneStep()) { res.push_back(std::move(oneStepRes.value())); }
            else { break; }
        }
        return res;
    }
};
} // namespace packing_2


} // namespace aoc
} // namespace incom
