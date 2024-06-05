#include <iostream>
#include <fstream>
#include <queue>
#include <math.h>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"


namespace AOC2017 {
    int day3_1(std::string dataFile) {
        auto ctr = ctre::search<R"(\d+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctr);

        int input = std::stoi(VofV.front().front());

        auto sq = (int) std::sqrt(input - 1);
        sq += (sq % 2);

        int target = input - (sq - 1) * (sq - 1);
        target = target - (target / sq) * sq;

        int ans = abs(target - (sq / 2)) + sq / 2;;

        return ans;
    }

    // THIS DOESN'T WORK ... unnecessarily cumbersome :-)
    int day3_2(std::string dataFile) {
        auto ctr = ctre::search<R"(\d+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctr);

        int input = std::stoi(VofV.front().front());

        std::deque<std::deque<int> > spiral;
        int ans = INT_MIN;


        auto addNewCircle = [&]() {
            for (auto &line: spiral) {
                line.push_front(0);
                line.push_back(0);
            }
            spiral.push_front(std::deque<int>(spiral.front().size(), 0));
            spiral.push_back(std::deque<int>(spiral.front().size(), 0));
        };

        auto topLine = [&]() {
            auto itemFromL2F = (spiral.begin() + 1).operator*().rbegin() + 1;
            (spiral.begin() + 1).operator*().back() += itemFromL2F.operator*();
            (spiral.begin() + 2).operator*().back() += itemFromL2F.operator*();

            int i = 0;
            auto iterInsert = spiral.front().rbegin();
            while (itemFromL2F != (spiral.begin() + 1).operator*().rend() - 2) {
                if (itemFromL2F.operator*() > input) {
                    ans = itemFromL2F.operator*();
                    return;
                }
                (itemFromL2F + 1).operator*() += itemFromL2F.operator*();
                (iterInsert + i).operator*() += itemFromL2F.operator*();
                (iterInsert + i + 1).operator*() += itemFromL2F.operator*();
                (iterInsert + i + 2).operator*() += itemFromL2F.operator*();
                i++;
                ++itemFromL2F;
            }
            ((spiral.begin() + 2)->begin() + 1).operator*() += itemFromL2F.operator*();
        };
        auto bottomLine = [&]() {
            auto itemFromL2F = (spiral.end() - 1).operator*().begin() + 1;
            (spiral.end() - 1).operator*().front() += itemFromL2F.operator*();
            (spiral.end() - 2).operator*().front() += itemFromL2F.operator*();

            int i = 0;
            auto iterInsert = spiral.back().begin();
            while (itemFromL2F != (spiral.end() - 1).operator*().end() - 2) {
                if (itemFromL2F.operator*() > input) {
                    ans = itemFromL2F.operator*();
                    return;
                }
                (itemFromL2F + 1).operator*() += itemFromL2F.operator*();
                (iterInsert + i).operator*() += itemFromL2F.operator*();
                (iterInsert + i + 1).operator*() += itemFromL2F.operator*();
                (iterInsert + i + 2).operator*() += itemFromL2F.operator*();
                i++;
                ++itemFromL2F;
            }
        };

        auto rightLine = [&]() {
            auto itemFromL2F = (spiral.rbegin() + 2).operator*().rbegin() + 1;
            ((spiral.rbegin() + 1)->rend() + 1).operator*() += itemFromL2F.operator*();
            ((spiral.rbegin() + 1)->rend() + 2).operator*() += itemFromL2F.operator*();

            int i = 0;
            auto iterInsert = spiral.front().rbegin();
            while (itemFromL2F != (spiral.begin() + 1).operator*().rend() - 2) {
                if (itemFromL2F.operator*() > input) {
                    ans = itemFromL2F.operator*();
                    return;
                }

                (itemFromL2F + 1).operator*() += itemFromL2F.operator*();
                (spiral.rbegin() + i + 1).operator*().back() += itemFromL2F.operator*();
                (spiral.rbegin() + i + 2).operator*().back() += itemFromL2F.operator*();
                (spiral.rbegin() + i + 3).operator*().back() += itemFromL2F.operator*();
                i++;
                ++itemFromL2F;
            }
        };
        auto leftLine = [&]() {
            auto itemFromL2F = (spiral.begin() + 1).operator*().rbegin() + 1;
            (spiral.begin() + 1).operator*().back() += itemFromL2F.operator*();
            (spiral.begin() + 2).operator*().back() += itemFromL2F.operator*();

            int i = 0;
            auto iterInsert = spiral.front().rbegin();
            while (itemFromL2F != (spiral.begin() + 1).operator*().rend() - 2) {
                if (itemFromL2F.operator*() > input) {
                    ans = itemFromL2F.operator*();
                    return;
                }
                (itemFromL2F + 1).operator*() += itemFromL2F.operator*();
                (iterInsert + i).operator*() += itemFromL2F.operator*();
                (iterInsert + i + 1).operator*() += itemFromL2F.operator*();
                (iterInsert + i + 2).operator*() += itemFromL2F.operator*();
                i++;
                ++itemFromL2F;
            }
        };

        auto addLastPoint = [&]() {
        };

        while (true) {
            addNewCircle();

            rightLine();
            if (ans != INT_MIN) return ans;

            topLine();
            if (ans != INT_MIN) return ans;

            leftLine();
            if (ans != INT_MIN) return ans;

            bottomLine();
            if (ans != INT_MIN) return ans;

            addLastPoint();
            if (ans != INT_MIN) return ans;


            return ans;
        }
    }

    // BETTER IDEA. Check all surroundings always and only add if wasn't previously visited.
    int day3_3(std::string dataFile) {
        auto ctr = ctre::search<R"(\d+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctr);
        int input = std::stoi(VofV.front().front());

        struct spiralItem {
            int value = 0;
            bool finalized = false;
        };

        std::deque<std::deque<spiralItem> > spiral(3, std::deque<spiralItem>(3, spiralItem{1}));
        spiral[1][1].finalized = true;
        int ans = INT_MIN;

        std::vector<std::pair<int, int> > directions{
            {0, 1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {1, -1}, {1, 0}, {1, 1}
        };

        auto addNewCircle = [&]() {
            for (auto &line: spiral) {
                line.push_front(spiralItem());
                line.push_back(spiralItem());
            }
            spiral.push_front(std::deque<spiralItem>(spiral.front().size(), spiralItem()));
            spiral.push_back(std::deque<spiralItem>(spiral.front().size(), spiralItem()));
        };

        auto addToRelevantSurroundings = [&](auto const &itrLine, auto const &&itrCol) {
            for (auto const &dir: directions) {
                if (((itrLine + dir.first)->begin() + (itrCol - itrLine->begin()) + dir.second)->finalized) continue;
                else ((itrLine + dir.first)->begin() + (itrCol - itrLine->begin()) + dir.second)->value += itrCol->value;
            }
            itrCol->finalized = true;
            ans = itrCol.operator*().value;
        };

        int steps = 0;
        while (true) {
            steps += 2;
            auto lineIter = (spiral.end() - 2);
            addNewCircle();
            int itemInLine = (spiral.end() - 2)->size() - 2;
            addToRelevantSurroundings(lineIter, lineIter->begin() + itemInLine);

            for (int i = 0; i < steps-1; ++i) {
                --lineIter;
                addToRelevantSurroundings(lineIter, lineIter->begin() + itemInLine);
                if (ans > input) return ans;
            }

            for (int i = 0; i < steps; ++i) {
                itemInLine--;
                addToRelevantSurroundings(lineIter, lineIter->begin() + itemInLine);
                if (ans > input) return ans;
            }

            for (int i = 0; i < steps; ++i) {
                ++lineIter;
                addToRelevantSurroundings(lineIter, lineIter->begin() + itemInLine);
                if (ans > input) return ans;
            }

            for (int i = 0; i < steps ; ++i) {
                itemInLine++;
                addToRelevantSurroundings(lineIter, lineIter->begin() + itemInLine);
                if (ans > input) return ans;
            }
        }
        std::unreachable();
    }
}
