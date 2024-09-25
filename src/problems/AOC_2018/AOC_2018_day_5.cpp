#include <incom_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <stack>


namespace AOC2018 {
int day5_1(std::string dataFile) {
    auto all_ctre = ctre::search<R"(.+)">;
    auto line =
        std::move(incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, all_ctre).front().front());

    auto toBeDeleted = [](char &first, char &second) -> bool {
        if ((first - 32) == second) { return true; }
        if ((second - 32) == first) { return true; }
        return false;
    };

    bit::bit_vector<unsigned char> bv(line.size(), bit::bit1);
    std::stack<int, std::vector<int>> stck;
    for (int i = 0; i < line.size(); ++i) {
        if (not stck.empty() && toBeDeleted(line[stck.top()], line[i])) {
            bv[stck.top()] = bit::bit0;
            bv[i]          = bit::bit0;
            stck.pop();
        }
        else { stck.push(i); }
    }
    return std::ranges::count_if(bv, [](auto &&a) { return a == bit::bit1; });
}

int day5_2(std::string dataFile) {
    auto all_ctre = ctre::search<R"(.+)">;
    auto line =
        std::move(incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, all_ctre).front().front());

    auto toBeDeleted = [](char &first, char &second) -> bool {
        if ((first - 32) == second) { return true; }
        if ((second - 32) == first) { return true; }
        return false;
    };

    int  shortestPoly = INT_MAX;
    char chr1         = 'A';
    char chr2         = 'a';

    chr1--;
    chr2--;

    for (int j = 0; j < 26; ++j) {
        std::stack<int> stck;
        chr1++;
        chr2++;
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] == chr1 || line[i] == chr2) { continue; }
            else if (not stck.empty() && toBeDeleted(line[stck.top()], line[i])) { stck.pop(); }
            else { stck.push(i); }
        }
        shortestPoly = std::min(shortestPoly, (int)stck.size());
    }
    return shortestPoly;
}
} // namespace AOC2018