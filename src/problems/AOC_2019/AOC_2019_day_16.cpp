#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>


namespace AOC2019 {
long long day16_1(std::string dataFile, size_t num_phases) {


    std::vector<short> basePattern = {0, 1, 0, -1};

    auto               d_ctre = ctre::search<R"(\d+)">;
    auto               input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front();
    std::vector<short> data;
    for (auto &chr : input) { data.push_back(chr - 48); }

    incom::commons::doubleBuffer<std::vector<short>> buf(data);
    buf.getNext().clear();

    int const bpOrigSize   = basePattern.size();
    int const dataOrigSize = data.size();
    while (basePattern.size() < data.size()) {
        for (int i = 0; i < bpOrigSize && basePattern.size() < data.size(); ++i) {
            basePattern.push_back(basePattern.at(i));
        }
    }

    auto computeNewList = [&]() -> void {
        for (int line = 0; line < dataOrigSize; ++line) {
            int   inpCursor = 0;
            short accu      = 0;
            for (int repFirst = 0; repFirst < line && inpCursor < dataOrigSize; ++repFirst) {
                accu += buf.getCurrent().at(inpCursor++) * basePattern.at(0);
            }
            for (int basePatChar = 1; basePatChar < dataOrigSize; ++basePatChar) {
                for (int rep = 0; rep < (line + 1) && inpCursor < dataOrigSize; ++rep) {
                    accu += buf.getCurrent().at(inpCursor++) * basePattern.at(basePatChar);
                }
            }
            buf.getNext().push_back(std::abs(std::move(accu)) % 10);
        }
        buf.swapBuffers();
        buf.getNext().clear();
    };

    for (int i = 0; i < num_phases; ++i) { computeNewList(); }

    std::string strAns = "";
    for (auto const &chrShort : buf.getCurrent()) { strAns += std::to_string(chrShort); }
    strAns = strAns.substr(0, 8);

    return std::stoll(strAns);
}

long long day16_2(std::string dataFile, size_t num_phases) {
    std::vector<short> basePattern = {0, 1, 0, -1};

    auto               d_ctre = ctre::search<R"(\d+)">;
    auto               input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front();
    std::vector<short> data;

    size_t const offset = std::stoul(input.substr(0, 7));
    for (auto &chr : input) { data.push_back(chr - 48); }

    auto inp = std::views::repeat(data, 10000) | std::views::join | std::ranges::to<std::vector>();

    // Didn't quite realize that one doesn't need to compute the whole 6.5M data points :-(
    // However, got like 70% there with the ideas that:
    // A) the 2nd half od the digits is computed simply by summing up all the digits from the previous phase from the
    // position upwards.
    // B) One can greatly simplify by the realization that each digit 'n' in the 2nd half depends only
    // on the digit 'n+1' from the same phase and the 'n' digit from the previous phase. Thereby a solution
    // in the ballpark of 'dynamic programming' emerges.
    for (int phs = 0; phs < num_phases; ++phs) {
        for (size_t i = inp.size() - 2; i >= offset; --i) { inp[i] = (inp[i + 1] + inp[i]) % 10; };
    }

    long long mult = 1;
    long long ans  = 0;
    for (size_t id = offset + 7; id >= offset; --id) {
        ans  += inp[id] * mult;
        mult *= 10;
    }
    return ans;
}
} // namespace AOC2019