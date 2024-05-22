#include <iostream>
#include <fstream>
#include <queue>
#include <cassert>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {

/*
Recursive lambdas to the rescue.
Design pattern inspired by 'deducing this' feature of C++23, but not actually using it since GCC 13 doesnt support it atm ...
Technically the following solution should be identical for this purpouse (although not possible to do everything 'deducing this' enables).
Essentially passing a lambda to itself as 'auto deduced' parameter.
*/
long long day24_1(std::string dataFile, int numberOfGroups) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return -1;
    assert(numberOfGroups >= 3);

    std::string oneStr;
    std::vector<int> inputNumbers;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\d+)">(bg, oneStr.end());
        inputNumbers.push_back(re.to_number());
    }

    std::ranges::sort(inputNumbers, std::greater());
    int weightPortion = std::ranges::fold_left_first(inputNumbers, std::plus()).value_or(0);
    if (weightPortion % numberOfGroups == 0) weightPortion /= numberOfGroups;
    else return -1;

    std::vector<std::vector<long long> > ans;
    std::vector<std::vector<long long>> groups (numberOfGroups, std::vector<long long>()); // first group is special ie. needs to have smalest number of items && lowest quantum entanglement
    std::vector<int> groupSums (numberOfGroups, 0);
    std::vector<std::vector<int>> masks(numberOfGroups-1, std::vector<int>(inputNumbers.size(), 1));

    size_t minGroup1 = INT_MAX;
    int currentLevel = 0;
    std::vector<std::vector<int>> inputsIntoNextStage (numberOfGroups-1, std::vector<int>());

    auto filter1 = [](auto &&a) { return a.second; };
    auto map1 = [](auto &&a) { return a.first; };

    /*
    groupSelectorOthers is a recursive lambda that traverses the groups until it finds a valid 'solution',
    then it rolls back up and exits back into group1Selector (it only need to find one such solution, hence reducing complexity to manageable level)

    group1Selector is also a recursive lambda that finds all the valid 'group 0' combinations of the lowest number of members.
    */
    auto groupSelectorOthers = [&](auto const &self, int startPos) -> void {
        for (int i = startPos; i < inputsIntoNextStage[currentLevel].size(); ++i) {
            groupSums[currentLevel] += inputsIntoNextStage[currentLevel][i];        // select one to add on this level
            masks[currentLevel][i] = 0;                                             // update mask (used only when going to the next level)

            if (groupSums[currentLevel] == weightPortion) {
                if (currentLevel == numberOfGroups-2) {                             // Must be valid answer
                    ans.push_back(groups[0]);
                    minGroup1 = std::min(minGroup1, ans.front().size());
                }
                else {                                                              // Try to find possible combination on next level
                    inputsIntoNextStage[currentLevel+1] = flux::zip(flux::ref(inputsIntoNextStage[currentLevel]), flux::ref(masks[currentLevel]))
                        .filter(filter1)
                        .map(map1)
                        .to<std::vector<int> >();
                    currentLevel++;
                    self(self,0);

                    masks[currentLevel] = std::vector<int>(inputNumbers.size(), 1);
                    groupSums[currentLevel] = 0;
                    currentLevel--;
                }
            }
            else if (groupSums[currentLevel] < weightPortion) self(self, i + 1);        // same level, need more numbers to satisfy weight
            if (groupSums[currentLevel] == weightPortion && ans.back() == groups[0]) return;   // only need to find 1 valid combination for each valid groups[0]
            masks[currentLevel][i] = 1;                                                        // removing
            groupSums[currentLevel] -= inputsIntoNextStage[currentLevel][i];                   // removing
        }
    };

    auto group1Selector = [&](auto const &self, int startPos) -> void {
        for (int i = startPos; i < inputNumbers.size(); ++i) {
            groupSums[0] += inputNumbers[i];
            groups[0].push_back(inputNumbers[i]);
            masks[0][i] = 0;
            if (groupSums[0] == weightPortion && minGroup1 >= groups[0].size()) {

                inputsIntoNextStage[currentLevel+1] = flux::zip(flux::ref(inputNumbers), flux::ref(masks[currentLevel]))
                        .filter(filter1)
                        .map(map1)
                        .to<std::vector<int> >();

                currentLevel++;
                groupSelectorOthers(groupSelectorOthers, 0);
                currentLevel--;
                masks[1] = std::vector<int>(inputNumbers.size(), 1);
                groups[1].clear();
                groupSums[1] = 0;
            } else if (groupSums[0] < weightPortion) self(self, i + 1);
            masks[0][i] = 1;
            groups[0].pop_back();
            groupSums[0] -= inputNumbers[i];
        }
    };

    group1Selector(group1Selector, 0);
    auto quantumReturn = flux::ref(ans)
                        .map ([] (auto &&a) {return flux::ref(a).fold(std::multiplies(), 1);})
                        .min([] (auto &&a, auto &&b) {return a < b;})
                        .value_or(-1);
    return quantumReturn;
}

}