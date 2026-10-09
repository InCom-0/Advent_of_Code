#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <expected>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/views.hpp>
#include <ranges>
#include <string>
#include <type_traits>


namespace AOC2023 {

auto
day11_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto inp    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    std::vector<std::string> inp2;
    inp2.reserve(inp.size() * 2);

    for (auto const &inpLine : inp) {
        if (std::ranges::all_of(inpLine, [](char c) { return c == '.'; })) { inp2.push_back(inpLine); }
        inp2.push_back(inpLine);
    }

    inp = incstd::matrix::matrixRotateRight_copy(inp2).value();
    inp2.clear();

    for (auto const &inpLine : inp) {
        if (std::ranges::all_of(inpLine, [](char c) { return c == '.'; })) { inp2.push_back(inpLine); }
        inp2.push_back(inpLine);
    }

    inp = incstd::matrix::matrixRotateLeft_copy(inp2).value();
    return inp;
}

auto
day11_00(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto inp    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    std::vector<std::string> inp2;
    inp2.reserve(inp.size() * 2);

    std::vector<size_t> rowsEmpt{};
    std::vector<size_t> colsEmpt{};
    for (size_t i{}; auto const &inpLine : inp) {
        if (std::ranges::all_of(inpLine, [](char c) { return c == '.'; })) { rowsEmpt.push_back(i); }
        i++;
    }

    inp2 = incstd::matrix::matrixRotateRight_copy(inp).value();

    for (size_t i{}; auto const &inpLine : inp2) {
        if (std::ranges::all_of(inpLine, [](char c) { return c == '.'; })) { colsEmpt.push_back(i); }
        i++;
    }

    return std::make_tuple(inp, rowsEmpt, colsEmpt);
}

auto
day11_1(std::string dataFile) {
    auto input = day11_0(dataFile);

    std::vector<std::pair<size_t, size_t>> galaxiesPos;

    for (size_t y{}; auto const &inpLine : input) {
        for (size_t x{}; char const oneC : inpLine) {
            if (oneC == '#') { galaxiesPos.emplace_back(y, x); }
            x++;
        }
        y++;
    }

    size_t distRes{};
    for (auto const &[pr1, pr2] : incstd::views::combinations_k<2>(galaxiesPos)) {
        distRes += pr1.first > pr2.first ? pr1.first - pr2.first : pr2.first - pr1.first;
        distRes += pr1.second > pr2.second ? pr1.second - pr2.second : pr2.second - pr1.second;
    }

    return distRes;
}

auto
day11_2(std::string dataFile) {
    auto const [input, rowsEmpty, colsEmpty] = day11_00(dataFile);

    std::vector<std::pair<size_t, size_t>> galaxiesPos;

    for (size_t y{}; auto const &inpLine : input) {
        for (size_t x{}; char const oneC : inpLine) {
            if (oneC == '#') { galaxiesPos.emplace_back(y, x); }
            x++;
        }
        y++;
    }

    size_t const emptyRCAdd = 1'000'000uz - 1uz;

    size_t distRes{};
    for (auto const &[pr1r, pr2r] : incstd::views::combinations_k<2>(galaxiesPos)) {
        auto pr1 = pr1r;
        auto pr2 = pr2r;
        if (pr1.first < pr2.first) { std::swap(pr1.first, pr2.first); }
        if (pr1.second < pr2.second) { std::swap(pr1.second, pr2.second); }

        distRes += (pr1.first - pr2.first) + (pr1.second - pr2.second);
        for (size_t const row : rowsEmpty) {
            if ((row > pr2.first) && (row < pr1.first)) { distRes += emptyRCAdd; }
        }
        for (size_t const col : colsEmpty) {
            if ((col > pr2.second) && (col < pr1.second)) { distRes += emptyRCAdd; }
        }
    }

    return distRes;
}
} // namespace AOC2023