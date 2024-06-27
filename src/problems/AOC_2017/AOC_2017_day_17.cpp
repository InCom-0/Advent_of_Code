#include <list>

#include <AOC_commons.h>
#include <ctre.hpp>

#include <flux.hpp>

namespace AOC2017 {
int day17_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto Vinp   = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre);

    int steps = std::stoi(Vinp.front().front());

    std::list<int> lst;
    lst.push_back(0);

    auto iter   = lst.begin();
    int  stepBy = 0;

    for (int i = 1; i < 2018; ++i) {
        stepBy = ((steps % i) + (std::distance(lst.begin(), iter))) % lst.size();

        iter = lst.begin();
        std::advance(iter, stepBy + 1);
        lst.insert(iter, i);
        std::advance(iter, -1);
    }
    std::advance(iter, 1);
    return iter.operator*();
}

int day17_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto Vinp   = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre);

    unsigned int steps = std::stoi(Vinp.front().front());
    unsigned int pos   = 0;
    unsigned int len   = 1;
    unsigned int ans   = UINT_MAX;

    for (int i = 1; i < 50'000'001; ++i) {
        pos = (pos + steps) % len++;
        if (not pos++) { ans = i; }
    }
    return ans;
}

} // namespace AOC2017