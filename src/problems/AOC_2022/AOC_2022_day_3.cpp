#include <AOC_2022.h>
#include <algorithm>
#include <incom_commons.hpp>
#include <ranges>

namespace AOC2022 {
auto
day3_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\w+)">;
    return incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();
}

std::size_t
day3_1(std::string dataFile) {

    auto const input = day3_0(dataFile);

    auto scoreChar = [](char c) {
        int res{};

        if (c >= 'a') { res = static_cast<int>(c) - 96; }
        else { res = static_cast<int>(c) - 38; }

        return res;
    };


    auto theView = std::views::transform(input, [](auto const &line) {
        return std::views::chunk(line, (line.size() / 2) + line.size() % 2) | std::views::pairwise;
    });


    return std::ranges::fold_left(theView, 0uz, [&](auto init, auto const &compPair) {
        for (auto const &[leftC, rightC] : compPair) {
            std::vector<unsigned char> memo(256, 0);
            for (auto const &c : leftC) { memo[c]++; }

            for (auto const &c : rightC) {
                if (memo[c]) { return init + scoreChar(c); }
            }
        }
        assert(false);
        return init;
    });
}

std::size_t
day3_2(std::string dataFile) {

    auto const input = day3_0(dataFile);

    auto scoreChar = [](char c) {
        int res{};

        if (c >= 'a') { res = static_cast<int>(c) - 96; }
        else { res = static_cast<int>(c) - 38; }

        return res;
    };

    auto theView = std::views::chunk(input, 3) |
                   std::views::transform([](auto const &chnk) { return std::tie(chnk[0], chnk[1], chnk[2]); });

    auto res = std::ranges::fold_left(theView, 0uz, [&](auto init, auto const &tpl) {
        auto const &[first, second, third] = tpl;
        std::vector<unsigned char> memo(256, 0);

        for (auto const &c : first) { memo[c] = 1; }
        for (auto const &c : second) {
            if (memo[c] == 1) { memo[c] = 2; }
        }
        for (auto const &c : third) {
            if (memo[c] == 2) { return init + scoreChar(c); }
        }
        assert(false);
        return init;
    });


    return res;
}
} // namespace AOC2022