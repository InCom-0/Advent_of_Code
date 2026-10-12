#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/buffers.hpp>
#include <string>
#include <string_view>


namespace AOC2023 {

auto
day15_0(std::string dataFile) {
    auto nws_ctre = ctre::search<R"([^,]+)">;
    return incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, nws_ctre).front();
}

void
processOneChar(char const &c, size_t &curVal) {
    curVal += static_cast<size_t>(c);
    curVal *= 17;
    curVal %= 256;
}

size_t
createHashFrom(std::string_view sv) {
    size_t curVal{};
    for (auto const &c : sv) { processOneChar(c, curVal); }
    return curVal;
}


size_t
day15_1(std::string dataFile) {
    auto             input = day15_0(dataFile);
    std::vector<int> out;

    size_t accu{};
    for (auto const &inpItem : input) { accu += createHashFrom(inpItem); }

    return accu;
}

size_t
day15_2(std::string dataFile) {
    auto input = day15_0(dataFile);

    auto hsh = [](std::string_view sv) { return createHashFrom(sv); };

    size_t      accu{};
    std::vector fakeMap(256, std::vector(0, std::pair<std::string, size_t>{}));

    for (auto const &inpItem : input) {
        if (inpItem.empty()) { continue; }
        else if (inpItem.back() >= 48 && inpItem.back() < 58) {
            auto const str = std::string(inpItem.begin(), inpItem.end() - 2);

            if (auto it = std::ranges::find_if(fakeMap[hsh(str)], [&str](auto const &pr) { return pr.first == str; });
                it == fakeMap[hsh(str)].end()) {
                fakeMap[hsh(str)].push_back({str, inpItem.back() - 48});
            }
            else { it->second = inpItem.back() - 48; }
        }
        else if (inpItem.back() == '-') {
            auto const str = std::string(inpItem.begin(), inpItem.end() - 1);
            std::erase_if(fakeMap[hsh(str)], [&str](auto const &pr) { return pr.first == str; });
        }
        else { assert(false); }
    }

    for (size_t bucketID = 1uz; auto const &fakeMap_line : fakeMap) {
        for (size_t itemID = 1uz; auto const &[name, val] : fakeMap_line) { accu += bucketID * itemID++ * val; }
        bucketID++;
    }


    return accu;
}

} // namespace AOC2023