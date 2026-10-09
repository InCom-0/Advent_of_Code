#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/matrix.hpp>
#include <ranges>
#include <span>
#include <string>
#include <string_view>


namespace AOC2023 {

auto
findMirroPos(std::string_view sv) {
    std::vector<size_t> res;
    for (size_t middle = 1uz, beg{}; middle < sv.size(); ++middle) {
        if (middle > sv.size() / 2) { beg = 2 * middle - sv.size(); }

        auto zip =
            std::views::zip(std::span(sv.begin() + beg, sv.begin() + middle),
                            std::views::reverse(sv) | std::views::drop(beg > 0 ? 0 : sv.size() - 2 * middle + beg) |
                                std::views::take(middle - beg));
        if (not zip.empty() &&
            std::ranges::all_of(zip, [](auto const &tpl) { return std::get<0>(tpl) == std::get<1>(tpl); })) {
            res.push_back(middle);
        }
    }
    return res;
};

auto
calcMirroForMatrix(std::vector<std::string> const &inpMatrix) {
    std::vector<size_t> countTracker(inpMatrix.front().size(), 0uz);
    for (auto const &line : inpMatrix) {
        auto const mirPosss = findMirroPos(line);
        for (auto const &mirPos : findMirroPos(line)) { countTracker[mirPos]++; }
    }

    std::vector<size_t> res;
    for (size_t i{}; i < countTracker.size(); ++i) {
        if (countTracker[i] == inpMatrix.size()) { res.push_back(i); }
    }
    return res;
};

auto
day13_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }


    std::vector<std::vector<std::string>> res;
    res.emplace_back();

    std::string oneStr;
    while (std::getline(iStream, oneStr)) {
        if (oneStr == "") {
            res.emplace_back();
            continue;
        }
        res.back().push_back(any_ctre(oneStr.begin(), oneStr.end()).to_string());
    };
    return res;
}

auto
day13_1(std::string dataFile) {
    auto input = day13_0(dataFile);

    size_t globalRes{};
    for (auto const &inpMatrix : input) {

        size_t localRes    = std::ranges::fold_left(calcMirroForMatrix(inpMatrix), 0uz, std::plus{});
        auto   rotMatLeft  = incstd::matrix::matrixRotateLeft_copy(inpMatrix).value();
        localRes          += (100 * std::ranges::fold_left(calcMirroForMatrix(rotMatLeft), 0uz, std::plus{}));
        size_t t{};
        globalRes += localRes;
    }


    return globalRes;
}

auto
day13_2(std::string dataFile) {
    auto input = day13_0(dataFile);

    auto calcMirCount = [&](std::vector<std::string> const &inpMatrix) {
        std::vector<size_t> countTracker(inpMatrix.front().size(), 0uz);
        for (auto const &line : inpMatrix) {
            auto const mirPosss = findMirroPos(line);
            for (auto const &mirPos : findMirroPos(line)) { countTracker[mirPos]++; }
        }

        return countTracker;
    };

    size_t globalRes{};
    for (auto const &inpMatrix : input) {

        auto mirroredLinesCountAts = calcMirCount(inpMatrix);

        if (auto it = std::ranges::find(mirroredLinesCountAts, inpMatrix.size() - 1);
            it != mirroredLinesCountAts.end()) {}
        else {
            auto rotMatLeft                = incstd::matrix::matrixRotateLeft_copy(inpMatrix).value();
            auto mirroredLinesCountAts_rot = calcMirCount(rotMatLeft);
            
            if (auto it = std::ranges::find(mirroredLinesCountAts_rot, rotMatLeft.size() - 1);
                it != mirroredLinesCountAts_rot.end()) {}

            else { assert(false); }
        }
    }


    return globalRes;
}

} // namespace AOC2023