#include <algorithm>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/solvers/graph.hpp>
#include <limits>
#include <string>


#include <ankerl/unordered_dense.h>
#include <type_traits>


namespace AOC2023 {
    
auto
day25_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(\w+)">;
    auto input    = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, any_ctre);

    ankerl::unordered_dense::set<std::string> strSet;

    for (auto const &inpLine : input) {
        for (auto const &lineItem : inpLine) { strSet.insert(lineItem); }
    }

    auto res = std::vector(strSet.size(), std::vector(strSet.size(), 0));

    for (auto const &inpLine : input) {
        auto const lineFromID = strSet.find(inpLine.front()) - strSet.begin();
        for (auto const &lineItem : std::views::drop(inpLine, 1)) {
            auto const lineToID       = strSet.find(lineItem) - strSet.begin();
            res[lineFromID][lineToID] = 1;
            res[lineToID][lineFromID] = 1;
        }
    }

    int a;
    return res;
}

size_t
day25_1(std::string dataFile) {
    auto input = day25_0(dataFile);
    auto res = incstd::solvers::graph::compute_globalMinCut(input);
    return res.second.size() * (input.size() - res.second.size());
}

} // namespace AOC2023