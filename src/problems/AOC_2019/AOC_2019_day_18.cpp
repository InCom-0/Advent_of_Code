#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <bitset>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <queue>


namespace AOC2019 {
long long day18_1(std::string dataFile) {
    auto                line_ctre = ctre::search<R"(.+)">;
    auto                input     = incom::commons::parseInputUsingCTRE::processFile(dataFile, line_ctre).front();
    std::pair<int, int> startLoc;

    std::vector<std::tuple<int, int, char>> keyLocations;

    // DATA PREP
    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input.at(0).size(); ++col) {
            if (input[row][col] > 96 && input[row][col] < 123) { keyLocations.push_back({row, col, input[row][col]}); }
            else if (input[row][col] == '@') { startLoc = {row, col}; }
        }
    }
    std::ranges::sort(keyLocations, [](auto &a, auto &b) { return std::get<2>(a) < std::get<2>(b); });
    keyLocations.push_back({startLoc.first, startLoc.second, std::get<2>(keyLocations.back()) + 1});
    input[startLoc.first][startLoc.second] = std::get<2>(keyLocations.back());

    // 1:All source destinations, 2: All target destinations (incl the source), 3: pair of distance, bitset of
    // 'doors' (door on route == bit0)
    std::vector shortestDistances(keyLocations.size(),
                                  std::vector(keyLocations.size(), std::make_pair(INT_MAX, std::bitset<32>())));
    for (int i = 0; i < shortestDistances.size(); ++i) {
        shortestDistances[i][i].first = 0;
        shortestDistances[i][i].second.set();
    }


    // Find all 'shortest' paths from each key to each other key
    for (int srcID = 0; srcID < keyLocations.size(); ++srcID) {
        auto                                                                            map2Explore = input;
        incom::commons::doubleBuffer<std::queue<std::tuple<int, int, std::bitset<32>>>> buf_queues;

        // Push source location to queue
        buf_queues.getNext().push({std::get<0>(keyLocations[srcID]), std::get<1>(keyLocations[srcID]),
                                   shortestDistances[srcID][srcID].second});
        // Mark source location as 'visited'
        map2Explore.at(std::get<0>(keyLocations[srcID])).at(std::get<1>(keyLocations[srcID])) = '/';

        int  stepsCount = 0;
        auto explore    = [&]() -> void {
            constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
            auto const                                       &pos = buf_queues.getCurrent().front();

            for (auto const &di : dirs) {
                std::tuple<int, int, std::bitset<32>> newPos{std::get<0>(pos) + di.front(),
                                                             std::get<1>(pos) + di.back(), std::get<2>(pos)};
                // Wall or explored previously, do nothing, continue
                if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] == '#' ||
                    map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] == '/') {
                    continue;
                }

                // Empty position on route, put in next queue
                else if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] == '.') {}

                // Door hit, mark in bitset, put in next queue
                else if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] >= 'A' &&
                         map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] <= 'Z') {
                    // Set the right door in bitset of 'doors' to false (meaning that door is on the route considered)
                    std::get<2>(newPos)[map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] - 'A'] = false;
                }

                // Key hit, one of targets reached, put in next queue
                else if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] >= 'a' &&
                         map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] <= '{') {
                    shortestDistances[srcID][map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] - 'a'] = {
                        stepsCount, std::get<2>(newPos)};
                }

                else { assert((void("Impossible character in map being explored"), false)); }
                buf_queues.getNext().push({newPos});
                map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] = '/';
            }
        };

        while (not buf_queues.getNext().empty()) {
            buf_queues.swapBuffers();
            stepsCount++;
            while (not buf_queues.getCurrent().empty()) {
                explore();
                buf_queues.getCurrent().pop();
            }
        }
    }

    incom::commons::doubleBuffer<
        ankerl::unordered_dense::map<std::pair<size_t, unsigned long>, size_t, incom::commons::XXH3Hasher>>
        buf_maps;

    std::bitset<32> thisBS;
    thisBS.set(keyLocations.size() - 1, true);
    buf_maps.getNext().insert({{keyLocations.size() - 1, thisBS.to_ulong()}, 0});

    // MAIN LOGIC
    while (not buf_maps.getNext().empty()) {
        buf_maps.swapBuffers();
        buf_maps.getNext().clear();

        for (auto &srcLoc : buf_maps.getCurrent()) {
            auto possibilities =
                std::views::enumerate(shortestDistances.at(srcLoc.first.first)) | std::views::filter([&](auto &&item) {
                    auto z = std::bitset<32>(srcLoc.first.second) | (std::get<1>(item).second);
                    return z.all() && (not std::bitset<32>(srcLoc.first.second).test(std::get<0>(item)));
                });
            for (auto const &poss : possibilities) {
                thisBS = std::bitset<32>(srcLoc.first.second);
                thisBS.set(std::get<0>(poss), true);
                auto insIter = buf_maps.getNext().insert(
                    {{std::get<0>(poss), thisBS.to_ulong()}, srcLoc.second + std::get<1>(poss).first});

                if (insIter.first->second > srcLoc.second + std::get<1>(poss).first) {
                    insIter.first->second = srcLoc.second + std::get<1>(poss).first;
                }
            }
        }
    }
    return std::ranges::min_element(buf_maps.getCurrent(), [](auto &&a, auto &&b) { return a.second < b.second; })
        ->second;
}

long long day18_2(std::string dataFile) {
    auto                line_ctre = ctre::search<R"(.+)">;
    auto                input     = incom::commons::parseInputUsingCTRE::processFile(dataFile, line_ctre).front();
    std::pair<int, int> startLoc;

    std::vector<std::tuple<int, int, char>> keyLocations;

    // DATA PREP
    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input.at(0).size(); ++col) {
            if (input[row][col] > 96 && input[row][col] < 123) { keyLocations.push_back({row, col, input[row][col]}); }
            else if (input[row][col] == '@') { startLoc = {row, col}; }
        }
    }

    std::array<std::array<char, 3>, 3> changes{'@', '#', '@', '#', '#', '#', '@', '#', '@'};
    for (int r = startLoc.first - 1; r < startLoc.first + 2; ++r) {
        for (int c = startLoc.second - 1; c < startLoc.second + 2; ++c) {
            input[r][c] = changes[r - (startLoc.first - 1)][c - (startLoc.second - 1)];
        }
    }

    std::array<std::array<int, 2>, 4> startLocs{startLoc.first - 1,  startLoc.second - 1, startLoc.first - 1,
                                                startLoc.second + 1, startLoc.first + 1,  startLoc.second - 1,
                                                startLoc.first + 1,  startLoc.second + 1};

    std::ranges::sort(keyLocations, [](auto &a, auto &b) { return std::get<2>(a) < std::get<2>(b); });
    for (auto const &oneStart : startLocs) {
        input[oneStart.front()][oneStart.back()] = std::get<2>(keyLocations.back()) + 1;
        keyLocations.push_back({oneStart.front(), oneStart.back(), std::get<2>(keyLocations.back()) + 1});
    }

    // 1:All source destinations, 2: All target destinations (incl the source), 3: pair of distance, bitset of
    // 'doors' (door on route == bit0)
    std::vector shortestDistances(keyLocations.size(),
                                  std::vector(keyLocations.size(), std::make_pair(INT_MAX, std::bitset<32>())));
    for (int i = 0; i < shortestDistances.size(); ++i) {
        shortestDistances[i][i].first = 0;
        shortestDistances[i][i].second.set();
    }


    // Find all 'shortest' paths from each key to each other key
    for (int srcID = 0; srcID < keyLocations.size(); ++srcID) {
        auto                                                                            map2Explore = input;
        incom::commons::doubleBuffer<std::queue<std::tuple<int, int, std::bitset<32>>>> buf_queues;

        // Push source location to queue
        buf_queues.getNext().push({std::get<0>(keyLocations[srcID]), std::get<1>(keyLocations[srcID]),
                                   shortestDistances[srcID][srcID].second});
        // Mark source location as 'visited'
        map2Explore.at(std::get<0>(keyLocations[srcID])).at(std::get<1>(keyLocations[srcID])) = '/';

        int  stepsCount = 0;
        auto explore    = [&]() -> void {
            constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
            auto const                                       &pos = buf_queues.getCurrent().front();

            for (auto const &di : dirs) {
                std::tuple<int, int, std::bitset<32>> newPos{std::get<0>(pos) + di.front(),
                                                             std::get<1>(pos) + di.back(), std::get<2>(pos)};
                // Wall or explored previously, do nothing, continue
                if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] == '#' ||
                    map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] == '/') {
                    continue;
                }

                // Empty position on route, put in next queue
                else if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] == '.') {}

                // Door hit, mark in bitset, put in next queue
                else if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] >= 'A' &&
                         map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] <= 'Z') {
                    // Set the right door in bitset of 'doors' to false (meaning that door is on the route considered)
                    std::get<2>(newPos)[map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] - 'A'] = false;
                }

                // Key hit, one of targets reached, put in next queue
                else if (map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] >= 'a' &&
                         map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] <= '~') {
                    shortestDistances[srcID][map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] - 'a'] = {
                        stepsCount, std::get<2>(newPos)};
                }

                else { assert((void("Impossible character in map being explored"), false)); }
                buf_queues.getNext().push({newPos});
                map2Explore[std::get<0>(newPos)][std::get<1>(newPos)] = '/';
            }
        };

        while (not buf_queues.getNext().empty()) {
            buf_queues.swapBuffers();
            stepsCount++;
            while (not buf_queues.getCurrent().empty()) {
                explore();
                buf_queues.getCurrent().pop();
            }
        }
    }

    incom::commons::doubleBuffer<
        ankerl::unordered_dense::map<std::pair<size_t, unsigned long>, size_t, incom::commons::XXH3Hasher>>
        buf_maps;

    std::bitset<32> thisBS;
    thisBS.set(keyLocations.size() - 1, true);
    buf_maps.getNext().insert({{keyLocations.size() - 1, thisBS.to_ulong()}, 0});

    // MAIN LOGIC
    while (not buf_maps.getNext().empty()) {
        buf_maps.swapBuffers();
        buf_maps.getNext().clear();

        for (auto &srcLoc : buf_maps.getCurrent()) {
            auto possibilities =
                std::views::enumerate(shortestDistances.at(srcLoc.first.first)) | std::views::filter([&](auto &&item) {
                    auto z = std::bitset<32>(srcLoc.first.second) | (std::get<1>(item).second);
                    return z.all() && (not std::bitset<32>(srcLoc.first.second).test(std::get<0>(item)));
                });
            for (auto const &poss : possibilities) {
                thisBS = std::bitset<32>(srcLoc.first.second);
                thisBS.set(std::get<0>(poss), true);
                auto insIter = buf_maps.getNext().insert(
                    {{std::get<0>(poss), thisBS.to_ulong()}, srcLoc.second + std::get<1>(poss).first});

                if (insIter.first->second > srcLoc.second + std::get<1>(poss).first) {
                    insIter.first->second = srcLoc.second + std::get<1>(poss).first;
                }
            }
        }
    }
    return std::ranges::min_element(buf_maps.getCurrent(), [](auto &&a, auto &&b) { return a.second < b.second; })
        ->second;
}
} // namespace AOC2019