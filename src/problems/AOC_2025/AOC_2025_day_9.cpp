#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>
#include <ranges>
#include <utility>


namespace AOC2025 {

size_t
day9_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::array<long long, 2>> data{};
    for (size_t id = 0; auto const &line : input) { data.push_back({std::stoll(line.at(0)), std::stoll(line.at(1))}); }

    long long res = 0ll;
    for (auto const &[pA, pB] : incstd::views::combinations_k<2>(data)) {
        res = std::max(res, (pA[0] - pB[0] + 1) * (pA[1] - pB[1] + 1));
    }

    return static_cast<size_t>(res);
}


size_t
day9_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::pair<long long, std::array<long long, 2>>> data{};
    for (long long id = 0; auto const &line : input) {
        data.push_back({id++, {std::stoll(line.at(0)), std::stoll(line.at(1))}});
    }

    std::vector<std::array<long long, 3>> rects{};
    for (auto const &[pA, pB] : incstd::views::combinations_k<2>(data)) {
        rects.push_back({pA.first, pB.first,
                         (std::abs(pA.second[0] - pB.second[0]) + 1) * (std::abs(pA.second[1] - pB.second[1]) + 1)});
    }

    data.push_back(data.front());
    std::ranges::sort(rects, [](auto &itemA, auto &itemB) { return itemA[2] > itemB[2]; });

    auto evaluateRect = [&](std::array<long long, 3> const &rect) -> std::optional<long long> {
        long long const minX = std::min(data[rect[0]].second[0], data[rect[1]].second[0]);
        long long const maxX = std::max(data[rect[0]].second[0], data[rect[1]].second[0]);
        long long const minY = std::min(data[rect[0]].second[1], data[rect[1]].second[1]);
        long long const maxY = std::max(data[rect[0]].second[1], data[rect[1]].second[1]);

        bool switchedArrangement = (data[rect[0]].second[0] == minX) xor (data[rect[0]].second[1] == minY);

        auto is_segmentInRect = [&](std::array<long long, 2> const &segPointA,
                                   std::array<long long, 2> const &segPointB) -> bool {
            if (segPointA[0] == segPointB[0]) {
                if (segPointA[0] > minX && segPointA[0] < maxX) {
                    if (segPointA[1] > minY && segPointA[1] < maxY) { return true; }
                    else if (segPointB[1] > minY && segPointB[1] < maxY) { return true; }
                    else if (((segPointA[1] - minY) * (segPointB[1] - maxY)) < 0) { return true; }
                }
                return false;
            }
            else if (segPointA[1] == segPointB[1]) {
                if (segPointA[1] > minY && segPointA[1] < maxY) {
                    if (segPointA[0] > minX && segPointA[0] < maxX) { return true; }
                    else if (segPointB[0] > minX && segPointB[0] < maxX) { return true; }
                    else if (((segPointA[0] - minX) * (segPointB[0] - maxX)) < 0) { return true; }
                }
                return false;
            }
            else { assert(false); }
            std::unreachable();
        };

        auto is_otherCornersCanBeInside = [&]() -> bool {
            auto exec = [&](long long firstY, long long secondY) -> bool {
                size_t goodA_id = 0uz;
                size_t goodB_id = 0uz;
                bool   res      = false;

                for (auto const &point : data) {
                    if (point.second[0] >= maxX && point.second[1] >= firstY) {
                        goodA_id = point.first;
                        break;
                    }
                }
                if (goodA_id) {
                    for (auto const &point : data) {
                        if (point.second[0] <= minX && point.second[1] <= secondY) {
                            goodB_id = point.first;
                            break;
                        }
                    }
                    if (goodA_id && goodB_id) {
                        bool l = (goodA_id < rect[0] || goodA_id > rect[1]);
                        bool r = (goodB_id < rect[0] || goodB_id > rect[1]);
                        res    = l xor r;
                    }
                }
                return res;
            };

            if (switchedArrangement) { return exec(maxY, minY); }
            else { return exec(minY, maxY); }
        };

        if (is_otherCornersCanBeInside()) {
            for (auto const &[a, b] : std::views::pairwise(data)) {
                if (is_segmentInRect(a.second, b.second)) { return std::nullopt; }
            }
            return rect[2];
        }
        else { return std::nullopt; }
    };

    for (auto const &oneDist : rects) {
        if (auto maybeRes = evaluateRect(oneDist)) { return maybeRes.value(); }
    }

    return 0uz;
}
} // namespace AOC2025