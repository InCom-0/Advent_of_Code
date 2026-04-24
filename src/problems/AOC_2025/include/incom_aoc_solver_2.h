#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <shlobj.h>
#include <string>
#include <string_view>
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
    };

    struct AlternID {
        size_t shpID;
        size_t alternID;
    };

    struct Shape {
        struct OverlayRes {
            Shape res_shp;

            size_t pointsAdded;
            size_t pointsOverlaid;
            size_t bordersTouching;
            size_t bordersNotTouching;

            size_t pointsTouching;
            size_t pointsNotTouching;

            // The best overlays is where the 'empty pixels' form a continuous area, Gaps count measures how many such
            // areas there (ideal cases == 1)
            size_t gapsCount;

            // It is conveivable that overlay may produce a shape where there isn't just one contiguous 'filled pixels'
            // area
            // This variable measure how many such areas there are (ideal case == 1)
            size_t shapesCount;

            double surfacePointsCovered_relative = 0.0;
            double surfacePointsOpened_relative  = std::numeric_limits<double>::max();

            double surfaceCovered_relative = 0.0;
            double surfaceOpened_relative  = std::numeric_limits<double>::max();
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
            size_t const count = count_filled();
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
            OverlayRes res{};
            for (size_t r = 0; r < SQSZ; ++r) {
                for (size_t c = 0; c < SQSZ; ++c) {
                    res.pointsOverlaid         += (m_matrix[r][c] and other.m_matrix[r][c]);
                    res.pointsAdded            += (not m_matrix[r][c]) and other.m_matrix[r][c];
                    res.res_shp.m_matrix[r][c]  = m_matrix[r][c] | other.m_matrix[r][c];
                }
            }

            Shape Touch{};
            Shape NotTouch{};

            for (size_t r = 1; r < SQSZ - 1; ++r) {
                for (size_t c = 1; c < SQSZ - 1; ++c) {
                    if (other.m_matrix[r][c]) {
                        res.bordersTouching += m_matrix[r - 1][c] & (not other.m_matrix[r - 1][c]);
                        res.bordersTouching += m_matrix[r][c - 1] & (not other.m_matrix[r][c - 1]);
                        res.bordersTouching += m_matrix[r][c + 1] & (not other.m_matrix[r][c + 1]);
                        res.bordersTouching += m_matrix[r + 1][c] & (not other.m_matrix[r + 1][c]);

                        Touch.m_matrix[r - 1][c] |= m_matrix[r - 1][c] & (not other.m_matrix[r - 1][c]);
                        Touch.m_matrix[r][c - 1] |= m_matrix[r][c - 1] & (not other.m_matrix[r][c - 1]);
                        Touch.m_matrix[r][c + 1] |= m_matrix[r][c + 1] & (not other.m_matrix[r][c + 1]);
                        Touch.m_matrix[r + 1][c] |= m_matrix[r + 1][c] & (not other.m_matrix[r + 1][c]);

                        res.bordersNotTouching += (not m_matrix[r - 1][c]) & (not other.m_matrix[r - 1][c]);
                        res.bordersNotTouching += (not m_matrix[r][c - 1]) & (not other.m_matrix[r][c - 1]);
                        res.bordersNotTouching += (not m_matrix[r][c + 1]) & (not other.m_matrix[r][c + 1]);
                        res.bordersNotTouching += (not m_matrix[r + 1][c]) & (not other.m_matrix[r + 1][c]);

                        NotTouch.m_matrix[r - 1][c] |= not m_matrix[r - 1][c] & (not other.m_matrix[r - 1][c]);
                        NotTouch.m_matrix[r][c - 1] |= not m_matrix[r][c - 1] & (not other.m_matrix[r][c - 1]);
                        NotTouch.m_matrix[r][c + 1] |= not m_matrix[r][c + 1] & (not other.m_matrix[r][c + 1]);
                        NotTouch.m_matrix[r + 1][c] |= not m_matrix[r + 1][c] & (not other.m_matrix[r + 1][c]);
                    }
                }
            }

            Shape gapPastMemo;
            Shape filledPastMemo;
            Shape curMemo;

            Pos curPos{.y = 0ll, .x = 0ll};

            auto gapsRecLambda = [&](this auto const &self) -> bool {
                if (res.res_shp.m_matrix[curPos.y][curPos.x] == true) { return true; }
                if (curMemo.m_matrix[curPos.y][curPos.x] == true) { return true; }
                curMemo.m_matrix[curPos.y][curPos.x] = true;

                if (gapPastMemo.m_matrix[curPos.y][curPos.x] == true) { return false; } // We were there already
                gapPastMemo.m_matrix[curPos.y][curPos.x] = true;

                for (long long row : {-1ll, 1ll}) {
                    if (curPos.y + row < 0 || curPos.y + row >= SQSZ) { continue; }
                    curPos.y += row;
                    if (not self()) { return false; }
                    curPos.y -= row;
                }
                for (long long col : {-1ll, 1ll}) {
                    if (curPos.x + col < 0 || curPos.x + col >= SQSZ) { continue; }
                    curPos.x += col;
                    if (not self()) { return false; };
                    curPos.x -= col;
                }
                return true;
            };
            auto filledRecLambda = [&](this auto const &self) -> bool {
                if (res.res_shp.m_matrix[curPos.y][curPos.x] == false) { return true; }
                if (curMemo.m_matrix[curPos.y][curPos.x] == true) { return true; }
                curMemo.m_matrix[curPos.y][curPos.x] = true;

                if (filledPastMemo.m_matrix[curPos.y][curPos.x] == true) { return false; } // We were there already
                filledPastMemo.m_matrix[curPos.y][curPos.x] = true;

                for (long long row : {-1ll, 1ll}) {
                    if (curPos.y + row < 0 || curPos.y + row >= SQSZ) { continue; }
                    curPos.y += row;
                    if (not self()) { return false; }
                    curPos.y -= row;
                }
                for (long long col : {-1ll, 1ll}) {
                    if (curPos.x + col < 0 || curPos.x + col >= SQSZ) { continue; }
                    curPos.x += col;
                    if (not self()) { return false; };
                    curPos.x -= col;
                }
                return true;
            };

            for (size_t r = 0; r < SQSZ; ++r) {
                for (size_t c = 0; c < SQSZ; ++c) {
                    if (res.res_shp.m_matrix[r][c] == false && gapPastMemo.m_matrix[r][c] == false) {
                        curPos.y       = r;
                        curPos.x       = c;
                        curMemo        = Shape{};
                        res.gapsCount += gapsRecLambda();
                    }
                    if (res.res_shp.m_matrix[r][c] == true && filledPastMemo.m_matrix[r][c] == false) {
                        curPos.y         = r;
                        curPos.x         = c;
                        curMemo          = Shape{};
                        res.shapesCount += filledRecLambda();
                    }
                }
            }


            res.pointsTouching    = Touch.count_filled();
            res.pointsNotTouching = NotTouch.count_filled();

            res.surfacePointsCovered_relative =
                (static_cast<double>(res.pointsTouching) / (res.pointsTouching + res.pointsNotTouching));
            res.surfacePointsOpened_relative =
                (static_cast<double>(res.pointsNotTouching) / (res.pointsTouching + res.pointsNotTouching));

            res.surfaceCovered_relative =
                (static_cast<double>(res.bordersTouching) / (res.bordersTouching + res.bordersNotTouching));
            res.surfaceOpened_relative =
                (static_cast<double>(res.bordersNotTouching) / (res.bordersTouching + res.bordersNotTouching));

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

    struct PastRes {
        struct AlternID {
            size_t shpID;
            size_t alternID;
        };
        AlternID          ol_shpID;
        Shape::OverlayRes ol_res;
    };

    struct FrontierTile {
        // 1) Position (top left) in matrix, 2) What shape currently is at that position, 3) Possibilities
        Pos                                                       p;
        Shape                                                     curShp;
        std::reference_wrapper<std::vector<std::vector<PastRes>>> possibs;
    };

    using pastResMap_t = ankerl::unordered_dense::segmented_map<Shape, std::vector<std::vector<PastRes>>,
                                                                incom::standard::hashing::XXH3Hasher>;


    std::vector<std::vector<std::optional<std::reference_wrapper<std::vector<std::vector<PastRes>>>>>>
        m_frontierTiles_NEW;

private:
    std::vector<std::vector<char>> m_area;
    Pos                            m_firstTilePos;

    std::vector<std::vector<Shape>> m_shapes_alterns;
    size_t                          m_shapesMaxOccupied;
    std::vector<size_t>             m_useableCount_perShape;
    std::vector<double>             m_shapesRatios_orig;
    FastPseudoRandom                m_fprng;

    // Memoization of what 'OverlayRes' we can use on a particular 'Shape'
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

        // shapePos needs to be the Pos of some valid window in our area (but that will be implicit )
        for (long long rc = -(SQSZcpy - 2); rc < (SQSZcpy - 1); ++rc) {
            for (long long cc = -(SQSZcpy - 2); cc < (SQSZcpy - 1); ++cc) {
                long long const r_loc = shapePos.y + rc;
                long long const c_loc = shapePos.x + cc;

                if (r_loc >= 0 && r_loc <= (rows - SQSZ) && c_loc >= 0 && c_loc <= (cols - SQSZ)) {
                    res.push_back(Pos{.y = r_loc, .x = c_loc});
                }
            }
        }

        return res;
    }

    std::vector<Shape>
    get_surrOverlappingWindows(Pos const &shapePos) {
        std::vector<Pos> res;


        size_t const rows = m_area.size();
        size_t const cols = m_area.size() > 0 ? m_area.front().size() : 0;

        constexpr long long SQSZcpy = static_cast<long long>(SQSZ);

        // shapePos needs to be the Pos of some valid window in our area (but that will be implicit )
        for (long long rc = -(SQSZcpy - 1); rc < (SQSZcpy); ++rc) {
            for (long long cc = -(SQSZcpy - 1); cc < (SQSZcpy); ++cc) {
                long long const r_loc = shapePos.y + rc;
                long long const c_loc = shapePos.x + cc;

                if (r_loc >= 0 && r_loc <= (rows - SQSZ) && c_loc >= 0 && c_loc <= (cols - SQSZ)) {
                    res.push_back(Pos{.y = r_loc, .x = c_loc});
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
    set_windowAtPos(Pos const &shapePos, PastRes const &pr) {
        if (not is_posValid(shapePos)) { return false; }
        for (long long r = shapePos.y; r < (shapePos.y + SQSZ); ++r) {
            for (long long c = shapePos.x; c < (shapePos.x + SQSZ); ++c) {
                m_area[r][c] = pr.ol_res.res_shp.m_matrix[r - shapePos.y][c - shapePos.x];
            }
        }
        return true;
    }
    bool
    set_windowAtPos(Pos const &shapePos, Shape const &newWindow) {
        if (not is_posValid(shapePos)) { return false; }
        for (long long r = shapePos.y; r < (shapePos.y + SQSZ); ++r) {
            for (long long c = shapePos.x; c < (shapePos.x + SQSZ); ++c) {
                m_area[r][c] = newWindow.m_matrix[r - shapePos.y][c - shapePos.x];
            }
        }
        return true;
    }


    size_t
    erase_fromFrontier(std::vector<Pos> const &shapePoss) {
        size_t res_removed = 0uz;
        for (Pos const &onePos : shapePoss) {
            if (m_frontierTiles_NEW.at(onePos.y).at(onePos.x) != std::nullopt) { res_removed++; }
            m_frontierTiles_NEW.at(onePos.y).at(onePos.x) = std::nullopt;
        }
        return res_removed;
    }

    size_t
    add_toFrontier(std::vector<Pos> const &shapePoss) {
        size_t resCount = 0uz;
        for (auto const &onePos : shapePoss) {
            auto window = get_windowAtPos(onePos);
            if (not window.has_value() || window.value().count_filledBorderLess() > m_shapesMaxOccupied) { continue; }

            auto possibsForWindow = getOrCompute_possibsFor(window.value());
            if (std::get<1>(possibsForWindow).size() > 0) {
                m_frontierTiles_NEW.at(onePos.y).at(onePos.x) = std::ref(std::get<1>(possibsForWindow));
            }
            resCount++;
        }
        return resCount;
    }

    std::vector<double>
    compute_perShapeScoringAdjustments() {
        auto ratiosHlprView = std::views::transform(
            m_useableCount_perShape,
            [&, id = 0uz,
             sum = static_cast<double>(std::ranges::fold_left_first(m_useableCount_perShape, std::plus{}).value_or(0))](
                size_t oneCount) mutable {
                return (oneCount == 0uz ? std::numeric_limits<double>::max() : (sum / oneCount)) *
                       m_shapesRatios_orig.at(id++);
            });

        // This wierd adjustment is to prevent the solver f
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
                 size_t const firstTile_xPos = 0uz, pastResMap_t const &pastReslts = {})
        : m_useableCount_perShape(shps_counts),
          m_area(std::vector(area_ySize + 2, std::vector<char>(area_xSize + 2, 0))),
          m_frontierTiles_NEW(std::vector(
              area_ySize + 2,
              std::vector<std::optional<std::reference_wrapper<std::vector<std::vector<PastRes>>>>>(area_xSize + 2))),
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


        auto ft_possibs = getOrCompute_possibsFor(firstTile);

        m_frontierTiles_NEW.at(ftPos.y).at(ftPos.x) = std::ref(std::get<1>(ft_possibs));
    }

    BoxPacker_2D(size_t const &area_ySize, size_t const &area_xSize,
                 std::vector<std::array<std::array<bool, SQSZ - 2>, SQSZ - 2>> const &shps,
                 std::vector<size_t> const &shps_counts, size_t const firstTile_yPos = 0uz,
                 size_t const firstTile_xPos = 0uz, pastResMap_t const &pastReslts = {})
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
        for (auto &frontierLine : m_frontierTiles_NEW) {
            for (auto &frontierPos : frontierLine) { frontierPos = std::nullopt; }
        }
        auto ft_possibs                                               = getOrCompute_possibsFor(firstTile);
        m_frontierTiles_NEW.at(m_firstTilePos.y).at(m_firstTilePos.x) = std::ref(std::get<1>(ft_possibs));
    }
    void
    reset_frontier(Pos const &firstTilePos) {
        auto const ftPos = Pos{.y = static_cast<long long>(std::min(firstTilePos.y, m_area.size() - SQSZ)),
                               .x = static_cast<long long>(
                                   std::min(firstTilePos.y, (m_area.size() > 0 ? m_area.front().size() : 0) - SQSZ))};
        m_firstTilePos   = ftPos;
        reset_frontier();
    }

    void
    reset_frontier(std::vector<Pos> const &firstTiles) {}

    void
    reset_useableShapeCounts(std::vector<size_t> const &shps_counts) {
        m_useableCount_perShape = shps_counts;
        m_useableCount_perShape.resize(m_shapes_alterns.size(), 0);
    }

    void
    reset_pastComputed() {
        m_pastComputed.clear();
    }


    std::tuple<Shape &, std::vector<std::vector<PastRes>> &>
    getOrCompute_possibsFor(Shape const &tile) {
        auto insRes = m_pastComputed.insert({tile, std::vector<std::vector<PastRes>>(m_shapes_alterns.size())});
        if (insRes.second) {
            std::vector<std::vector<PastRes>> &vpr = insRes.first->second;
            std::vector<double>                lastSORs(m_shapes_alterns.size(), std::numeric_limits<double>::max());

            auto allowed = [&](PastRes const &toCheck) -> bool {
                if (toCheck.ol_res.pointsOverlaid == 0uz) { return true; }
                else { return false; }
            };

            for (size_t shpID = 0uz; shpID < m_shapes_alterns.size(); ++shpID) {
                for (size_t alternID = 0uz; alternID < m_shapes_alterns.at(shpID).size(); ++alternID) {
                    auto rs = PastRes{.ol_shpID{shpID, alternID},
                                      .ol_res = tile.compute_overlayWith(m_shapes_alterns.at(shpID).at(alternID))};
                    if (allowed(rs)) { vpr.at(shpID).push_back(rs); }
                }
            }
            for (auto &vprLine : vpr) {
                std::ranges::sort(vprLine, [](auto const &l, auto const &r) -> bool {
                    double const soDif = r.ol_res.surfaceOpened_relative - l.ol_res.surfaceOpened_relative;
                    if (soDif == 0.0) { return l.ol_res.pointsAdded > r.ol_res.pointsAdded; }
                    else { return std::abs(soDif) + soDif; }
                });
            }
        }
        return std::tie(insRes.first->first, insRes.first->second);
    }

    std::vector<std::tuple<Shape &, std::vector<PastRes> &>>
    getOrCompute_possibsFor(std::vector<Shape> const &tiles) {
        std::vector<std::tuple<Shape &, std::vector<PastRes> &>> res;
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

    struct ConsideredShapeOption {
        enum class Type : uint8_t {
            Gapless = 1,
            Dividing,
            Gapcreating
        };

        Pos     p;
        PastRes pr_option = PastRes{.ol_shpID = {}, .ol_res = {}};

        Type   type             = Type::Gapcreating;
        size_t uncoverableCount = std::numeric_limits<size_t>::max();

        bool found = false;
    };


    size_t
    calculate_uncoveredPossibilityAt(Pos const &p) {
        std::vector<Pos>                                         surrPoss = get_surrOverlappingPoss(p);
        std::tuple<Shape &, std::vector<std::vector<PastRes>> &> possibsForWindows =
            getOrCompute_possibsFor(get_windowsAtPos(surrPoss).value());

        for (auto const &sp_line : surrPoss) {}
    }

    std::optional<std::tuple<Pos, PastRes>>
    solve_oneStep() {
        std::vector<std::vector<std::array<size_t, 4>>> toConsider_OLD(m_shapes_alterns.size());
        std::vector<double> lastSORs(m_shapes_alterns.size(), std::numeric_limits<double>::max());


        std::vector<std::vector<ConsideredShapeOption>> toConsider(
            m_shapes_alterns.size(), std::vector<ConsideredShapeOption>(1, ConsideredShapeOption{}));

        ConsideredShapeOption co;

        auto findBestGaplessOpts = [&]() -> void {
            for (long long r = 0ll; auto const &frontierLine : m_frontierTiles_NEW) {
                for (long long c = 0ll; auto const &frontierPos : frontierLine) {
                    if (frontierPos == std::nullopt) { continue; }
                    std::vector<std::vector<PastRes>> const &vv_pr2 = frontierPos.value().get();

                    for (auto const &v_pr2 : vv_pr2) {
                        for (PastRes const &pr : v_pr2) {
                            // Only one (or none) gaps and one shape

                            if (pr.ol_res.gapsCount < 2 && pr.ol_res.shapesCount == 1) {
                                if (toConsider.at(pr.ol_shpID.shpID)
                                        .front()
                                        .pr_option.ol_res.surfacePointsOpened_relative >
                                    pr.ol_res.surfacePointsOpened_relative) {
                                    toConsider.at(pr.ol_shpID.shpID).clear();
                                    toConsider.at(pr.ol_shpID.shpID)
                                        .push_back(ConsideredShapeOption{.p         = {.y = r, .x = c},
                                                                         .pr_option = pr,
                                                                         .type  = ConsideredShapeOption::Type::Gapless,
                                                                         .found = true});
                                }
                                else if (toConsider.at(pr.ol_shpID.shpID)
                                             .front()
                                             .pr_option.ol_res.surfacePointsOpened_relative ==
                                         pr.ol_res.surfacePointsOpened_relative) {
                                    toConsider.at(pr.ol_shpID.shpID)
                                        .push_back(ConsideredShapeOption{.p         = {.y = r, .x = c},
                                                                         .pr_option = pr,
                                                                         .type  = ConsideredShapeOption::Type::Gapless,
                                                                         .found = true});
                                }
                                break; // Break because we don't need to evaluate the rest
                            }
                        }
                    }
                    c++;
                }
                r++;
            }
        };

        auto findbestGappedOpt = [&]() -> void {
            for (long long r = 0ll; auto const &frontierLine : m_frontierTiles_NEW) {
                for (long long c = 0ll; auto const &frontierPos : frontierLine) {
                    if (frontierPos == std::nullopt) { continue; }
                    std::vector<std::vector<PastRes>> const &vv_pr2 = frontierPos.value().get();

                    for (auto const &v_pr2 : vv_pr2) {
                        for (PastRes const &pr : v_pr2) {
                            // Only more than one gaps
                            if (pr.ol_res.gapsCount > 1) {
                                Pos const  curPos{.y = r, .x = c};
                                auto const prevWindowState = get_windowAtPos(curPos);

                                set_windowAtPos(curPos, pr);
                                size_t const uncoverable = calculate_uncoveredPossibilityAt(curPos);

                                bool input = false;
                                if (toConsider.at(pr.ol_shpID.shpID).front().uncoverableCount > uncoverable) {
                                    toConsider.at(pr.ol_shpID.shpID).clear();
                                    input = true;
                                }

                                else if (toConsider.at(pr.ol_shpID.shpID).front().uncoverableCount == uncoverable) {
                                    if (toConsider.at(pr.ol_shpID.shpID)
                                            .front()
                                            .pr_option.ol_res.surfacePointsOpened_relative >
                                        pr.ol_res.surfacePointsOpened_relative) {

                                        toConsider.at(pr.ol_shpID.shpID).clear();
                                        input = true;
                                    }
                                    else if (toConsider.at(pr.ol_shpID.shpID)
                                                 .front()
                                                 .pr_option.ol_res.surfacePointsOpened_relative ==
                                             pr.ol_res.surfacePointsOpened_relative) {
                                        input = true;
                                    }
                                }

                                if (input) {
                                    toConsider.at(pr.ol_shpID.shpID)
                                        .push_back(ConsideredShapeOption{.p         = {.y = r, .x = c},
                                                                         .pr_option = pr,
                                                                         .type  = ConsideredShapeOption::Type::Gapless,
                                                                         .found = true});
                                }

                                set_windowAtPos(curPos, prevWindowState);
                            }
                        }
                    }
                    c++;
                }
                r++;
            }
        };


        findBestGaplessOpts();
        // No 'gapless' overlays available, we need to look more thoroughly
        if (std::ranges::all_of(toConsider, [](auto const &item) { return item.found == false; })) {
            findbestGappedOpt();
        }

        //  There are none viable overlays ... can't solve any more
        if (std::ranges::all_of(toConsider, [](auto const &item) { return item.found == false; })) {
            return std::nullopt;
        }


        // for (size_t ft_i = 0uz; auto const &oneFT : m_frontierTiles) {

        //     std::vector<char> tracker(m_shapes_alterns.size(), 0);
        //     for (size_t alt_i = 0uz; alt_i < oneFT.possibs.get().size(); ++alt_i) {
        //         PastRes const &pr = oneFT.possibs.get().at(alt_i);

        //         if (std::ranges::fold_left_first(tracker, std::bit_and{}).value_or(0) == 1) { break; }
        //         else if (m_useableCount_perShape.at(pr.ol_shpID.shpID) == 0 ||
        //                  pr.ol_res.surfaceOpened_relative > lastSORs.at(pr.ol_shpID.shpID)) {
        //             continue;
        //         }
        //         else if (pr.ol_res.surfaceOpened_relative < lastSORs.at(pr.ol_shpID.shpID)) {
        //             lastSORs.at(pr.ol_shpID.shpID) = pr.ol_res.surfaceOpened_relative;
        //             toConsider_OLD.at(pr.ol_shpID.shpID).clear();
        //         }
        //         tracker.at(pr.ol_shpID.shpID) = 1;
        //         toConsider_OLD.at(pr.ol_shpID.shpID)
        //             .push_back({ft_i, alt_i, static_cast<size_t>(pr.ol_shpID.shpID),
        //                         static_cast<size_t>(pr.ol_shpID.alternID)});
        //     }
        //     ft_i++;
        // }
        //  There are none viable overlays ... can't solve any more
        if (std::ranges::all_of(toConsider_OLD, [](auto const &toConsLine) { return toConsLine.empty(); })) {
            return std::nullopt;
        }

        // This 'wierd' adjustment is to make sure the solver selects shapes more evenly
        for (auto const &[oneSOR, psa] : std::views::zip(lastSORs, compute_perShapeScoringAdjustments())) {
            oneSOR *= psa;
        }

        size_t const selEleToConsider  = std::ranges::min_element(lastSORs, std::less()) - lastSORs.begin();
        size_t const selEleAnternative = m_fprng.pseudoRandom_0_to(toConsider_OLD.at(selEleToConsider).size() - 1);

        std::tuple<Pos, PastRes> res{};
        // std::tuple<Pos, PastRes> res{
        //     m_frontierTiles.at(toConsider_OLD.at(selEleToConsider).at(selEleAnternative).at(0)).p,
        //     m_frontierTiles.at(toConsider_OLD.at(selEleToConsider).at(selEleAnternative).at(0))
        //         .possibs.get()
        //         .at(toConsider_OLD.at(selEleToConsider).at(selEleAnternative).at(1))};


        auto const surrPoss = get_surrOverlappingPoss(std::get<0>(res));
        erase_fromFrontier(surrPoss);
        set_windowAtPos(std::get<0>(res), std::get<1>(res));
        add_toFrontier(surrPoss);

        // We used one
        m_useableCount_perShape[std::get<1>(res).ol_shpID.shpID]--;
        return res;
    }

    std::vector<std::tuple<Pos, PastRes>>
    solve_XSteps(size_t numOfSteps = std::numeric_limits<size_t>::max()) {
        std::vector<std::tuple<Pos, PastRes>> res;
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
