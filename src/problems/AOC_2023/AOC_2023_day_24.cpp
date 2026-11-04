#include <algorithm>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <string>


#include <Mathematics/IntrRay2Ray2.h>
#include <ankerl/unordered_dense.h>


namespace AOC2023 {
auto
day24_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"([\-]?\d+)">;
    return incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, any_ctre);
}

size_t
day24_1(std::string dataFile) {

    auto input = day24_0(dataFile);

    std::vector<gte::Ray<2, double>> rays;

    double minPos = 200000000000000;
    double maxPos = 400000000000000;

    gte::FIQuery<double, gte::Ray<2, double>, gte::Ray<2, double>> RayTI{};

    for (auto const &inpLine : input) {
        rays.push_back(gte::Ray<2, double>{{std::stod(inpLine[0]), std::stod(inpLine[1])},
                                           {std::stod(inpLine[3]), std::stod(inpLine[4])}});
    }

    size_t intersections{};
    for (auto const &[l1, l2] : incstd::views::combinations_k<2uz>(rays)) {
        auto ld_res = RayTI(l1, l2);

        if (ld_res.point[0][0] >= minPos && ld_res.point[0][0] <= maxPos && ld_res.point[0][1] >= minPos &&
            ld_res.point[0][1] <= maxPos) {
            if (ld_res.intersect) { intersections++; }
        }
    }

    return intersections;
}

size_t
day24_2(std::string dataFile) {
    auto input = day24_0(dataFile);
    if (input.size() < 3) { throw std::runtime_error("Need at least three hailstones"); }

    using Vec = std::array<long double, 3>;

    auto cross = [](Vec const &a, Vec const &b) -> Vec {
        return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
    };

    std::array<Vec, 3> positions{}, velocities{};
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            positions[i][j]  = std::stold(input[i][j]);
            velocities[i][j] = std::stold(input[i][j + 3]);
        }
    }

    // Work relative to the first hailstone and scale the unknowns.
    long double posScale = 0, velScale = 0;
    for (size_t i = 1; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            posScale = std::max(posScale, std::abs(positions[i][j] - positions[0][j]));
            velScale = std::max(velScale, std::abs(velocities[i][j] - velocities[0][j]));
        }
    }
    if (posScale == 0 || velScale == 0) {
        throw std::runtime_error("Degenerate observations; try different hailstones");
    }

    // Augmented matrix: six unknowns and one right-hand side.
    std::array<std::array<long double, 7>, 6> matrix{};

    for (size_t i = 1; i < 3; ++i) {
        Vec dp{}, dv{};
        for (size_t j = 0; j < 3; ++j) {
            dp[j] = (positions[i][j] - positions[0][j]) / posScale;
            dv[j] = (velocities[i][j] - velocities[0][j]) / velScale;
        }

        Vec    rhs = cross(dp, dv);
        size_t r   = 3 * (i - 1);

        matrix[r]     = {0, dv[2], -dv[1], 0, -dp[2], dp[1], rhs[0]};
        matrix[r + 1] = {-dv[2], 0, dv[0], dp[2], 0, -dp[0], rhs[1]};
        matrix[r + 2] = {dv[1], -dv[0], 0, -dp[1], dp[0], 0, rhs[2]};
    }

    // Gaussian elimination with partial pivoting.
    for (size_t col = 0; col < 6; ++col) {
        size_t pivot = col;
        for (size_t row = col + 1; row < 6; ++row) {
            if (std::abs(matrix[row][col]) > std::abs(matrix[pivot][col])) { pivot = row; }
        }

        if (std::abs(matrix[pivot][col]) < 1e-12L) {
            throw std::runtime_error("Singular system; try different hailstones");
        }

        std::swap(matrix[col], matrix[pivot]);
        long double divisor = matrix[col][col];
        for (size_t j = col; j < 7; ++j) { matrix[col][j] /= divisor; }

        for (size_t row = 0; row < 6; ++row) {
            if (row == col) { continue; }
            long double factor = matrix[row][col];
            for (size_t j = col; j < 7; ++j) { matrix[row][j] -= factor * matrix[col][j]; }
        }
    }

    // The puzzle asks for the sum of the rock's initial coordinates.
    long long sum = 0;
    for (size_t j = 0; j < 3; ++j) {
        long double const coordinate  = positions[0][j] + matrix[j][6] * posScale;
        sum                    += std::llround(coordinate);
    }

    if (sum < 0) { throw std::runtime_error("Negative position sum"); }
    return static_cast<size_t>(sum);
}

} // namespace AOC2023