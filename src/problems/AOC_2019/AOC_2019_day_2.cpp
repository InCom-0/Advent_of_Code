#include <cassert>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>


namespace AOC2019 {
long long day2_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(-?\d+)">;

    auto input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<long long> dataVect;
    for (auto const &strNumber : input.front()) { dataVect.push_back(std::stoll(strNumber)); }

    size_t id = 0;

    auto executeOneInstruction = [&]() -> void {
        if (dataVect[id] == 99) { id = (ULLONG_MAX - 4); }
        else if (dataVect[id] == 1) {
            dataVect[dataVect[id + 3]] = dataVect[dataVect[id + 1]] + dataVect[dataVect[id + 2]];
        }
        else if (dataVect[id] == 2) {
            dataVect[dataVect[id + 3]] = dataVect[dataVect[id + 1]] * dataVect[dataVect[id + 2]];
        }
        else { assert(false); }
    };

    dataVect[1] = 12;
    dataVect[2] = 2;

    while (id < dataVect.size()) {
        executeOneInstruction();
        id += 4;
    }

    return dataVect[0];
}

long long day2_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(-?\d+)">;

    auto input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<long long> dataVect;
    for (auto const &strNumber : input.front()) { dataVect.push_back(std::stoll(strNumber)); }

    size_t id = 0;

    auto executeOneInstruction = [&]() -> void {
        if (dataVect[id] == 99) { id = (ULLONG_MAX - 4); }
        else if (dataVect[id] == 1) {
            dataVect[dataVect[id + 3]] = dataVect[dataVect[id + 1]] + dataVect[dataVect[id + 2]];
        }
        else if (dataVect[id] == 2) {
            dataVect[dataVect[id + 3]] = dataVect[dataVect[id + 1]] * dataVect[dataVect[id + 2]];
        }
        else { assert(false); }
    };
    std::vector<long long> dataVect_BU = dataVect;
    long long              inpChange_a;
    long long              inpChange_b;

    for (inpChange_a = 0; inpChange_a < 100; ++inpChange_a) {
        for (inpChange_b = 0; inpChange_b < 100; ++inpChange_b) {
            dataVect    = dataVect_BU;
            dataVect[1] = inpChange_a;
            dataVect[2] = inpChange_b;
            id          = 0;

            while (id < dataVect.size()) {
                executeOneInstruction();
                id += 4;
            }
            if (dataVect[0] == 19690720) { break; }
        }
        if (dataVect[0] == 19690720) { break; }
    }


    return ((100 * inpChange_a) + inpChange_b);
}
} // namespace AOC2019