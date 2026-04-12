#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>

#include <incstd/core/hashing.hpp>
#include <ranges>
#include <string>

#include <AOC_2025_solvers.h>
#include <incom_commons.h>


namespace AOC2025 {
size_t
day12_1_trial(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;
    auto input    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    struct Shape {
        std::array<std::array<bool, 3>, 3> matrices;
    };
    struct Tree {
        int yDim;
        int xDim;

        std::vector<int> reqdShapes;
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
                     .reqdShapes{std::stoi(prsRes.at(2)), std::stoi(prsRes.at(3)), std::stoi(prsRes.at(4)),
                                 std::stoi(prsRes.at(5)), std::stoi(prsRes.at(6)), std::stoi(prsRes.at(7))}});
        }
    }
    namespace inctetrix = incom::aoc::solvers::tetris;

    inctetrix::Solver<5> solv_1(trees.front().yDim, trees.front().xDim,
                                std::views::transform(shapes, [](auto const &oneShp) { return oneShp.matrices; }) |
                                    std::ranges::to<std::vector>());
    for (auto useableCount : trees.front().reqdShapes) { solv_1.m_useableCount_perShape.push_back(useableCount); }

    auto rrr  = solv_1.hash_stateOfSelf();
    auto rrr2 = solv_1.hash_stateOfSelf();
    solv_1.prime_fprng();

    auto rest = solv_1.m_useableCount_perShape;
    while (auto oneStepres = solv_1.solve_oneStep()) {
        solv_1.print_areaState();
        std::cout << "\n\n";
    }


    return 0uz;
}

size_t
day12_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;
    auto input    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    struct Shape {
        std::vector<std::array<std::array<bool, 3>, 3>> matrices;
    };

    struct Tree {
        int yDim;
        int xDim;

        std::vector<std::vector<char>> spots;
        std::vector<int>               reqdShapes;
    };

    std::vector<Shape> shapes;
    std::vector<Tree>  trees;


    for (size_t lineID = 0; lineID < input.size(); ++lineID) {
        if (input.at(lineID).size() == 2) {
            shapes.emplace_back();
            shapes.back().matrices.emplace_back();
            for (size_t shape_line = 0; shape_line < 3; ++shape_line) {
                lineID++;
                for (int chrID = 0; auto oneChr : input.at(lineID)) {
                    shapes.back().matrices.front().at(shape_line).at(chrID++) = (oneChr == '#' ? true : false);
                }
            }
        }
        if (input.at(lineID).size() > 5) {
            auto prsRes = incom::aoc::parseInputUsingCTRE::processOneLine(input.at(lineID), d_ctre, d_ctre, d_ctre,
                                                                          d_ctre, d_ctre, d_ctre, d_ctre, d_ctre);
            trees.push_back(
                Tree{.yDim  = std::stoi(prsRes.at(0)),
                     .xDim  = std::stoi(prsRes.at(1)),
                     .spots = std::vector(std::stoi(prsRes.at(0)), std::vector<char>(std::stoi(prsRes.at(1)), 0)),
                     .reqdShapes{std::stoi(prsRes.at(2)), std::stoi(prsRes.at(3)), std::stoi(prsRes.at(4)),
                                 std::stoi(prsRes.at(5)), std::stoi(prsRes.at(6)), std::stoi(prsRes.at(7))}});
        }
    }
    return 0uz;
}

size_t
day12_2(std::string dataFile) {
    return 0uz;
}

} // namespace AOC2025