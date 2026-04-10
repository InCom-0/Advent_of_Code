#pragma once

#include <ankerl/unordered_dense.h>
#include <cassert>
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
requires(SQSZ > 1)
struct Shape {
    std::array<std::array<bool, SQSZ>, SQSZ> m_matrix = {};

    // 1) Resulting shape, 2) Points newly added, 3) Points 'overlaid'
    std::tuple<Shape, size_t, size_t>
    overlayWith(Shape const &other) {
        std::tuple<Shape, size_t, size_t> res{{}, 0, 0};
        for (size_t r = 0; r < SQSZ; ++r) {
            for (size_t c = 0; c < SQSZ; ++c) {
                std::get<2>(res)       += (m_matrix[r][c] and other[r][c]);
                std::get<1>(res)       += m_matrix[r][c] xor other[r][c];
                std::get<0>(res)[r][c]  = m_matrix[r][c] | other[r][c];
            }
        }
        return res;
    }
};

// ADL for hashing using XXH3Hasher
template <size_t SQSZ>
requires(SQSZ > 1)
constexpr void
XXH3Hash(Shape<SQSZ> const &input, XXH3_state_t *state) {
    XXH3_64bits_update(state, input.m_matrix.data(), sizeof(decltype(input.m_matrix)) * input.m_matrix.size());
}

template <size_t SQSZ>
class Solver {
public:
    using Shape_t = Shape<SQSZ>;

    struct Pos {
        size_t y = 0;
        size_t x = 0;
    };

    std::vector<std::pair<Pos, Shape_t>> borderSpaces = {{{0, 0}, {}}};


private:
    std::vector<Shape_t>              shapes;
    std::vector<std::vector<Shape_t>> shapes_alterns;
    std::vector<size_t>               useableCount_perShape;

    // Memoization of what 'shapes_alterns'
    ankerl::unordered_dense::map<Shape_t, std::vector<std::tuple<Shape_t, size_t, size_t>>> pastResults;


public:
    Solver() {}
    Solver(std::vector<Shape_t> const &shps, std::vector<std::vector<Shape_t>> const &shps_alterns,
           ankerl::unordered_dense::map<Shape_t, std::vector<std::tuple<Shape_t, size_t, size_t>>> const &pastReslts)
        : shapes(shps), shapes_alterns(shps_alterns), pastResults(pastReslts) {}


    // Returns true if this shape is actually new, False if it is the same as some other existing shape
    bool
    addShape(Shape<5> toAdd) {
        return true;
    }

    Solver
    clone_keepingShapeData() {
        return Solver(shapes, shapes_alterns, pastResults);
    }
};
} // namespace tetris


} // namespace solvers
} // namespace aoc
} // namespace incom