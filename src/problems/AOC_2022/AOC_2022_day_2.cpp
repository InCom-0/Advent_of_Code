#include <AOC_2022.h>
#include <algorithm>
#include <incom_commons.hpp>
#include <utility>

namespace AOC2022 {
auto
day2_0(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\w+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    return input;
}
std::size_t
day2_1(std::string dataFile) {
    auto const input = day2_0(dataFile);

    enum class RPS : int {
        rock     = 1,
        paper    = 2,
        scissors = 3,
    };

    auto tr = std::views::transform(input, [](auto const &line) {
        std::array<RPS, 2uz> res{};
        switch (line.front().front()) {
            case 'A': res.front() = RPS::rock; break;
            case 'B': res.front() = RPS::paper; break;
            case 'C': res.front() = RPS::scissors; break;
        }
        switch (line.back().front()) {
            case 'X': res.back() = RPS::rock; break;
            case 'Y': res.back() = RPS::paper; break;
            case 'Z': res.back() = RPS::scissors; break;
        }
        return res;
    });


    // loss 0, draw 3, win 6.
    // plus std::to_underlying value
    auto Score = [](RPS const &other, RPS const &mine) {
        size_t    res{static_cast<size_t>(std::to_underlying(mine))};
        int const dif = (std::to_underlying(mine) - std::to_underlying(other));
        if (dif == 1 || dif == -2) { res += 6; }
        else if (other == mine) { res += 3; }

        return res;
    };

    return std::ranges::fold_left(
        tr | std::views::transform([&](auto const &itemPR) { return Score(itemPR.front(), itemPR.back()); }), 0uz,
        std::plus{});
}
std::size_t
day2_2(std::string dataFile) {
    auto const input = day2_0(dataFile);

    enum class RPS : int {
        rock     = 1,
        paper    = 2,
        scissors = 3,
    };

    auto tr = std::views::transform(input, [](auto const &line) {
        std::array<RPS, 2uz> res{};
        switch (line.front().front()) {
            case 'A': res.front() = RPS::rock; break;
            case 'B': res.front() = RPS::paper; break;
            case 'C': res.front() = RPS::scissors; break;
        }
        switch (line.back().front()) {
            case 'X': res.back() = RPS::rock; break;
            case 'Y': res.back() = RPS::paper; break;
            case 'Z': res.back() = RPS::scissors; break;
        }
        return res;
    });


    auto Convert = [](RPS const &other, RPS const &wdl) -> RPS {
        RPS mine;
        switch (wdl) {
            case RPS::rock:
                mine = RPS{(std::to_underlying(other)) - 1 + (((std::to_underlying(other)) - 1) == 0 ? 3 : 0)};
                break;
            case RPS::paper:    mine = other; break;
            case RPS::scissors: mine = RPS{((std::to_underlying(other)) % 3) + 1}; break;
        }

        return mine;
    };

    // loss 0, draw 3, win 6.
    // plus std::to_underlying value
    auto Score = [&](RPS const &other, RPS const &wdl) {
        auto      mine = Convert(other, wdl);
        size_t    res{static_cast<size_t>(std::to_underlying(mine))};
        int const dif = (std::to_underlying(mine) - std::to_underlying(other));
        if (dif == 1 || dif == -2) { res += 6; }
        else if (other == mine) { res += 3; }

        return res;
    };

    return std::ranges::fold_left(
        tr | std::views::transform([&](auto const &itemPR) { return Score(itemPR.front(), itemPR.back()); }), 0uz,
        std::plus{});
    return 0uz;
}
} // namespace AOC2022