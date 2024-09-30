#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>


namespace AOC2018 {
long long day23_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::array<long long, 4>> data;
    for (auto &inpLine : input) {
        data.push_back(
            {std::stoll(inpLine[0]), std::stoll(inpLine[1]), std::stoll(inpLine[2]), std::stoll(inpLine[3])});
    }

    std::array<long long, 4> strongest =
        flux::ref(data).max([](auto &&a, auto &&b) { return a.back() < b.back(); }).value();

    long long res = flux::ref(data).count_if([&](auto &&a) {
        long long accu = 0;
        for (int i = 0; i < 3; ++i) { accu += std::abs(strongest[i] - a[i]); }
        return accu <= strongest[3];
    });

    return res;
}

long long day23_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::array<long long, 4>> data;
    for (auto &inpLine : input) {
        data.push_back(
            {std::stoll(inpLine[0]), std::stoll(inpLine[1]), std::stoll(inpLine[2]), std::stoll(inpLine[3])});
    }

    std::vector<long long> countInRngOfOther(data.size(), 0);


    flux::ref(data).for_each([&](auto &&a) {
        for (int i = 0; auto &oneBot : data) {
            long long accu = 0;
            for (int j = 0; j < 3; ++j) { accu += std::abs(oneBot[j] - a[j]); }
            if (accu < a[3]) { countInRngOfOther[i]++; }
            i++;
        }
    });

    long long mostCoveredBotID =
        std::max_element(countInRngOfOther.begin(), countInRngOfOther.end()) - countInRngOfOther.begin();


    auto calcInRng = [&](std::array<long long, 4> &refToPoint) -> long long {
        long long count = 0;
        for (int i = 0; auto &oneBot : data) {
            long long accu = 0;
            for (int j = 0; j < 3; ++j) { accu += std::abs(oneBot[j] - refToPoint[j]); }
            if (accu < oneBot[3]) { count++; }
            i++;
        }
        return count;
    };


    auto findInCircles = [&]() {
        long long  curMaxCoverage = countInRngOfOther[mostCoveredBotID];
        auto const origLoc        = data[mostCoveredBotID];
        auto       trialLoc       = origLoc;
        int        cycle          = 0;

        std::vector<decltype(trialLoc)> possibilities{trialLoc};

        long long cont         = 0;
        auto      insideLambda = [&]() {
            long long tempCov = calcInRng(trialLoc);
            if (tempCov > curMaxCoverage) {
                curMaxCoverage = tempCov;
                possibilities.clear();
                possibilities.push_back(trialLoc);
                cont++;
            }
            if (tempCov == curMaxCoverage) {
                possibilities.push_back(trialLoc);
                cont++;
            }
        };

        do {
            cont = 0;
            cycle++;

            for (int colOffset = (-cycle + 1); colOffset <= (cycle); ++colOffset) {
                for (int rowOffset = (-cycle); rowOffset <= (cycle); ++rowOffset) {
                    trialLoc = {origLoc[0] - cycle, origLoc[1] + colOffset, origLoc[2] + rowOffset, 0};
                    insideLambda();

                    trialLoc = {origLoc[0] + cycle, origLoc[1] - colOffset, origLoc[2] + rowOffset, 0};
                    insideLambda();
                }
            }
            for (int colOffset = (-cycle + 1); colOffset <= (cycle); ++colOffset) {
                for (int rowOffset = (-cycle); rowOffset <= (cycle); ++rowOffset) {
                    trialLoc = {origLoc[0] - colOffset, origLoc[1] - cycle, origLoc[2] + rowOffset, 0};
                    insideLambda();

                    trialLoc = {origLoc[0] + colOffset, origLoc[1] + cycle, origLoc[2] + rowOffset, 0};
                    insideLambda();
                }
            }
            for (int colOffset = (-cycle + 1); colOffset <= (cycle - 1); ++colOffset) {
                for (int rowOffset = (-cycle + 1); rowOffset <= (cycle - 1); ++rowOffset) {
                    trialLoc = {origLoc[0] + colOffset, origLoc[1] + rowOffset, origLoc[2] - cycle, 0};
                    insideLambda();
                    trialLoc = {origLoc[0] + colOffset, origLoc[1] + rowOffset, origLoc[2] + cycle, 0};
                    insideLambda();
                }
            }


        } while (cont != 0);

        return possibilities;
    };

    auto aaa = findInCircles();

    return -999;
}
} // namespace AOC2018