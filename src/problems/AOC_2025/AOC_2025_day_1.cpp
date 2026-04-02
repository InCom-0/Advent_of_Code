#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <string>


namespace AOC2025 {

long long
day1_1(std::string dataFile) {
    auto oneLet_ctre = ctre::search<R"(\D)">;
    auto d_ctre      = ctre::search<R"(\d+)">;
    auto input       = incom::commons::parseInputUsingCTRE::processFile(dataFile, oneLet_ctre, d_ctre);

    int       cur      = 50;
    size_t    resCount = 0;
    int       mult     = 1;

    for (auto const &[dir, val] : std::views::zip(input.at(0), input.at(1))) {
        mult = (dir.front() == 'R' ? 1 : -1);

        cur      += (mult * std::stoi(val));
        cur      %= 100;
        resCount += (cur == 0);
    }

    return resCount;
}
long long
day1_2(std::string dataFile) {
    auto oneLet_ctre = ctre::search<R"(\D)">;
    auto d_ctre      = ctre::search<R"(\d+)">;
    auto input       = incom::commons::parseInputUsingCTRE::processFile(dataFile, oneLet_ctre, d_ctre);

    int       cur      = 50;
    size_t    resCount = 0;
    int       mult     = 1;

    for (auto const &[dir, val] : std::views::zip(input.at(0), input.at(1))) {
        mult = (dir.front() == 'R' ? 1 : -1);

        resCount += (cur * (cur + (mult * std::stoi(val)))) < 0 ? 1 : 0;

        cur      += (mult * std::stoi(val));
        resCount += (cur == 0 ? 1 : std::abs(cur / 100));
        cur      %= 100;
    }

    return resCount;
}

} // namespace AOC2025