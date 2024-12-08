#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <string_view>


namespace AOC2024 {
namespace incc = incom::commons;

long long day3_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre);

    std::string data;
    for (auto const &line : input.front()) { data += line; }

    auto mul_ctre = ctre::search<R"(mul\(\d{1,3},\d{1,3}\))">;
    auto parsed   = incc::parseInputUsingCTRE::processOneLineRPToneVect(data, mul_ctre);

    auto d_ctre = ctre::search<R"(\d+)">;

    long long res = 0;
    for (auto &parsedLine : parsed) {
        auto twoNumbers  = incc::parseInputUsingCTRE::processOneLineRPToneVect(parsedLine, d_ctre);
        res             += (std::stoll(twoNumbers.front()) * std::stoll(twoNumbers.back()));
    }

    return res;
}

long long day3_2(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre);

    std::string data;
    for (auto const &line : input.front()) { data += line; }

    auto      mul_ctre = ctre::search<R"(mul\(\d{1,3},\d{1,3}\))">;
    auto      d_ctre   = ctre::search<R"(\d+)">;
    auto splitByDo = std::views::split(data, "do()"sv);

    long long res      = 0;
    for (auto const &splt : splitByDo) {
        auto doInThisStr = std::views::split(splt, "don't()"sv).front() | std::ranges::to<std::string>();
        auto parsed      = incc::parseInputUsingCTRE::processOneLineRPToneVect(doInThisStr, mul_ctre);

        for (auto &parsedLine : parsed) {
            auto twoNumbers  = incc::parseInputUsingCTRE::processOneLineRPToneVect(parsedLine, d_ctre);
            res             += (std::stoll(twoNumbers.front()) * std::stoll(twoNumbers.back()));
        }
    }

    return res;
}
} // namespace AOC2024