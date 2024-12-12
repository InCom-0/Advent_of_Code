#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/format.h>
#include <incom_commons.h>
#include <ranges>


namespace AOC2024 {
namespace incc = incom::commons;

long long day5_1(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    auto firstPO = std::ranges::find_if(input, [](auto const &line) { return line.size() > 10; });
    std::vector<std::string> const rules(input.begin(), firstPO);
    std::vector<std::string> const PO(firstPO, input.end());

    auto rulesParsed = std::views::transform(rules, [](auto const &a) {
        auto it = (std::views::split(a, "|"sv) | std::views::transform([](auto const &b) {
                       return b | std::ranges::to<std::string>();
                   })).begin();
        return std::array<int, 2>{std::stoi(*(it++)), std::stoi(*it)};
    });

    auto POparsed = std::views::transform(PO, [](auto const &a) {
        auto splt = std::views::split(a, ","sv) |
                    std::views::transform([](auto const &b) { return std::stoi(b | std::ranges::to<std::string>()); });
        return std::vector<int>(splt.begin(), splt.end());
    });

    ankerl::unordered_dense::set<int, incc::XXH3Hasher>                   inCur;
    ankerl::unordered_dense::map<int, std::vector<int>, incc::XXH3Hasher> mustBeAfterKey;

    for (auto const &rule : rulesParsed) {
        mustBeAfterKey.insert({rule.front(), std::vector<int>()});
        mustBeAfterKey.at(rule.front()).push_back(rule.back());
    }

    auto POfiltered = std::views::filter(POparsed, [&](auto const &a) {
        inCur.clear();
        for (auto const &oneInt : a) {
            if (mustBeAfterKey.contains(oneInt)) {
                for (auto const &oneMust : mustBeAfterKey.at(oneInt)) {
                    if (inCur.contains(oneMust)) { return false; }
                }
            }
            inCur.insert(oneInt);
        }
        return true;
    });
    return std::ranges::fold_left(POfiltered, 0ll, [](auto &&ini, auto const &a) { return ini + (a[a.size() / 2]); });
}

long long day5_2(std::string dataFile) {
    using namespace std::literals;
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    auto firstPO = std::ranges::find_if(input, [](auto const &line) { return line.size() > 10; });
    std::vector<std::string> const rules(input.begin(), firstPO);
    std::vector<std::string> const PO(firstPO, input.end());

    auto rulesParsed = std::views::transform(rules, [](auto const &a) {
        auto it = (std::views::split(a, "|"sv) | std::views::transform([](auto const &b) {
                       return b | std::ranges::to<std::string>();
                   })).begin();
        return std::array<int, 2>{std::stoi(*(it++)), std::stoi(*it)};
    });

    auto POparsed = std::views::transform(PO, [](auto const &a) {
        auto splt = std::views::split(a, ","sv) |
                    std::views::transform([](auto const &b) { return std::stoi(b | std::ranges::to<std::string>()); });
        return std::vector<int>(splt.begin(), splt.end());
    });

    ankerl::unordered_dense::set<int, incc::XXH3Hasher>                   inCur;
    ankerl::unordered_dense::map<int, std::vector<int>, incc::XXH3Hasher> mustBeAfterKey;

    for (auto const &rule : rulesParsed) {
        mustBeAfterKey.insert({rule.front(), std::vector<int>()});
        mustBeAfterKey.at(rule.front()).push_back(rule.back());
    }

    auto POfiltered = std::views::filter(POparsed, [&](auto const &a) {
        inCur.clear();
        for (auto const &oneInt : a) {
            if (mustBeAfterKey.contains(oneInt)) {
                for (auto const &oneMust : mustBeAfterKey.at(oneInt)) {
                    if (inCur.contains(oneMust)) { return true; }
                }
            }
            inCur.insert(oneInt);
        }
        return false;
    });


    auto POcorrected = std::views::transform(POfiltered, [&](auto const &po) {
        std::vector<int> res = po;

        std::ranges::sort(res, [&](auto &a, auto &b) {
            if (not mustBeAfterKey.contains(a)) { }
            else {
                auto vecOfMustBeAfterA = mustBeAfterKey.at(a);
                if(std::ranges::find(vecOfMustBeAfterA, b) != vecOfMustBeAfterA.end()) {return true;}
            }

            if (not mustBeAfterKey.contains(b)) { }
            else {
                auto vecOfMustBeAfterB = mustBeAfterKey.at(b);
                if(std::ranges::find(vecOfMustBeAfterB, a) != vecOfMustBeAfterB.end()) {return false;}
            }
            return true;
        });
        return res;
    });

    return std::ranges::fold_left(POcorrected, 0ll, [](auto &&ini, auto const &a) { return ini + (a[a.size() / 2]); });
}
} // namespace AOC2024