#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/core.h>
#include <incom_commons.h>
#include <ranges>
#include <string_view>


namespace AOC2024 {
namespace incc = incom::commons;

long long day4_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
    auto inpVert  = input;

    incc::matrixRotateRight(inpVert);

    auto createDiagonalRngs = [](std::vector<std::string> const &inp) {
        std::vector<std::string> res;

        size_t curX = 0;
        size_t curY = 0;

        for (; curX < inp.size(); curX++) {
            curY            = 0;
            size_t curX_loc = curX;
            res.push_back(std::string());
            while (curX_loc < inp.size() && curY < inp.front().size()) {
                res.back().push_back(inp[curX_loc++][curY++]);
            }
        }
        curY = 1;
        for (; curY < inp.front().size(); curY++) {
            curX            = 0;
            size_t curY_loc = curY;
            res.push_back(std::string());
            while (curX < inp.size() && curY_loc < inp.front().size()) {
                res.back().push_back(inp[curX++][curY_loc++]);
            }
        }
        return res;
    };

    auto TLDRdiag = createDiagonalRngs(input);
    auto TRDLdiag = createDiagonalRngs(inpVert);

    auto testView = std::views::slide(input.front(), 4);

    auto tvItem = testView.front();

    long long res = 0;
    using namespace std::literals;

    auto computeXMAScnt = [](auto const &inp, std::string_view const &&svToCompare) -> long long {
        long long ret = 0;
        for (auto const &line : inp) {
            ret += std::ranges::count_if(std::views::slide(line, 4), [&](auto const &fourLetters) {
                std::string_view strView(fourLetters.begin(), fourLetters.end());
                return (strView == svToCompare);
            });
        }
        return ret;
    };

    res += computeXMAScnt(input, "XMAS"sv);
    res += computeXMAScnt(inpVert, "XMAS"sv);
    res += computeXMAScnt(TLDRdiag, "XMAS"sv);
    res += computeXMAScnt(TRDLdiag, "XMAS"sv);

    res += computeXMAScnt(input, "SAMX"sv);
    res += computeXMAScnt(inpVert, "SAMX"sv);
    res += computeXMAScnt(TLDRdiag, "SAMX"sv);
    res += computeXMAScnt(TRDLdiag, "SAMX"sv);

    return res;
}

long long day4_2(std::string dataFile) {

    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    for (auto &line : input) {
        line.push_back('_');
        line.insert(line.begin(), '_');
    }
    input.insert(input.begin(), std::string(input.front().size(), '_'));
    input.push_back(std::string(input.front().size(), '_'));

    constexpr std::array<std::array<int, 2>, 4> const dirs{-1, -1, -1, 1, 1, 1, 1, -1};

    long long res = 0;
    for (int row = 1; row < input.size() - 1; ++row) {
        for (int col = 1; col < input.front().size() - 1; ++col) {
            if (input[row][col] == 'A') {
                int mCount = 0;
                int sCount = 0;
                for (auto const &dr : dirs) {
                    if (input[row + dr.front()][col + dr.back()] == 'M') { mCount++; }
                    if (input[row + dr.front()][col + dr.back()] == 'S') { sCount++; }
                }
                if (sCount == 2 && mCount == 2) {
                    if (input[row + dirs[0].front()][col + dirs[0].back()] !=
                        input[row + dirs[2].front()][col + dirs[2].back()]) {
                        res++;
                    }
                }
            }
        }
    }
    return res;
}
long long day4_3(std::string dataFile) {

    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    auto sld = std::views::adjacent_transform<3>(input, [](auto const &a, auto const &b, auto const &c) {
        return std::views::zip(std::views::slide(a, 3), std::views::slide(b, 3), std::views::slide(c, 3));
    });

    long long res = 0;
    for (auto const &slidItem : sld) {
        for (auto const &zipItem : slidItem) {
            std::string_view sv1(std::get<0>(zipItem));
            std::string_view sv2(std::get<1>(zipItem));
            std::string_view sv3(std::get<2>(zipItem));
            if (sv2[1] != 'A') { continue; }

            int mCount = ((sv1[0] == 'M') + (sv1[2] == 'M') + (sv3[0] == 'M') + (sv3[2] == 'M'));
            int sCount = ((sv1[0] == 'S') + (sv1[2] == 'S') + (sv3[0] == 'S') + (sv3[2] == 'S'));
            if (mCount == 2 && sCount == 2 && (sv1[0] != sv3[2])) { res++; }
        }
    }
    return res;
}
} // namespace AOC2024