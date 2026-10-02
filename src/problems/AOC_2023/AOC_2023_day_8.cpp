#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <numeric>
#include <ranges>
#include <string>


namespace AOC2023 {


auto
day8_0(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;

    auto processed = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);
    ankerl::unordered_dense::map<std::string, std::size_t, incstd::hashing::XXH3Hasher> map;

    std::size_t id = 0uz;
    for (auto const &inpLine : std::span(processed.begin() + 2, processed.end())) {
        for (auto const &str : inpLine) {
            auto it  = map.insert({str, id});
            id      += it.second;
        }
    }
    auto res = std::make_tuple(processed.front().front(), std::vector<std::array<size_t, 2>>(map.size(), {{}}),
                               map.at("AAA"), map.at("ZZZ"));
    for (auto const &inpLine : std::span(processed.begin() + 2, processed.end())) {
        std::get<1>(res)[map.at(inpLine[0])] = {map.at(inpLine[1]), map.at(inpLine[2])};
    }

    return res;
}

size_t
day8_1(std::string dataFile) {

    auto const [dirs, map, start, end] = day8_0(dataFile);

    size_t steps      = 0uz;
    size_t dirsCursor = 0uz;
    size_t curPlace   = start;

    while (curPlace != end) {
        curPlace    = map[curPlace][0 + (dirs[dirsCursor++] == 'R')];
        dirsCursor *= (dirsCursor != dirs.size());
        steps++;
    }


    return steps;
}


size_t
day8_2(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;

    auto processed = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);
    ankerl::unordered_dense::map<std::string, std::size_t, incstd::hashing::XXH3Hasher> map;


    for (std::size_t id = 0uz; auto const &inpLine : std::span(processed.begin() + 2, processed.end())) {
        for (auto const &str : inpLine) {
            auto it  = map.insert({str, id});
            id      += it.second;
        }
    }

    std::vector<size_t> curIDs;
    std::vector<size_t> endIDs;

    auto const &dirs      = processed.front().front();
    auto        lookupVec = std::vector<std::array<size_t, 2>>(map.size(), {{}});

    for (auto const &inpLine : std::span(processed.begin() + 2, processed.end())) {
        lookupVec[map.at(inpLine[0])] = {map.at(inpLine[1]), map.at(inpLine[2])};
        if (inpLine[0][2] == 'A') { curIDs.push_back(map.at(inpLine[0])); }
        if (inpLine[0][2] == 'Z') { endIDs.push_back(map.at(inpLine[0])); }
    }

    std::vector pastZresults = std::vector<std::vector<std::pair<size_t, size_t>>>(curIDs.size());
    size_t      dirSZ        = dirs.size();

    for (size_t id = 0uz; auto &curID : curIDs) {

        size_t dirsCursor = 0uz;
        size_t steps      = 0uz;
        while (true) {
            steps++;
            curID = lookupVec[curID][0 + (dirs.at(dirsCursor++) == 'R')];
            if (std::ranges::find(endIDs, curID) != endIDs.end()) {

                if (std::ranges::find_if(pastZresults[id], [&](auto const &pr) {
                        return (std::get<0>(pr) == steps && std::get<1>(pr) == curID);
                    }) != pastZresults[id].end()) {
                    break;
                }
                else {
                    pastZresults[id].push_back({steps, curID});
                    steps = 0uz;
                }
            }

            dirsCursor *= (dirsCursor != dirs.size());
        }
        id++;
    }

    return std::ranges::fold_left(pastZresults, 1uz, [](std::size_t init, auto const &prItem) {
        return std::lcm(init, std::get<0>(prItem.front()));
    });
}
} // namespace AOC2023