#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.hpp>
#include <ranges>
#include <string>


namespace AOC2023 {


auto
day12_0(std::string dataFile) {
    auto nws_ctre = ctre::search<R"([^ ]+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }

    struct InpItem {
        std::string         springs;
        std::vector<size_t> dmgContSizes{};
    };

    std::vector<InpItem> res;

    std::string oneStr;
    while (std::getline(iStream, oneStr)) {
        auto                  result = nws_ctre(oneStr.begin(), oneStr.end());
        std::string::iterator bg     = oneStr.begin();
        std::string::iterator end    = oneStr.end();

        InpItem oneInp{.springs = result.to_string()};

        while (result = d_ctre(bg, end), result) {
            bg = result.get_end_position();
            oneInp.dmgContSizes.emplace_back();
            std::from_chars(result.data(), result.data() + result.size(), oneInp.dmgContSizes.back());
        }
        res.push_back(oneInp);
    };


    return res;
}

auto
day12_1(std::string dataFile) {

    auto   input = day12_0(dataFile);
    size_t arrangements{};

    for (auto const &inpItem : input) {
        size_t              curSeqID{};
        std::vector<size_t> nextFreeStack{0uz};

        while (curSeqID < inpItem.dmgContSizes.size() && not nextFreeStack.empty() && true) {
            size_t const nfs   = nextFreeStack.back();
            size_t const cs    = inpItem.dmgContSizes.at(curSeqID);
            bool         found = false;
            for (size_t i = nfs; auto const &sld :
                                 std::ranges::subrange(std::next(inpItem.springs.begin(), nfs), inpItem.springs.end()) |
                                     std::views::slide(cs)) {

                if ((i > 0 && inpItem.springs.at(i - 1) == '#') || (i + cs > inpItem.springs.size())) { break; }
                else if (std::ranges::all_of(sld, [](char const &c) { return c == '#' || c == '?'; })) {
                    nextFreeStack.back() = i + 1;
                    found                = true;
                    if ((curSeqID + 1) == inpItem.dmgContSizes.size()) {
                        if (std::ranges::none_of(
                                std::ranges::subrange(std::next(inpItem.springs.begin(), nextFreeStack.back() - 1 + cs),
                                                      inpItem.springs.end()),
                                [](char const &c) { return c == '#'; })) {
                            ++arrangements;
                        }
                    }
                    else {
                        nextFreeStack.push_back(nextFreeStack.back() + cs);
                        ++curSeqID;
                    }
                    break;
                }
                ++i;
            }
            if (not found) {
                --curSeqID;
                nextFreeStack.pop_back();
            }
        }
    }

    return arrangements;
}

auto
day12_2(std::string dataFile) {
    auto input = day12_0(dataFile);
    for (auto &inpLine : input) {

        auto const inpLineCPY = inpLine;
        for (size_t i{}; i < 4uz; ++i) {
            inpLine.springs.push_back('?');
            inpLine.springs.append(inpLineCPY.springs);
            inpLine.dmgContSizes.append_range(inpLineCPY.dmgContSizes);
        }
    }


    size_t globalRes{};
    for (auto const &inpLine : input) {
        size_t const n = inpLine.springs.size();
        size_t const m = inpLine.dmgContSizes.size();
        std::vector  memo(n + 1, std::vector(m + 1, std::vector<long long>(n + 1, -1)));

        auto recu = [&](this auto const &self, int const i, int const g, int const run) -> size_t {
            long long &ans = memo[i][g][run];
            if (ans != -1) { return ans; }
            ans = 0;

            if (i == n) {
                if (run == 0 && g == m) { return ans = 1; }
                if (g == m - 1 && run == inpLine.dmgContSizes[g]) { return ans = 1; }
                return ans = 0;
            }

            char const &c = inpLine.springs[i];

            // Try '.'
            if (c == '.' || c == '?') {
                if (run == 0) { ans += self(i + 1, g, 0); }
                else if (g < m && run == inpLine.dmgContSizes[g]) { ans += self(i + 1, g + 1, 0); }
            }

            // Try '#'
            if (c == '#' || c == '?') {
                if (g < m && run < inpLine.dmgContSizes[g]) { ans += self(i + 1, g, run + 1); }
            }

            return ans;
        };

        globalRes += recu(0, 0, 0);
    }

    return globalRes;
}
} // namespace AOC2023