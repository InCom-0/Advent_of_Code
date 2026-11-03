#include <algorithm>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <string>


#include <Mathematics/DistRayRay.h>
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

    gte::DCPQuery<double, gte::Ray<2, double>, gte::Ray<2, double>> RayDist{};

    for (auto const &inpLine : input) {
        rays.push_back(gte::Ray<2, double>{{std::stod(inpLine[0]), std::stod(inpLine[1])},
                                           {std::stod(inpLine[3]), std::stod(inpLine[4])}});
    }

    size_t intersections{};
    for (auto const &[l1, l2] : incstd::views::combinations_k<2uz>(rays)) {
        auto ld_res = RayDist(l1, l2);

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