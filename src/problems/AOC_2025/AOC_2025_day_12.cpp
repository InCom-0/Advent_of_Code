#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>

#include <ranges>
#include <string>

#include <incom_commons.h>
#include <AOC_2025_solvers.h>

namespace AOC2025 {
size_t
day12_1_trial(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;
    auto input    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    struct Shape {
        std::vector<std::array<std::array<bool, 5>, 5>> matrices;

        std::pair<Shape, size_t>
        overlayWith(Shape const &other) {


            return {};
        }
    };

    struct Tree {
        int yDim;
        int xDim;

        std::vector<std::vector<char>> spots;


        std::vector<Shape>  shapesToUse;
        std::vector<size_t> reqdShapes;

        Tree(size_t ySize, size_t xSize, std::vector<Shape> shapesToUse, std::vector<size_t> perShapeCountReq,
             size_t windowsSz_y, size_t windowsSz_x)
            : yDim(std::move(ySize)), xDim(std::move(xSize)),
              spots(std::vector(yDim + 2, std::vector<char>(xDim + 2, 0))), shapesToUse(std::move(shapesToUse)) {
            // 'Fake' border of occupied spots at construction to make later computations easier
            spots.front() = std::vector<char>(spots.front().size(), 1);
            for (auto &spotLine : std::views::drop(std::views::take(spots, spots.size() - 1), 1)) {
                spotLine.front() = 1;
                spotLine.back()  = 1;
            }
            spots.back() = std::vector<char>(spots.front().size(), 1);
        }
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
            trees.push_back(Tree(std::stoi(prsRes.at(0)), std::stoi(prsRes.at(1)), shapes,
                                 {std::stoull(prsRes.at(2)), std::stoull(prsRes.at(3)), std::stoull(prsRes.at(4)),
                                  std::stoull(prsRes.at(5)), std::stoull(prsRes.at(6)), std::stoull(prsRes.at(7))},
                                 5, 5));
        }
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