#include <AOC_2022.h>
#include <incom_commons.hpp>
#include <ranges>
#include <string>

namespace AOC2022 {
auto
day4_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto inpRaw = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    return std::views::transform(inpRaw,
                                 [](auto const &item) {
                                     return std::views::transform(item,
                                                                  [](auto const &str) { return std::stoull(str); }) |
                                            std::ranges::to<std::vector>();
                                 }) |
           std::ranges::to<std::vector>();
}
std::size_t
day4_1(std::string dataFile) {
    auto const input = day4_0(dataFile);
    size_t     accu{};

    for (auto const &oneQuad : input) {
        if ((oneQuad[0] <= oneQuad[2]) && (oneQuad[1] >= oneQuad[3])) { accu++; }
        else if ((oneQuad[2] <= oneQuad[0]) && (oneQuad[3] >= oneQuad[1])) { accu++; }
    }

    return accu;
}
std::size_t
day4_2(std::string dataFile) {
    auto const input = day4_0(dataFile);
    size_t     accu{};

    for (auto const &oneQuad : input) {
        if ((oneQuad[1] < oneQuad[2]) || (oneQuad[0] > oneQuad[3])) {}
        else { accu++; }
    }

    return accu;
}
} // namespace AOC2022