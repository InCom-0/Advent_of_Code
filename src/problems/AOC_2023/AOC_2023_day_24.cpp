#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/views.hpp>
#include <limits>
#include <ranges>
#include <string>
#include <type_traits>

#include <Mathematics/DistAlignedBox3OrientedBox3.h>


namespace AOC2023 {
namespace {


template <typename T, size_t SZ>
using Line = std::array<std::array<T, SZ>, 2uz>;

auto
Dot(auto const &v0, auto const &v1) {
    if constexpr (! std::is_same_v<typename std::remove_cvref_t<decltype(v0)>::value_type,
                                   typename std::remove_cvref_t<decltype(v1)>::value_type>) {
        static_assert(false);
    }
    else {
        return std::ranges::fold_left(
            std::views::zip(v0, v1), typename std::remove_cvref_t<decltype(v0)>::value_type{},
            [](auto const init, auto const &pr) { return init + (std::get<0>(pr) * std::get<1>(pr)); });
    }
}

template <typename T, size_t N>
class LineDist {
public:
    struct Result {
        Result()
            : distance(static_cast<T>(0)),
              sqrDistance(static_cast<T>(0)),
              parameter{static_cast<T>(0), static_cast<T>(0)},
              closest{} {}

        T                distance, sqrDistance;
        std::array<T, 2> parameter;
        Line<T, N>       closest; // Not really a 'Line' ... just two closest points
    };

    Result
    operator()(Line<T, N> const &line0, Line<T, N> const &line1) {
        Result           result{};
        std::array<T, N> diff{};
        for (size_t i = 0uz; i < N; ++i) { diff[i] = line0[0][i] - line1[0][i]; }

        T const zero = static_cast<T>(0);
        T       a00  = Dot(line0[1], line0[1]);
        T       a01  = -Dot(line0[1], line1[1]);
        T       a11  = Dot(line1[1], line1[1]);
        T       b0   = Dot(line0[1], diff);
        T       det  = std::max(a00 * a11 - a01 * a01, zero);
        T       s0{}, s1{};

        if (det > zero) {
            // The lines are not parallel.
            T b1 = -Dot(line1[1], diff);
            s0   = (a01 * b1 - a11 * b0) / det;
            s1   = (a01 * b0 - a00 * b1) / det;
        }
        else {
            // The lines are parallel. Select any pair of closest points.
            s0 = -b0 / a00;
            s1 = zero;
        }

        result.parameter[0] = s0;
        result.parameter[1] = s1;
        for (size_t i = 0uz; i < N; ++i) {
            result.closest[0][i] = line0[0][i] + s0 * line0[1][i];
            result.closest[1][i] = line1[0][i] + s1 * line1[1][i];
        }
        for (size_t i = 0uz; i < N; ++i) { diff[i] = result.closest[0][i] - result.closest[1][i]; }
        result.sqrDistance = Dot(diff, diff);
        result.distance    = std::sqrt(result.sqrDistance);

        return result;
    }
};


template <typename T, size_t N>
class RayDist {
public:
    struct Result {
        Result()
            : distance(static_cast<T>(0)),
              sqrDistance(static_cast<T>(0)),
              parameter{static_cast<T>(0), static_cast<T>(0)},
              closest{} {}

        T                distance, sqrDistance;
        std::array<T, 2> parameter;
        Line<T, N>       closest; // Not really a 'Line' ... just two closest points
    };

    Result
    operator()(Line<T, N> const &ray0, Line<T, N> const &ray1) {
        Result           result{};
        std::array<T, N> diff{};
        for (size_t i = 0uz; i < N; ++i) { diff[i] = ray0[0][i] - ray1[0][i]; }

        T const zero = static_cast<T>(0);
        T       a00  = Dot(ray0[1], ray0[1]);
        T       a01  = -Dot(ray0[1], ray1[1]);
        T       a11  = Dot(ray1[1], ray1[1]);
        T       b0   = Dot(ray0[1], diff);
        T       det  = std::max(a00 * a11 - a01 * a01, zero);
        T       s0{}, s1{};

        if (det > zero) {
            // The rays are not parallel.
            T b1 = -Dot(ray1[1], diff);
            s0   = a01 * b1 - a11 * b0;
            s1   = a01 * b0 - a00 * b1;

            if (s0 >= zero) {
                if (s1 >= zero) // region 0 (interior)
                {
                    // The minimum occurs at two interior points of
                    // the rays.
                    s0 /= det;
                    s1 /= det;
                }
                else // region 3 (side)
                {
                    if (b0 >= zero) { s0 = zero; }
                    else { s0 = -b0 / a00; }
                    s1 = zero;
                }
            }
            else {
                if (s1 >= zero) // region 1 (side)
                {
                    s0 = zero;
                    if (b1 >= zero) { s1 = zero; }
                    else { s1 = -b1 / a11; }
                }
                else // region 2 (corner)
                {
                    if (b0 < zero) {
                        s0 = -b0 / a00;
                        s1 = zero;
                    }
                    else {
                        s0 = zero;
                        if (b1 >= zero) { s1 = zero; }
                        else { s1 = -b1 / a11; }
                    }
                }
            }
        }
        else {
            // The rays are parallel.
            if (a01 > zero) {
                // Opposite direction vectors.
                s1 = zero;
                if (b0 >= zero) { s0 = zero; }
                else { s0 = -b0 / a00; }
            }
            else {
                // Same direction vectors.
                if (b0 >= zero) {
                    T b1 = -Dot(ray1[1], diff);
                    s0   = zero;
                    s1   = -b1 / a11;
                }
                else {
                    s0 = -b0 / a00;
                    s1 = zero;
                }
            }
        }

        result.parameter[0] = s0;
        result.parameter[1] = s1;
        for (size_t i = 0uz; i < N; ++i) {
            result.closest[0][i] = ray0[0][i] + s0 * ray0[1][i];
            result.closest[1][i] = ray1[0][i] + s1 * ray1[1][i];
        }
        for (size_t i = 0uz; i < N; ++i) { diff[i] = result.closest[0][i] - result.closest[1][i]; }
        result.sqrDistance = Dot(diff, diff);
        result.distance    = std::sqrt(result.sqrDistance);

        return result;
    }
};

// Template aliases for convenience.
} // namespace


auto
day24_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"([\-]?\d+)">;

    auto input = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, any_ctre);

    Line<double, 2> l1{{{1, 5}, {-1, -1}}};
    Line<double, 2> l2{{{-1, 5}, {2, 1}}};

    RayDist<double, 2> RD{};

    auto r = RD(l1, l2);


    return input;
}

size_t
day24_1(std::string dataFile) {

    auto input = day24_0(dataFile);

    std::vector<Line<double, 2>> lines2;

    double minPos = 200000000000000;
    double maxPos = 400000000000000;

    RayDist<double, 2> RD{};

    for (auto const &inpLine : input) {
        Line<double, 2> one{
            {std::stod(inpLine[0]), std::stod(inpLine[1]), std::stod(inpLine[3]), std::stod(inpLine[4])}};
        lines2.push_back(std::move(one));
    }

    size_t intersections{};
    for (auto const &[l1, l2] : incstd::views::combinations_k<2uz>(lines2)) {
        auto ld_res = RD(l1, l2);

        if (ld_res.closest[0][0] >= minPos && ld_res.closest[0][0] <= maxPos && ld_res.closest[0][1] >= minPos &&
            ld_res.closest[0][1] <= maxPos) {
            if (ld_res.parameter[0] == 0.0 || ld_res.parameter[1] == 0.0) { continue; }
            intersections++;
        }
    }

    return intersections;
}

size_t
day24_2(std::string dataFile) {
    return 0uz;
}

} // namespace AOC2023