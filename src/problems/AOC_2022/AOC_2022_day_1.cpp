#include <AOC_2022.h>
#include <algorithm>
#include <functional>
#include <incom_commons.hpp>

namespace AOC2022 {

auto
day1_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }

    auto res = std::vector(1, std::vector<size_t>{});

    std::string oneStr;
    size_t      oneInp;
    while (std::getline(iStream, oneStr)) {
        auto result = d_ctre(oneStr.begin(), oneStr.end());
        if (result) {
            std::from_chars(result.data(), result.data() + result.size(), oneInp);
            res.back().push_back(oneInp);
        }
        else { res.emplace_back(); }
    };

    return res;
}

std::size_t
day1_1(std::string dataFile) {

    auto const input = day1_0(dataFile);

    auto VecOfCalorieSums = std::views::transform(
        input, [](auto const &CalorieVec) { return std::ranges::fold_left(CalorieVec, 0uz, std::plus{}); });
    return *std::ranges::max_element(VecOfCalorieSums, std::less{});
}

std::size_t
day1_2(std::string dataFile) {
    auto const input = day1_0(dataFile);

    auto VecOfCalorieSums =
        std::views::transform(
            input, [](auto const &CalorieVec) { return std::ranges::fold_left(CalorieVec, 0uz, std::plus{}); }) |
        std::ranges::to<std::vector>();

    std::ranges::sort(VecOfCalorieSums, std::greater{});

    return std::ranges::fold_left(std::views::take(VecOfCalorieSums, 3), 0uz, std::plus{});
}

} // namespace AOC2022