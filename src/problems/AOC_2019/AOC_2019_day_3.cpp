#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>
#include <string>


namespace AOC2019 {


long long day3_1(std::string dataFile) {

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
            if ((firstWire.start.x >= secondWire.start.x && firstWire.start.x <= secondWire.end.x) ||
                (firstWire.start.x <= secondWire.start.x && firstWire.start.x >= secondWire.end.x)) {
                if ((secondWire.start.y >= firstWire.start.y && secondWire.start.y <= firstWire.end.y) ||
                    (secondWire.start.y <= firstWire.start.y && secondWire.start.y >= firstWire.end.y)) {
                    candidates.push_back(Point{firstWire.start.x, secondWire.start.y});
                }
            }
        }

        if (not is_TD(firstWire) && is_TD(secondWire)) {
            if ((firstWire.start.y >= secondWire.start.y && firstWire.start.y <= secondWire.end.y) ||
                (firstWire.start.y <= secondWire.start.y && firstWire.start.y >= secondWire.end.y)) {
                if ((secondWire.start.x >= firstWire.start.x && secondWire.start.x <= firstWire.end.x) ||
                    (secondWire.start.x <= firstWire.start.x && secondWire.start.x >= firstWire.end.x)) {
                    candidates.push_back(Point{secondWire.start.x, firstWire.start.y});
                }
            }
        }
    };


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

    // Super ugly
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

    struct Point {
        long long x;
        long long y;
    };

    struct WireLine {
        Point start;
        Point end;
    };

    auto calc_intersection = [&](WireLine const &firstWire, WireLine const &secondWire,
                                 std::vector<Point> &candidates) {
        auto is_TD = [&](WireLine const &wire) { return wire.start.x == wire.end.x; };

        if (is_TD(firstWire) && not is_TD(secondWire)) {
            if ((firstWire.start.x >= secondWire.start.x && firstWire.start.x <= secondWire.end.x) ||
                (firstWire.start.x <= secondWire.start.x && firstWire.start.x >= secondWire.end.x)) {
                if ((secondWire.start.y >= firstWire.start.y && secondWire.start.y <= firstWire.end.y) ||
                    (secondWire.start.y <= firstWire.start.y && secondWire.start.y >= firstWire.end.y)) {
                    candidates.push_back(Point{firstWire.start.x, secondWire.start.y});
                }
            }
        }

        if (not is_TD(firstWire) && is_TD(secondWire)) {
            if ((firstWire.start.y >= secondWire.start.y && firstWire.start.y <= secondWire.end.y) ||
                (firstWire.start.y <= secondWire.start.y && firstWire.start.y >= secondWire.end.y)) {
                if ((secondWire.start.x >= firstWire.start.x && secondWire.start.x <= firstWire.end.x) ||
                    (secondWire.start.x <= firstWire.start.x && secondWire.start.x >= firstWire.end.x)) {
                    candidates.push_back(Point{secondWire.start.x, firstWire.start.y});
                }
            }
        }
    };


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

    // Super ugly
    for (auto &wire : vOfV_wireLines) {
        for (auto &otherWire : vOfV_wireLines) {
            if (&otherWire == &wire) { continue; }
            for (auto &wireLine_a : wire) {
                for (auto &wireLine_b : otherWire) { calc_intersection(wireLine_a, wireLine_b, candidates); }
            }
        }
    }

    std::vector<long long> stepToReachInterSecs(candidates.size(), 0);

    for (int cand = 0; cand < candidates.size(); ++cand) {
        for (auto &wire : vOfV_wireLines) {
            for (auto oneLine : wire) {
                if (oneLine.end.x == oneLine.start.x) {
                    if (candidates[cand].x == oneLine.start.x &&
                        ((candidates[cand].y >= oneLine.start.y && candidates[cand].y <= oneLine.end.y) ||
                         (candidates[cand].y <= oneLine.start.y && candidates[cand].y >= oneLine.end.y))) {
                        stepToReachInterSecs[cand] += std::abs(candidates[cand].y - oneLine.start.y);
                        break;
                    }
                    else { stepToReachInterSecs[cand] += std::abs(oneLine.end.y - oneLine.start.y); }
                }
                else if (oneLine.end.y == oneLine.start.y) {
                    if (candidates[cand].y == oneLine.start.y &&
                        ((candidates[cand].x >= oneLine.start.x && candidates[cand].x <= oneLine.end.x) ||
                         (candidates[cand].x <= oneLine.start.x && candidates[cand].x >= oneLine.end.x))) {
                        stepToReachInterSecs[cand] += std::abs(candidates[cand].x - oneLine.start.x);
                        break;
                    }
                    else { stepToReachInterSecs[cand] += std::abs(oneLine.end.x - oneLine.start.x); }
                }
                else { assert(false); }
            }
        }
    }

    auto ans = flux::ref(stepToReachInterSecs).filter([](auto &&a) { return a != 0; }).min([](auto &&aa, auto &&bb) {
        return aa < bb;
    });


    return ans.value();
}
} // namespace AOC2019