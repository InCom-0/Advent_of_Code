#include <ranges>

#include <ankerl/unordered_dense.h>
#include <ctre.hpp>

#include <incstd/core/hashing.hpp>
#include <incstd/core/solvers.hpp>

#include <incom_aoc_solver.h>
#include <incom_commons.h>


namespace AOC2025 {

size_t
day12_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;
    auto input    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    struct Shape_LOC {
        std::array<std::array<bool, 3>, 3> matrices;
    };
    struct Tree {
        int                 yDim;
        int                 xDim;
        std::vector<size_t> reqdShapes;
    };

    std::vector<Shape_LOC> shapes;
    std::vector<Tree>      trees;

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
    namespace incpack = incom::standard::solvers::packing;

    incpack::BoxPacker_2D solv_1(trees.front().yDim, trees.front().xDim,
                                 std::views::transform(shapes, [](auto const &oneShp) { return oneShp.matrices; }) |
                                     std::ranges::to<std::vector>(),
                                 trees.front().reqdShapes, 0);


    size_t resAccu   = 0uz;
    size_t resUltima = 0uz;
    size_t empty     = 0uz;
    size_t filled    = 0uz;

    for (auto const &oneTree : std::views::drop(trees, 0)) {
        solv_1.reset_allButNotPastComputed(oneTree.yDim, oneTree.xDim, oneTree.reqdShapes);
        solv_1.add_toFrontier_allCorners();

        size_t iter = 0;
        while (solv_1.solve_oneStep()) {
            // std::cout << solv_1.get_areaState() << solv_1.get_useableShapeCountRemaining() << '\n' << '\n';
            // iter++;
        }

        resUltima += solv_1.get_useableShapeCountRemaining();
        if (solv_1.get_useableShapeCountRemaining() == 0uz) { resAccu++; }
        else {
            auto [emp, fil]  = solv_1.get_emptyFilled();
            empty           += emp;
            filled          += fil;
        }

        // std::cout << solv_1.get_areaState() << solv_1.get_useableShapeCountRemaining();
        // std::cout << '\n' << solv_1.get_useableShapeCountRemaining() << '\n' << solv_1.get_areaEmptyCount();
        // std::cout << "\n\n\n";
    }

    // std::cout << resUltima << '\n';
    // std::cout << empty << '\n';
    // std::cout << filled << '\n';
    return resAccu;
}

} // namespace AOC2025