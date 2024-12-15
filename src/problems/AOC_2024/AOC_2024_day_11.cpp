#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>


namespace AOC2024 {
namespace incc = incom::commons;
long long day11_0(std::string dataFile, int const repeats) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(\d+)">;
    auto input    = incc::parseInputUsingCTRE::processFileRPT(dataFile, any_ctre).front();

    std::vector<size_t> data;
    for (auto const &number : input) { data.push_back(std::stoull(number)); }

    ankerl::unordered_dense::map<std::pair<size_t, int>, size_t, incc::XXH3Hasher> pastResults;


    auto rec_solve = [&](this auto const &self, size_t const numOnStone, int const blinkRemaining) {
        size_t res;
        size_t hlpr;
        auto   ite = pastResults.find({numOnStone, blinkRemaining});

        if (ite != pastResults.end()) { return ite->second; }
        else if (blinkRemaining == 0) { return 1uz; }
        else if (numOnStone == 0) { res = self(1, blinkRemaining - 1); }
        else if ((hlpr = incc::numeric::get_numOfDigits(numOnStone)) % 2 == 0) {
            res = self(numOnStone / static_cast<size_t>(std::pow(10, hlpr / 2)), blinkRemaining - 1) +
                  self(numOnStone % static_cast<size_t>(std::pow(10, hlpr / 2)), blinkRemaining - 1);
        }
        else { res = self(numOnStone * 2024, blinkRemaining - 1); }
        pastResults.insert({{numOnStone, blinkRemaining}, res});
        return res;
    };

    size_t accu = 0;
    for (auto const &inputNum : data) { accu += rec_solve(inputNum, repeats); }

    return accu;
}

long long day11_1(std::string dataFile) {
    return day11_0(dataFile, 25);
}

long long day11_2(std::string dataFile) {
    return day11_0(dataFile, 75);
}
} // namespace AOC2024