
#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <queue>
#include <unordered_set>


namespace AOC2018 {
struct node {
    char                           nodeName;
    int                            bitCounter           = 0;
    bit::bit_vector<unsigned char> mustFinishBeforeThis = bit::bit_vector<unsigned char>(26, bit::bit0);
};

std::string day7_1(std::string dataFile) {
    // CUSTOM TYPES
    auto oneLetter_ctre = ctre::search<R"((?<= ).(?= ))">;
    auto VofV           = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, oneLetter_ctre);

    std::vector<node>                                 lockedOnes;
    std::unordered_set<char, AOC_commons::XXH3Hasher> tmpOnes;

    // DATA PREP
    for (auto &line : VofV) {
        auto found = std::ranges::find_if(lockedOnes, [&](auto &&a) { return a.nodeName == line.back().front(); });
        if (found == lockedOnes.end()) {
            bit::bit_vector<unsigned char> bv = bit::bit_vector<unsigned char>(26, bit::bit0);
            bv[line.front().front() - 65]     = bit::bit1;
            lockedOnes.push_back({line.back().front(), 1, bv});
        }
        else {
            (*found).mustFinishBeforeThis[line.front().front() - 65] = bit::bit1;
            (*found).bitCounter++;
        }
    }
    for (auto &line : VofV) {
        if (std::ranges::find_if(lockedOnes, [&](auto &&a) { return a.nodeName == line.front().front(); }) ==
            lockedOnes.end()) {
            tmpOnes.insert(line.front().front());
        }
    }
    std::priority_queue openOnes(tmpOnes.begin(), tmpOnes.end(), std::greater());

    // MAIN LOGIC
    std::string res;
    while (not openOnes.empty()) {
        res.push_back(openOnes.top());
        openOnes.pop();
        int id = res.back() - 65;
        for (auto &nd : lockedOnes) {
            if (nd.mustFinishBeforeThis[id] == bit::bit1) {
                nd.mustFinishBeforeThis[id].flip();
                nd.bitCounter--;
                if (nd.bitCounter == 0) { openOnes.push(nd.nodeName); }
            }
        }
    }
    return res;
}

int day7_2(std::string dataFile, const int workers) {
    // CUSTOM TYPES
    auto oneLetter_ctre = ctre::search<R"((?<= ).(?= ))">;
    auto VofV           = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, oneLetter_ctre);

    std::vector<node>                                 lockedOnes;
    std::unordered_set<char, AOC_commons::XXH3Hasher> tmpOnes;

    // DATA PREP
    for (auto &line : VofV) {
        auto found = std::ranges::find_if(lockedOnes, [&](auto &&a) { return a.nodeName == line.back().front(); });
        if (found == lockedOnes.end()) {
            bit::bit_vector<unsigned char> bv = bit::bit_vector<unsigned char>(26, bit::bit0);
            bv[line.front().front() - 65]     = bit::bit1;
            lockedOnes.push_back({line.back().front(), 1, bv});
        }
        else {
            (*found).mustFinishBeforeThis[line.front().front() - 65] = bit::bit1;
            (*found).bitCounter++;
        }
    }
    for (auto &line : VofV) {
        if (std::ranges::find_if(lockedOnes, [&](auto &&a) { return a.nodeName == line.front().front(); }) ==
            lockedOnes.end()) {
            tmpOnes.insert(line.front().front());
        }
    }
    std::priority_queue            openOnes(tmpOnes.begin(), tmpOnes.end(), std::greater());
    int                            workersAvailable = workers;
    int                            round            = 0;
    std::vector<std::vector<char>> rounds;
    rounds.push_back(std::vector<char>());

    std::string                    res;

    auto finishStep = [&](char &stepToFinish) {
        res.push_back(stepToFinish);
        int id = stepToFinish - 65;
        for (auto &nd : lockedOnes) {
            if (nd.mustFinishBeforeThis[id] == bit::bit1) {
                nd.mustFinishBeforeThis[id].flip();
                nd.bitCounter--;
                if (nd.bitCounter == 0) { openOnes.push(nd.nodeName); }
            }
        }
        workersAvailable++;
    };

    // MAIN LOGIC
    while (not openOnes.empty() || workersAvailable < workers) {
        for (auto &chr : rounds[round]) { finishStep(chr); }

        while (workersAvailable && not openOnes.empty()) {
            int targetRound = round + (openOnes.top() - 4);
            if (targetRound > rounds.size()) { rounds.resize(targetRound+1); }

            rounds[targetRound].push_back(openOnes.top());
            openOnes.pop();
            workersAvailable--;
        }
        round++;
    }
    return round-1;
}
} // namespace AOC2018