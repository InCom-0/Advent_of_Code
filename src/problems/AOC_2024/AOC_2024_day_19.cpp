#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string_view>


namespace AOC2024 {
namespace incc = incom::commons;
long long day19_1(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    std::vector<std::string> basePatterns;
    std::vector<std::string> desired;

    for (auto const &desiredOne : input.front()) { basePatterns.push_back(desiredOne); }
    for (auto item = input.begin() + 2; item != input.end(); ++item) { desired.push_back((*item).front()); }

    ankerl::unordered_dense::set<std::string_view, incc::XXH3Hasher> bp_set;
    size_t                                                           maxLength = 0;
    for (auto const &bp : basePatterns) {
        bp_set.insert(std::string_view(bp));
        maxLength = std::max(maxLength, bp.size());
    }

    long long counter = 0;
    for (auto const &desi : desired) {
        // Compute 'allowed steps' ... quadratic complexity (but in this case limited by the maxLength of available
        // patterns)
        std::vector allowedSteps_dp(desi.size(), std::vector(desi.size(), 0));
        for (int strt = 0; strt < desi.size(); ++strt) {
            for (int cnt = 1; cnt < desi.size() - strt + 1 && cnt <= maxLength; ++cnt) {
                if (bp_set.contains(std::string_view(&desi[strt], cnt))) { allowedSteps_dp[strt][strt + cnt - 1] = 1; }
            }
        }

        auto dp     = std::vector(desi.size() + 1, 0ll);
        dp.front()  = 1;
        auto solver = [&]() -> void {
            for (int fromID = 0; fromID < desi.size(); ++fromID) {
                if (dp[fromID] == 0) { continue; } // Skip unreachable
                for (int toID = fromID; toID < desi.size(); ++toID) {
                    if (allowedSteps_dp[fromID][toID] == 1) { dp[toID + 1] = 1; }
                }
                // if (dp.back() == 1) { break; }
            }
        };
        solver();
        if (dp.back() == 1) { counter++; }
    }
    return counter;
}

long long day19_2(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    std::vector<std::string> basePatterns;
    std::vector<std::string> desired;

    for (auto const &desiredOne : input.front()) { basePatterns.push_back(desiredOne); }
    for (auto item = input.begin() + 2; item != input.end(); ++item) { desired.push_back((*item).front()); }

    ankerl::unordered_dense::set<std::string_view, incc::XXH3Hasher> bp_set;
    size_t                                                           maxLength = 0;
    for (auto const &bp : basePatterns) {
        bp_set.insert(std::string_view(bp));
        maxLength = std::max(maxLength, bp.size());
    }

    long long counter = 0;
    for (auto const &desi : desired) {
        // Compute 'allowed steps' ... quadratic complexity (but in this case limited by the maxLength of available
        // patterns)
        std::vector allowedSteps_dp(desi.size(), std::vector(desi.size(), 0));
        for (int strt = 0; strt < desi.size(); ++strt) {
            for (int cnt = 1; cnt < desi.size() - strt + 1 && cnt <= maxLength; ++cnt) {
                if (bp_set.contains(std::string_view(&desi[strt], cnt))) { allowedSteps_dp[strt][strt + cnt - 1] = 1; }
            }
        }

        auto dp     = std::vector(desi.size() + 1, 0ll);
        dp.front()  = 1;
        auto solver = [&]() -> void {
            for (int fromID = 0; fromID < desi.size(); ++fromID) {
                if (dp[fromID] == 0) { continue; } // Skip unreachable
                for (int toID = fromID; toID <  desi.size(); ++toID) {
                    // The # of possibilities from 'fromID' added to 'toID' if direct route from the latter to the
                    // former.
                    if (allowedSteps_dp[fromID][toID] == 1) { dp[toID + 1] += dp[fromID]; }
                }
                // if (dp.back() == 1) { break; }
            }
        };
        solver();
        counter += dp.back();
    }
    return counter;
}
} // namespace AOC2024