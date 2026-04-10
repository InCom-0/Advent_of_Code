#pragma once

#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

#include <ctre.hpp>
#include <flux.hpp>
#include <incom_aoc_solver.h>
#include <more_concepts/more_concepts.hpp>
#include <xxhash.h>

#include <incstd/incstd_all.hpp>


namespace incom {
namespace aoc {
namespace solvers {
namespace tetris {


template <size_t SQSZ>
requires(SQSZ > 2)
struct Shape {
    struct OverlayRes {
        Shape res_shp;

        size_t pointsAdded;
        size_t pointsOverlaid;
        size_t pointTouching;
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
                    res.pointTouching += m_matrix[r - 1][c] & (not other.m_matrix[r - 1][c]);
                    res.pointTouching += m_matrix[r][c - 1] & (not other.m_matrix[r][c - 1]);
                    res.pointTouching += m_matrix[r][c + 1] & (not other.m_matrix[r][c + 1]);
                    res.pointTouching += m_matrix[r + 1][c] & (not other.m_matrix[r + 1][c]);
                }
            }
        }

        return res;
    }

    std::vector<Shape>
    compute_alternsRotFlip() const {}
};

// ADL for hashing using XXH3Hasher
template <size_t SQSZ>
constexpr void
XXH3Hash(Shape<SQSZ> const &input, XXH3_state_t *state) {
    XXH3_64bits_update(state, input.m_matrix.data(), sizeof(decltype(input.m_matrix)) * input.m_matrix.size());
}

template <size_t SQSZ>
class Solver {
public:
    struct Pos {
        size_t y = 0;
        size_t x = 0;
    };
    using Shape_t   = Shape<SQSZ>;
    using PastRes_t = std::pair<Pos, typename Shape_t::OverlayRes>;


    std::vector<std::pair<Pos, Shape_t>> frontierTiles = {{{0, 0}, {}}};


private:
    std::vector<Shape_t>              shapes;
    std::vector<std::vector<Shape_t>> shapes_alterns;
    std::vector<size_t>               useableCount_perShape;

    // Memoization of what 'shapes_alterns'
    ankerl::unordered_dense::map<Shape_t, std::vector<PastRes_t>, incom::standard::hashing::XXH3Hasher> pastComputed;


    std::optional<std::reference_wrapper<std::vector<PastRes_t>>>
    get_computedFor(Shape_t const &tile) {
        if (auto found = pastComputed.find(tile); found != pastComputed.end()) { return std::ref(found->second); }
        return std::nullopt;
    }


public:
    Solver() {}
    Solver(std::vector<Shape_t> const &shps, std::vector<std::vector<Shape_t>> const &shps_alterns,
           ankerl::unordered_dense::map<Shape_t, std::vector<PastRes_t>> const &pastReslts)
        : shapes(shps), shapes_alterns(shps_alterns), pastComputed(pastReslts) {}


    // Returns true if this shape is actually new, False if it is the same as some other existing shape
    bool
    addShape(Shape_t const &toAdd) {
        return true;
    }

    Solver
    clone_keepShapeData() {
        return Solver(shapes, shapes_alterns, pastComputed);
    }

    std::vector<PastRes_t> &
    computeFor(Shape_t const &tile) {
        std::vector<PastRes_t> resToMap;
        auto allowed = [](PastRes_t const &toCheck) -> bool { return (toCheck.second.pointsOverlaid == 0uz); };

        for (size_t shpID = 0; shpID < shapes_alterns.size(); ++shpID) {
            for (size_t alternID = 0; alternID < shapes_alterns.at(shpID).size(); ++alternID) {
                auto rs =
                    PastRes_t{Pos{shpID, alternID}, tile.compute_overlayWith(shapes_alterns.at(shpID).at(alternID))};
                if (allowed(rs)) { resToMap.push_back(rs); }
            }
        }
        std::ranges::sort(resToMap, [](auto const &l, auto const &r) {
            return ((l.second.pointsAdded + l.second.pointTouching) > (r.second.pointsAdded + r.second.pointTouching));
        });
        auto insRes = pastComputed.insert({tile, resToMap});
        return insRes.first->second;
    }

    std::vector<PastRes_t> &
    getOrCompute(Shape_t const &tile) {
        if (auto comp = get_computedFor(tile); comp.has_value()) { return comp.value(); }
        return computeFor(tile);
    }


    Shape_t
    solve_oneStep() {
        std::vector<PastRes_t> possibilties;

        for (auto const &oneFT : frontierTiles) {

        }
    }
};
} // namespace tetris


} // namespace solvers
} // namespace aoc
} // namespace incom