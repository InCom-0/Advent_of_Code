#include <iostream>
#include <ranges>


#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>

#include <incstd/core/hashing.hpp>
#include <incstd/core/solvers.hpp>

#include <incom_aoc_solver.h>
#include <incom_aoc_solver_2.h>
#include <incom_commons.h>


namespace AOC2025 {

size_t
day12_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;
    auto input    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    struct Shape {
        std::array<std::array<bool, 3>, 3> matrices;
    };
    struct Tree {
        int                 yDim;
        int                 xDim;
        std::vector<size_t> reqdShapes;
    };

    std::vector<Shape> shapes;
    std::vector<Tree>  trees;

    for (size_t lineID = 0; lineID < input.size(); ++lineID) {
        if (input.at(lineID).size() == 2) {
            shapes.emplace_back();
            for (size_t shape_line = 0; shape_line < 3; ++shape_line) {
                lineID++;
                for (int chrID = 0; auto oneChr : input.at(lineID)) {
                    shapes.back().matrices.at(shape_line).at(chrID++) = (oneChr == '#' ? true : false);
                }
            }
        }
        if (input.at(lineID).size() > 5) {
            auto prsRes = incom::aoc::parseInputUsingCTRE::processOneLine(input.at(lineID), d_ctre, d_ctre, d_ctre,
                                                                          d_ctre, d_ctre, d_ctre, d_ctre, d_ctre);
            trees.push_back(
                Tree{.yDim = std::stoi(prsRes.at(0)),
                     .xDim = std::stoi(prsRes.at(1)),
                     .reqdShapes{std::stoull(prsRes.at(2)), std::stoull(prsRes.at(3)), std::stoull(prsRes.at(4)),
                                 std::stoull(prsRes.at(5)), std::stoull(prsRes.at(6)), std::stoull(prsRes.at(7))}});
        }
    }
    namespace incsolvpack = incom::standard::solvers::packing;
    namespace locpack     = incom::aoc::packing;
    namespace locpack_2   = incom::aoc::packing_2;

    incsolvpack::BoxPacker_2D<5> solv_1(
        trees.front().yDim, trees.front().xDim,
        std::views::transform(shapes, [](auto const &oneShp) { return oneShp.matrices; }) |
            std::ranges::to<std::vector>(),
        trees.front().reqdShapes);

    size_t resAccu   = 0uz;
    size_t resUltima = 0uz;

    for (auto const &oneTree : std::views::drop(trees, 0)) {
        solv_1.reset_allButNotPastComputed(oneTree.yDim, oneTree.xDim, oneTree.reqdShapes);
        solv_1.prime_fprng();
        while (solv_1.solve_oneStep()) {
            // std::cout << solv_1.get_areaState() << '\n' << solv_1.get_useableShapeCountRemaining() << '\n';
            int a = 5;
            a++;
        }
        // solv_1.solve_XSteps();
        resAccu   += (solv_1.get_useableShapeCountRemaining() == 0uz);
        resUltima += solv_1.get_useableShapeCountRemaining();
        // std::cout << solv_1.get_areaState();
        // std::cout << '\n' << solv_1.get_useableShapeCountRemaining() << '\n' << solv_1.get_areaEmptyCount();
        // std::cout << "\n\n\n";
    }

    std::cout << resUltima << '\n';
    return resAccu;
}

size_t
day12_2(std::string dataFile) {
    return 0uz;
}

} // namespace AOC2025