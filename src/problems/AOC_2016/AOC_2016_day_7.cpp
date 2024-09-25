#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

#include "AOC_2016_day_7.h"


namespace AOC2016 {

int day7_1(std::string &&dataFile) {
    auto inp = incom::commons::parseInputUsingCTRE::processFile(dataFile, ctre::search<R"(.+)">);

    std::vector<std::vector<std::string>> found1;

    auto ABBActre          = ctre::search<R"((\w)(?!\g{1})(\w)\g{2}\g{1})">;
    auto ABBAinBracketctre = ctre::search<R"(\[[^\]]*(\w)(\w)\g{2}\g{1}[^\[]*\])">;

    for (auto &line : inp[0]) {
        found1.push_back(incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(line, ABBActre));
    }

    auto ans = flux::zip(flux::ref(inp[0]), flux::ref(found1))
                   .filter([](auto &&a) { return a.second.size() > 0; })
                   .filter([&](auto &&a) { return not ABBAinBracketctre(a.first.begin(), a.first.end()); })
                   .map([](auto &&a) { return std::pair<std::string, std::vector<std::string>>(a.first, a.second); })
                   .count();
    return ans;
}
int day7_2(std::string &&dataFile) {
    auto inp = incom::commons::parseInputUsingCTRE::processFile(dataFile, ctre::search<R"(.+)">)[0];
    for (auto &str : inp) { str.push_back('['); }

    auto ABA_BAB_ctre_2    = ctre::search<R"((\w)(?!\g{1})(\w)\g{1}[^\[]*\].*\g{2}\g{1}\g{2}[^\]]*\[)">;
    auto ABA_BAB_ctreREV_2 = ctre::search<R"((\w)(?!\g{1})(\w)\g{1}[^\]]*\[.*\g{2}\g{1}\g{2}[^\[]*\])">;

    auto ans = flux::ref(inp)
                   .filter([&](auto &&a) {
                       return ABA_BAB_ctre_2(a.begin(), a.end()) or ABA_BAB_ctreREV_2(a.begin(), a.end());
                   })
                   .to<std::vector<std::string>>();
    return ans.size();
}

} // namespace AOC2016