#include <algorithm>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <limits>
#include <string>


#include <ankerl/unordered_dense.h>
#include <type_traits>


namespace AOC2023 {

namespace {
using namespace std;

// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm#Example_code
template <typename T>
requires std::is_arithmetic_v<T>
auto
globalMinCut(std::vector<std::vector<T>> mat) {

    std::pair<size_t, std::vector<size_t>> res = {std::numeric_limits<size_t>::max(),
                                                  {}}; // {number of cuts, IDs of verts in one partition}
    size_t const                           n   = mat.size();
    std::vector<std::vector<size_t>>       co(n);

    for (size_t i = 0; i < n; i++) { co[i] = {i}; }

    for (int ph = 1; ph < n; ph++) {
        std::vector<T> w = mat[0];
        size_t         s = 0, t = 0;
        for (int it = 0; it < n - ph; it++) { // O(V^2) -> O(E log V) with prio. queue
            w[t] = std::numeric_limits<T>::min();
            s = t, t = max_element(w.begin(), w.end()) - w.begin();
            for (size_t i = 0; i < n; i++) { w[i] += mat[t][i]; }
        }
        res = min(res, {w[t] - mat[t][t], co[t]});
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (size_t i = 0; i < n; i++) { mat[s][i] += mat[t][i]; }
        for (size_t i = 0; i < n; i++) { mat[i][s] = mat[s][i]; }
        mat[0][t] = std::numeric_limits<T>::min();
    }

    return res;
}
} // namespace

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
    auto res = globalMinCut(input);
    return res.second.size() * (input.size() - res.second.size());
}

} // namespace AOC2023