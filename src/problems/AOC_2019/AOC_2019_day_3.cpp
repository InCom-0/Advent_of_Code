#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2019 {

long long day3_1(std::string dataFile) {
    // TYPE DEFINITIONS
    struct Point {
        long long x;
        long long y;
        long long calc_distance(Point &otherPoint) { return (std::abs(x - otherPoint.x) + std::abs(y - otherPoint.y)); }
    };

    struct WireLine {
        Point start;
        Point end;
    };

    auto calc_distance = [](Point const &firstPoint, Point const &secondPoint) -> long long {
        return (std::abs(firstPoint.x - secondPoint.x) + std::abs(firstPoint.y - secondPoint.y));
    };

    auto calc_intersection = [&](WireLine const &firstWire, WireLine const &secondWire,
                                 std::vector<Point> &candidates) {
        auto is_TD = [&](WireLine const &wire) { return wire.start.x == wire.end.x; };

        if (is_TD(firstWire) && not is_TD(secondWire)) {
            if ((std::abs(firstWire.start.x - secondWire.start.x) + std::abs(firstWire.start.x - secondWire.end.x)) ==
                std::abs(secondWire.end.x - secondWire.start.x)) {
                if ((std::abs(secondWire.start.y - firstWire.start.y) +
                     std::abs(secondWire.start.y - firstWire.end.y)) == std::abs(firstWire.end.y - firstWire.start.y)) {
                    candidates.push_back(Point{firstWire.start.x, secondWire.start.y});
                }
            }
        }
        else if (not is_TD(firstWire) && is_TD(secondWire)) {

            if ((std::abs(firstWire.start.y - secondWire.start.y) + std::abs(firstWire.start.y - secondWire.end.y)) ==
                std::abs(secondWire.end.y - secondWire.start.y)) {
                if ((std::abs(secondWire.start.x - firstWire.start.x) +
                     std::abs(secondWire.start.x - firstWire.end.x)) == std::abs(firstWire.end.x - firstWire.start.x)) {
                    candidates.push_back(Point{secondWire.start.x, firstWire.start.y});
                }
            }
        }
    };

    // DATA PREP
    auto ctreNotComma = ctre::search<R"([^,]+)">;
    auto input        = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, ctreNotComma);
    std::vector<std::vector<WireLine>> vOfV_wireLines;

    for (auto &oneWireInput : input) {
        Point curPoint{0, 0};
        vOfV_wireLines.push_back(std::vector<WireLine>());
        for (auto &wireInstr : oneWireInput) {
            Point pt_change{0, 0};
            if (wireInstr.front() == 'U') {
                pt_change.y += std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else if (wireInstr.front() == 'R') {
                pt_change.x += std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else if (wireInstr.front() == 'D') {
                pt_change.y -= std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else if (wireInstr.front() == 'L') {
                pt_change.x -= std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else { assert(false); }

            vOfV_wireLines.back().push_back({curPoint, {curPoint.x + pt_change.x, curPoint.y + pt_change.y}});
            curPoint = vOfV_wireLines.back().back().end;
        }
    }

    std::vector<Point> candidates;

    // MAIN LOGIC
    for (auto &wire : vOfV_wireLines) {
        for (auto &otherWire : vOfV_wireLines) {
            if (&otherWire == &wire) { continue; }
            for (auto &wireLine_a : wire) {
                for (auto &wireLine_b : otherWire) { calc_intersection(wireLine_a, wireLine_b, candidates); }
            }
        }
    }

    auto ans = flux::min(candidates, [&](auto &&a, auto &&b) {
        return (std::abs(a.x) + std::abs(a.y) < (std::abs(b.x) + std::abs(b.y)));
    });

    return (std::abs(ans.value().x) + std::abs(ans.value().y));
}


long long day3_2(std::string dataFile) {
    // TYPE DEFINITIONS
    struct Point {
        long long x;
        long long y;
    };

    struct WireLine {
        Point start;
        Point end;
    };

    auto calc_distance = [](Point const &firstPoint, Point const &secondPoint) -> long long {
        return (std::abs(firstPoint.x - secondPoint.x) + std::abs(firstPoint.y - secondPoint.y));
    };

    auto calc_intersection = [&](WireLine const &firstWire, WireLine const &secondWire,
                                 std::vector<Point> &candidates) {
        auto is_TD = [&](WireLine const &wire) { return wire.start.x == wire.end.x; };

        if (is_TD(firstWire) && not is_TD(secondWire)) {
            if ((std::abs(firstWire.start.x - secondWire.start.x) + std::abs(firstWire.start.x - secondWire.end.x)) ==
                std::abs(secondWire.end.x - secondWire.start.x)) {
                if ((std::abs(secondWire.start.y - firstWire.start.y) +
                     std::abs(secondWire.start.y - firstWire.end.y)) == std::abs(firstWire.end.y - firstWire.start.y)) {
                    candidates.push_back(Point{firstWire.start.x, secondWire.start.y});
                }
            }
        }
        else if (not is_TD(firstWire) && is_TD(secondWire)) {

            if ((std::abs(firstWire.start.y - secondWire.start.y) + std::abs(firstWire.start.y - secondWire.end.y)) ==
                std::abs(secondWire.end.y - secondWire.start.y)) {
                if ((std::abs(secondWire.start.x - firstWire.start.x) +
                     std::abs(secondWire.start.x - firstWire.end.x)) == std::abs(firstWire.end.x - firstWire.start.x)) {
                    candidates.push_back(Point{secondWire.start.x, firstWire.start.y});
                }
            }
        }
    };

    auto is_pointOnWireline = [](WireLine const &wl, Point const &pt) {
        return (std::abs(pt.x - wl.start.x) + std::abs(pt.x - wl.end.x) == std::abs(wl.start.x - wl.end.x)) *
               (std::abs(pt.y - wl.start.y) + std::abs(pt.y - wl.end.y) == std::abs(wl.start.y - wl.end.y));
    };

    // DATA PREP
    auto ctreNotComma = ctre::search<R"([^,]+)">;
    auto input        = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, ctreNotComma);
    std::vector<std::vector<WireLine>> vOfV_wireLines;

    for (auto &oneWireInput : input) {
        Point curPoint{0, 0};
        vOfV_wireLines.push_back(std::vector<WireLine>());
        for (auto &wireInstr : oneWireInput) {
            Point pt_change{0, 0};
            if (wireInstr.front() == 'U') {
                pt_change.y += std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else if (wireInstr.front() == 'R') {
                pt_change.x += std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else if (wireInstr.front() == 'D') {
                pt_change.y -= std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else if (wireInstr.front() == 'L') {
                pt_change.x -= std::stoll(std::string(wireInstr.begin() + 1, wireInstr.end()));
            }
            else { assert(false); }

            vOfV_wireLines.back().push_back({curPoint, {curPoint.x + pt_change.x, curPoint.y + pt_change.y}});
            curPoint = vOfV_wireLines.back().back().end;
        }
    }

    std::vector<Point> candidates;

    // Find the intersection points ...
    for (auto &wire : vOfV_wireLines) {
        for (auto &otherWire : vOfV_wireLines) {
            if (&otherWire == &wire) { continue; }
            for (auto &wireLine_a : wire) {
                for (auto &wireLine_b : otherWire) { calc_intersection(wireLine_a, wireLine_b, candidates); }
            }
        }
    }

    // MAIN LOGIC
    std::vector<long long> stepToReachInterSecs(candidates.size(), 0);

    for (int cand = 0; cand < candidates.size(); ++cand) {
        for (auto &wire : vOfV_wireLines) {
            for (auto oneLine : wire) {
                if (is_pointOnWireline(oneLine, candidates[cand])) {
                    stepToReachInterSecs[cand] += calc_distance(oneLine.start, candidates[cand]);
                    break;
                }
                else { stepToReachInterSecs[cand] += calc_distance(oneLine.start, oneLine.end); }
            }
        }
    }

    auto ans = flux::ref(stepToReachInterSecs).filter([](auto &&a) { return a != 0; }).min([](auto &&aa, auto &&bb) {
        return aa < bb;
    });

    return ans.value();
}
} // namespace AOC2019