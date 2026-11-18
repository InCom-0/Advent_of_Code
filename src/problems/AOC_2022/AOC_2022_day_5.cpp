#include <AOC_2022.h>
#include <algorithm>
#include <incom_commons.hpp>
#include <ranges>
#include <string>
#include <vector>

namespace AOC2022 {
auto
day5_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;

    std::pair<std::vector<std::vector<char>>, std::vector<std::array<size_t, 3uz>>> res{
        std::vector<std::vector<char>>(9), {}};
    std::vector<std::vector<char>> revertedStcks(9);

    auto &stcks  = res.first;
    auto &instrs = res.second;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }

    std::string oneStr;
    while (std::getline(iStream, oneStr)) {
        auto result = any_ctre(oneStr.begin(), oneStr.end());
        if (oneStr.size() < 32uz) { break; }

        std::string const str    = result.to_string();
        size_t            cursor = 1uz;
        size_t            counter{};

        while (cursor < str.size()) {
            if (str[cursor] >= 'A' && str[cursor] <= 'Z') { stcks.at(counter).push_back(str[cursor]); }
            else if (str[cursor] >= '0' && str[cursor] <= '9') { break; }
            else if (str[cursor] == ' ') {}
            else { assert(false); }

            counter++;
            cursor += 4;
        }
    }

    for (auto &oneStck : stcks) { std::ranges::reverse(oneStck); }

    while (std::getline(iStream, oneStr)) {
        auto result = incom::aoc::parseInputUsingCTRE::processOneLineRPToneVect(oneStr, d_ctre);
        if (result.empty()) { break; }
        if (result.size() != 3) { assert(false); }

        instrs.push_back({std::stoull(result.at(0)), std::stoull(result.at(1)), std::stoull(result.at(2))});
    }

    return res;
}

std::string
day5_1(std::string dataFile) {

    auto [stacks, instrs] = day5_0(dataFile);

    for (auto const &inst : instrs) {
        for (size_t i{}; i < inst.front(); ++i) {
            stacks.at(inst[2] - 1).push_back(stacks.at(inst[1] - 1).back());
            stacks.at(inst[1] - 1).pop_back();
        }
    }
    std::string res;
    for (auto const &stck : stacks) { res.push_back(stck.back()); }

    return res;
}
std::string
day5_2(std::string dataFile) {

    auto [stacks, instrs] = day5_0(dataFile);

    for (auto const &inst : instrs) {
        stacks.at(inst[2] - 1)
            .append_range(std::views::drop(stacks.at(inst[1] - 1), stacks.at(inst[1] - 1).size() - inst.front()));
        stacks.at(inst[1] - 1).resize(stacks.at(inst[1] - 1).size() - inst.front());
        // for (size_t i{}; i < inst.front(); ++i) { stacks.at(inst[1] - 1).pop_back(); }
    }
    std::string res;
    for (auto const &stck : stacks) { res.push_back(stck.back()); }

    return res;
}
} // namespace AOC2022