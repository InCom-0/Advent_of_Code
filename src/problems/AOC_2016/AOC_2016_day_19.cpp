#include <complex>
#include <iostream>
#include <fstream>
#include <queue>
#include <ranges>

#include <AOC_commons.h>
#include <list>

#include "flux.hpp"


namespace AOC2016 {
    int day19_1(int numOfElves) {
        int skip = 2;
        int curElf = 1;
        int powerCounter = 2;

        while (numOfElves > 1) {
            if (numOfElves % 2 == 1) {
                curElf += skip;
            }
            skip = std::pow(2, powerCounter++);
            numOfElves /= 2;
        }
        return curElf;
    }

    int day19_2(int numOfElves) {

        /*
        Couldn't quite figure out how to do this in a nice way for hours :-) ...
        In the end looked up the general gist of the solution ... trivial afterwards.
        */
        std::queue<int> takers = std::ranges::views::iota(1, numOfElves/2+1) | std::ranges::to<std::queue>();
        std::queue<int> losers = std::ranges::views::iota(numOfElves/2+1, numOfElves+1) | std::ranges::to<std::queue>();

        while (takers.size() > 0) {
            losers.pop();
            losers.emplace(takers.front());

            if (takers.size() != losers.size()) {
                takers.emplace(losers.front());
                losers.pop();
            }
            takers.pop();
        }
        return losers.front();
    }
}
