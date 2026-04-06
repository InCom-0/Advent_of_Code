#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <string>
#include <utility>


namespace AOC2025 {
size_t
day5_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre, d_ctre);

    std::vector<std::pair<size_t, size_t>> rngs;
    for (auto const &[a, b] : std::views::zip(input.at(0), input.at(1))) {
        rngs.push_back(std::make_pair(std::stoll(a), std::stoll(b)));
    }

    size_t avaFresh = 0uz;
    for (auto const &freeID : std::views::drop(input.at(0), input.at(1).size())) {
        for (auto const [start, end] : rngs) {
            size_t curID = std::stoll(freeID);
            if (curID >= start) {
                if (curID <= end) {
                    avaFresh++;
                    break;
                }
            }
        }
    }

    return avaFresh;
}
size_t
day5_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre, d_ctre);


    std::vector<std::optional<std::pair<size_t, size_t>>> rngs;
    for (auto const &[a, b] : std::views::zip(input.at(0), input.at(1))) {
        rngs.push_back(std::make_pair(static_cast<size_t>(std::stoll(a)), static_cast<size_t>(std::stoll(b))));
    }

    auto evalOverlap = [](std::optional<std::pair<size_t, size_t>> &toAssess, std::pair<size_t, size_t> &overlapWith) {
        size_t res         = 0uz;
        auto  &toAssessVal = toAssess.value();
        if (toAssessVal.first > overlapWith.second || toAssessVal.second < overlapWith.first) {}

        // overlap from below
        else if (toAssessVal.first < overlapWith.first) {
            if (toAssessVal.second > overlapWith.second) {
                res                = toAssessVal.second - overlapWith.second;
                toAssessVal.second = overlapWith.first - 1;
                overlapWith.second = toAssessVal.second;
            }
            else { toAssessVal.second = overlapWith.first - 1; }
        }

        // overlap from above
        else {
            if (toAssessVal.second > overlapWith.second) { toAssessVal.first = overlapWith.second + 1; }
            else { toAssess = std::nullopt; }
        }
        return res;
    };

    size_t res = 0uz;

    for (size_t id = 0; auto &toAssess : rngs) {
        auto toAssessCpy = toAssess;
        for (auto &ol_with : std::views::take(rngs, id)) {
            if (not toAssessCpy.has_value()) { break; }
            else if (ol_with.has_value()) { res += evalOverlap(toAssessCpy, ol_with.value()); }
            else { continue; }
        }
        if (toAssessCpy.has_value()) { res += toAssessCpy.value().second - toAssessCpy.value().first + 1; }
        toAssess = toAssessCpy;
        id++;
    }

    return res;
}
} // namespace AOC2025