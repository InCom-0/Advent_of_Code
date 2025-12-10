#include <incom_commons.h>
#include <algorithm>
#include <ctre.hpp>
#include <flux.hpp>
#include <list>
#include <string>


namespace AOC2018 {
unsigned long long day9_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofS   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front();

    unsigned long players    = std::stoul(VofS.front());
    unsigned long lastMarble = std::stoul(VofS.back());

    std::vector<unsigned long long> playerRewards(players, 0);
    std::list<unsigned long long> lst {0};

    unsigned long curPlayer = 0;
    auto          curIter   = lst.begin();

    auto advanceCircular = [&](auto &iter) {
        if (iter == lst.end()) { iter = std::next(lst.begin()); }
        else { std::advance(iter, 1); }
    };
    auto retreatCircular = [&](auto &iter) {
        if (iter == lst.begin()) { iter = std::prev(lst.end()); }
        else { std::advance(iter, -1); }
    };
    auto awardPlayer = [&](unsigned long &curMarble) {
        playerRewards[curPlayer] += curMarble;

        for (int i = 0; i < 8; ++i) { retreatCircular(curIter); }
        auto sevenBackIter = curIter;
        for (int i = 0; i < 2; ++i) { advanceCircular(curIter); }

        playerRewards[curPlayer] += sevenBackIter.operator*();
        lst.erase(sevenBackIter);
    };

    for (unsigned long marble = 1; marble <= lastMarble; ++marble) {
        if (marble % 23 == 0) { awardPlayer(marble); }
        else {
            advanceCircular(curIter);
            lst.insert(curIter, marble);
        }
        if (++curPlayer == players) { curPlayer = 0; }
    }
    return std::ranges::max_element(playerRewards, std::less()).operator*();
}
} // namespace AOC2018