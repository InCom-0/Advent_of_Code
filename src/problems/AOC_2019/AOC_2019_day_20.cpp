#include <ankerl/unordered_dense.h>
#include <cstdint>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>
#include <queue>
#include <string>


namespace AOC2019 {
long long day20_1(std::string dataFile) {
    auto line_ctre = ctre::search<R"(.+)">;
    auto input     = incom::commons::parseInputUsingCTRE::processFile(dataFile, line_ctre).front();
    auto inputOrig = input;

    std::pair<int, int>                         startPos;
    constexpr char const                        visited = '/';
    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    ankerl::unordered_dense::map<std::string, std::pair<int, int>, incom::commons::XXH3Hasher> portalMap;

    // FIND START
    for (int row = 0; row < (input.size() - 1); row++) {

        for (int col = 0; col < (input.front().size() - 1); ++col) {
            if (input[row][col] >= 'A' && input[row][col] <= 'Z') {
                if (input[row + 1][col] != ' ' && input[row + 1][col] != '.') {
                    std::string inputSymbol;
                    if (row < 5 || row > (input.size() - 5)) {
                        inputSymbol = {input[row][col], input[row + 1][col], '^'};
                    }
                    else { inputSymbol = {input[row][col], input[row + 1][col], 'v'}; }
                    if (row + 2 == input.size() || input[row + 2][col] == ' ') { startPos = {row - 1, col}; }
                    else { startPos = {row + 2, col}; }
                    portalMap.emplace(inputSymbol, std::pair<int, int>{startPos.first, startPos.second});
                }
                else if (input[row][col + 1] != ' ' && input[row][col + 1] != '.') {
                    std::string inputSymbol;
                    if (col < 5 || col > (input.front().size() - 5)) {
                        inputSymbol = {input[row][col], input[row][col + 1], '^'};
                    }
                    else { inputSymbol = {input[row][col], input[row][col + 1], 'v'}; }

                    if (col + 2 == input.front().size() || input[row][col + 2] == ' ') { startPos = {row, col - 1}; }
                    else { startPos = {row, col + 2}; }
                    portalMap.emplace(inputSymbol, std::pair<int, int>{startPos.first, startPos.second});
                }
            }
        }
    }
    portalMap.emplace("AAv", portalMap.at("AA^"));
    portalMap.emplace("ZZv", portalMap.at("ZZ^"));


    ankerl::unordered_dense::map<std::pair<int, int>, std::pair<int, int>, incom::commons::XXH3Hasher> locationsLinks;
    for (auto &pm_line : portalMap) {
        std::string theOtherPortal = pm_line.first;
        if (theOtherPortal.back() == 'v') { theOtherPortal.back() = '^'; }
        else if (theOtherPortal.back() == '^') { theOtherPortal.back() = 'v'; }

        locationsLinks.emplace(pm_line.second, portalMap.at(theOtherPortal));
    }

    auto const                                                    terminalLocation = portalMap.at("ZZ^");
    incom::commons::doubleBuffer<std::queue<std::pair<int, int>>> buf_que;

    auto explore = [&](std::pair<int, int> const &pr) -> std::optional<std::pair<int, int>> {
        std::pair<int, int> newLoc;
        input[pr.first][pr.second] = visited;
        for (auto const &dir : dirs) {
            newLoc.first  = pr.first + dir.front();
            newLoc.second = pr.second + dir.back();

            if (input[newLoc.first][newLoc.second] >= 'A' && input[newLoc.first][newLoc.second] <= 'Z') { return pr; }
            else if (input[newLoc.first][newLoc.second] == '.') { buf_que.getNext().push(newLoc); }
        }
        return std::nullopt;
    };

    ankerl::unordered_dense::map<std::pair<int, int>, std::vector<std::pair<size_t, std::pair<int, int>>>,
                                 incom::commons::XXH3Hasher>
        walkConnections;

    // Explore from all portals too every reachable portal
    for (auto &srcLoc : locationsLinks) {
        input = inputOrig;
        walkConnections.emplace(srcLoc.first, std::vector<std::pair<size_t, std::pair<int, int>>>());
        buf_que.getNext().push(srcLoc.first);
        input[srcLoc.first.first][srcLoc.first.second] = visited;
        for (auto const &dir : dirs) {
            if (input[srcLoc.first.first + dir.front()][srcLoc.first.second + dir.back()] == '.') {
                buf_que.getNext().front().first  += dir.front();
                buf_que.getNext().front().second += dir.back();
                break;
            }
        }

        size_t dist = 1;
        while (not buf_que.getNext().empty()) {
            buf_que.swapBuffers();
            while (not buf_que.getCurrent().empty()) {
                auto maybePortal = explore(buf_que.getCurrent().front());
                maybePortal.and_then([&](auto &item) -> std::optional<std::pair<int, int>> {
                    walkConnections.at(srcLoc.first).push_back({dist, item});
                    return item;
                });
                buf_que.getCurrent().pop();
            }
            dist++;
        }
    }
    ankerl::unordered_dense::map<std::pair<int, int>, size_t, incom::commons::XXH3Hasher> minDistancesFound;

    for (auto const &portal : locationsLinks) { minDistancesFound.emplace(portal.first, SIZE_MAX); }

    std::queue<std::pair<std::pair<int, int>, size_t>> que_2;
    que_2.push({portalMap.at("AA^"), 0});

    while (not que_2.empty()) {
        for (auto const &possi : walkConnections.at(que_2.front().first)) {
            auto &walkMinDist = minDistancesFound.at(possi.second);
            if (walkMinDist > (que_2.front().second + possi.first)) {
                walkMinDist = que_2.front().second + possi.first;
                if (possi.second == terminalLocation) { continue; } // Reached the destination

                auto const &posAfterPortal     = locationsLinks.at(possi.second);
                auto       &minDistAfterPortal = minDistancesFound.at(posAfterPortal);
                if (minDistAfterPortal > (walkMinDist + 1)) {
                    minDistAfterPortal = walkMinDist + 1;
                    que_2.push({posAfterPortal, minDistAfterPortal});
                }
            }
        }
        que_2.pop();
    }

    return minDistancesFound.at(terminalLocation);
}

long long day20_2(std::string dataFile) {
    auto line_ctre = ctre::search<R"(.+)">;
    auto input     = incom::commons::parseInputUsingCTRE::processFile(dataFile, line_ctre).front();
    auto inputOrig = input;

    std::pair<int, int>                         startPos;
    constexpr char const                        visited = '/';
    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    ankerl::unordered_dense::map<std::string, std::pair<int, int>, incom::commons::XXH3Hasher> portalMap;

    // CREATE PORTAL MAP
    for (int row = 0; row < (input.size() - 1); row++) {
        for (int col = 0; col < (input.front().size() - 1); ++col) {
            if (input[row][col] >= 'A' && input[row][col] <= 'Z') {
                if (input[row + 1][col] != ' ' && input[row + 1][col] != '.') {
                    std::string inputSymbol;
                    if (row < 5 || row > (input.size() - 5)) {
                        inputSymbol = {input[row][col], input[row + 1][col], '^'};
                    }
                    else { inputSymbol = {input[row][col], input[row + 1][col], 'v'}; }
                    if (row + 2 == input.size() || input[row + 2][col] == ' ') { startPos = {row - 1, col}; }
                    else { startPos = {row + 2, col}; }
                    portalMap.emplace(inputSymbol, std::pair<int, int>{startPos.first, startPos.second});
                }
                else if (input[row][col + 1] != ' ' && input[row][col + 1] != '.') {
                    std::string inputSymbol;
                    if (col < 5 || col > (input.front().size() - 5)) {
                        inputSymbol = {input[row][col], input[row][col + 1], '^'};
                    }
                    else { inputSymbol = {input[row][col], input[row][col + 1], 'v'}; }

                    if (col + 2 == input.front().size() || input[row][col + 2] == ' ') { startPos = {row, col - 1}; }
                    else { startPos = {row, col + 2}; }
                    portalMap.emplace(inputSymbol, std::pair<int, int>{startPos.first, startPos.second});
                }
            }
        }
    }
    portalMap.emplace("AAv", portalMap.at("AA^"));
    portalMap.emplace("ZZv", portalMap.at("ZZ^"));

    // CREATE A MAP OF PORTAL/TELEPORT CONNECTIONS (same symbols to each other)
    ankerl::unordered_dense::map<std::pair<int, int>, std::pair<int, int>, incom::commons::XXH3Hasher> portalLinks;
    for (auto &pm_line : portalMap) {
        std::string theOtherPortal = pm_line.first;
        if (theOtherPortal.back() == 'v') { theOtherPortal.back() = '^'; }
        else if (theOtherPortal.back() == '^') { theOtherPortal.back() = 'v'; }
        portalLinks.emplace(pm_line.second, portalMap.at(theOtherPortal));
    }
    portalMap.erase("AAv");
    portalMap.erase("ZZv");

    // CREATE A MAP OF WALK CONNECTIONS
    // Specify terminal location (that is 'ZZ^')
    std::pair<std::pair<int, int>, size_t> const startLocation{portalMap.at("AA^"), 0};
    std::pair<std::pair<int, int>, size_t> const terminalLocation{portalMap.at("ZZ^"), 0};

    ankerl::unordered_dense::map<std::pair<int, int>, std::vector<std::pair<size_t, std::pair<int, int>>>,
                                 incom::commons::XXH3Hasher>
                                                                  walkConnections;
    incom::commons::doubleBuffer<std::queue<std::pair<int, int>>> buf_que;

    // Explore func - evaluates surrounding of point on path and acts accordingly
    auto explore = [&](std::pair<int, int> const &pr) -> std::optional<std::pair<int, int>> {
        std::pair<int, int> newLoc;
        input[pr.first][pr.second] = visited;
        for (auto const &dir : dirs) {
            newLoc.first  = pr.first + dir.front();
            newLoc.second = pr.second + dir.back();

            if (input[newLoc.first][newLoc.second] >= 'A' && input[newLoc.first][newLoc.second] <= 'Z') { return pr; }
            else if (input[newLoc.first][newLoc.second] == '.') { buf_que.getNext().push(newLoc); }
        }
        return std::nullopt;
    };

    // Perform walk exploration from each portal to actually create 'map of walk connections'
    for (auto &srcLoc : portalLinks) {
        // Setup
        input = inputOrig;
        walkConnections.emplace(srcLoc.first, std::vector<std::pair<size_t, std::pair<int, int>>>());
        buf_que.getNext().push(srcLoc.first);
        input[srcLoc.first.first][srcLoc.first.second] = visited;
        // Must start exploring from the 'second' step not the first (to block the way we came in)
        for (auto const &dir : dirs) {
            if (input[srcLoc.first.first + dir.front()][srcLoc.first.second + dir.back()] == '.') {
                buf_que.getNext().front().first  += dir.front();
                buf_que.getNext().front().second += dir.back();
                break;
            }
        }
        size_t dist = 1;

        // Exploration part
        while (not buf_que.getNext().empty()) {
            buf_que.swapBuffers();
            while (not buf_que.getCurrent().empty()) {
                auto maybePortal = explore(buf_que.getCurrent().front());
                maybePortal.and_then([&](auto &item) -> std::optional<std::pair<int, int>> {
                    walkConnections.at(srcLoc.first).push_back({dist, item});
                    return item;
                });
                buf_que.getCurrent().pop();
            }
            dist++;
        }
    }

    ankerl::unordered_dense::map<std::pair<int, int>, char, incom::commons::XXH3Hasher> portalInOrOut;
    for (auto const &port : portalMap) {
        if (port.first.back() == '^') { portalInOrOut.emplace(port.second, '^'); }
        else if (port.first.back() == 'v') { portalInOrOut.emplace(port.second, 'v'); }
        else { assert((void("The 3rd character of portal symbol is not '^' or 'v'"), false)); }
    }

    // 1: pair of 'location' and 'level', 2: min distance travelled
    ankerl::unordered_dense::map<std::pair<std::pair<int, int>, size_t>, size_t, incom::commons::XXH3Hasher>
        minDistancesFound;

    std::queue<std::pair<std::pair<std::pair<int, int>, size_t>, size_t>> que_2;
    que_2.push({{portalMap.at("AA^"), 0}, 0});

    size_t curFoundDistToTerminal = SIZE_MAX;
    while (not que_2.empty()) {
        // For each 'walking possibility'
        for (auto const &possi : walkConnections.at(que_2.front().first.first)) {

            // Try to insert a entry with key of 'location, level' and value: 'distance so far'
            auto walkMinDist = minDistancesFound.insert(
                {{possi.second, que_2.front().first.second}, que_2.front().second + possi.first});

            // Check if really did insert with 'distance so far' lower than the one before
            if (walkMinDist.first->second >= (que_2.front().second + possi.first) &&
                curFoundDistToTerminal > (que_2.front().second + possi.first)) {
                // If so update the the distance the new lower distance
                walkMinDist.first->second = que_2.front().second + possi.first;

                // Check if this is not the terminalLocation
                if (possi.second == terminalLocation.first) {
                    if (que_2.front().first.second == 0) {
                        // If so, mark update the 'dist to terminal found'
                        curFoundDistToTerminal = walkMinDist.first->second;
                    }
                    continue;
                }
                if (possi.second == startLocation.first) { continue; }

                auto const &posAfterPortal = portalLinks.at(possi.second);
                // check if we go up or down
                int         upOrDown       = (portalInOrOut.at(possi.second) == '^' ? -1 : 1);

                // If we would go outside the outtermost level, we 'continue' which means terminating search here
                if (que_2.front().first.second == 0 && upOrDown == -1) { continue; }
                size_t newLvl   = que_2.front().first.second + upOrDown;
                size_t wMD_dist = walkMinDist.first->second + 1;

                // Try to insert a entry with key of 'location, level' and value: 'distance so far'
                auto minDistAfterPortal = minDistancesFound.insert({{posAfterPortal, newLvl}, wMD_dist});
                // Check if really did insert with 'distance so far' lower than the one before
                if (minDistAfterPortal.first->second >= (wMD_dist)) {
                    // If so update the the distance the new lower distance and input this location to the queue
                    minDistAfterPortal.first->second = wMD_dist;
                    que_2.push({{posAfterPortal, newLvl}, minDistAfterPortal.first->second});
                }
            }
        }
        que_2.pop();
    }

    return minDistancesFound.at(terminalLocation);
}
} // namespace AOC2019