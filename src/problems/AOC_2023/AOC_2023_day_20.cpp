#include <algorithm>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <iterator>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <variant>

#include <ankerl/unordered_dense.h>
#include <incom_commons.hpp>


namespace AOC2023 {
namespace {
struct Module {
    std::string              name{};
    std::vector<std::string> targetNames{};

    std::vector<size_t> targetIDX{};
};

struct FlipFlop : Module {
    bool state = false;
};

struct Conjunction : Module {
    std::vector<std::pair<size_t, bool>> tracker;

    bool
    setTrackerFor(size_t const &ID, bool const &val) {
        if (auto found = std::ranges::find_if(tracker, [&](auto const &item) { return item.first == ID; });
            found != tracker.end()) {
            found->second = val;
            return true;
        }

        return false;
    }

    bool
    is_allHigh() const {
        return std::ranges::all_of(tracker, [](auto const &item) { return item.second == true; });
    }
};

struct Broadcast : Module {};
struct NoModule : Module {};

struct Program {
    struct Signal {
        std::optional<size_t> source{};
        size_t                target{};
        bool                  value = false;
    };

    std::vector<std::variant<FlipFlop, Conjunction, Broadcast, NoModule>> instrs{};
    std::queue<Signal>                                                    signals_inFlight{};
    size_t                                                                rxModuleLowReceived{};

    Signal
    getAndPop() {
        Signal res = std::move(signals_inFlight.front());
        signals_inFlight.pop();
        return res;
    }

    std::pair<size_t, size_t>
    processInstr(Signal const &s) {
        auto &curInstr = instrs.at(s.target);

        auto ol_obj = incstd::variant_utils::Overloads{
            [&](FlipFlop &a) -> std::pair<size_t, size_t> {
                std::pair<size_t, size_t> res{};
                if (s.value == true) { return res; }
                else {
                    a.state = not a.state;
                    for (auto const &tarID : a.targetIDX) {
                        signals_inFlight.push(Signal{.source = s.target, .target = tarID, .value = a.state});
                    }
                    res.first  = (not a.state) * a.targetIDX.size();
                    res.second = (a.state) * a.targetIDX.size();
                }

                return res;
            },

            [&](Conjunction &a) -> std::pair<size_t, size_t> {
                a.setTrackerFor(s.source.value(), s.value);
                bool const outputVal = not a.is_allHigh();
                for (auto const &tarID : a.targetIDX) {
                    signals_inFlight.push(Signal{.source = s.target, .target = tarID, .value = outputVal});
                }
                return std::pair<size_t, size_t>{(not outputVal) * a.targetIDX.size(), outputVal * a.targetIDX.size()};
            },

            [&](const Broadcast &a) -> std::pair<size_t, size_t> {
                for (auto const &tarID : a.targetIDX) {
                    signals_inFlight.push(Signal{.source = s.target, .target = tarID, .value = s.value});
                }
                return std::pair<size_t, size_t>{(not s.value) * a.targetIDX.size(), s.value * a.targetIDX.size()};
            },
            [&](const NoModule &a) -> std::pair<size_t, size_t> {
                if (s.value == false) { rxModuleLowReceived++; }
                return std::make_pair(0uz, 0uz);
            },
        };
        return std::visit(ol_obj, curInstr);
    }

    auto
    part1_execute(size_t times, auto const &startSignals) {
        size_t lowSigCount{};
        size_t highSigCount{};
        auto   addTo = [&](std::pair<size_t, size_t> const &other) {
            lowSigCount  += other.first;
            highSigCount += other.second;
        };

        while (times-- > 0uz) {
            lowSigCount++;
            signals_inFlight.push_range(startSignals);
            while (not signals_inFlight.empty()) { addTo(processInstr(getAndPop())); }
        }

        return std::make_pair(lowSigCount, highSigCount);
    }
    auto
    part2_execute(auto const &startSignals) {
        size_t programRunCount{};
        auto   filt = [&](std::string const &str) {
            auto ol_obj = incstd::variant_utils::Overloads{
                [&](auto const &a) -> bool { return a.name == str; },
            };

            return std::ranges::find_if(instrs, [&](auto const &inst) { return std::visit(ol_obj, inst); }) -
                   instrs.begin();
        };

        size_t pl = filt("pl");
        size_t mz = filt("mz");
        size_t lz = filt("lz");
        size_t zm = filt("zm");


        std::array<std::vector<size_t>, 4uz> whenHigh;
        while (std::ranges::any_of(whenHigh, [](auto const &oneVec) { return oneVec.size() < 2; })) {
            programRunCount++;
            signals_inFlight.push_range(startSignals);
            while (not signals_inFlight.empty()) {
                if (signals_inFlight.front().value == true) {
                    if (signals_inFlight.front().source == pl) { whenHigh.at(0).push_back(programRunCount); }
                    if (signals_inFlight.front().source == mz) { whenHigh.at(1).push_back(programRunCount); }
                    if (signals_inFlight.front().source == lz) { whenHigh.at(2).push_back(programRunCount); }
                    if (signals_inFlight.front().source == zm) { whenHigh.at(3).push_back(programRunCount); }
                }

                processInstr(getAndPop());
            }
        }

        return std::ranges::fold_left(whenHigh, 1uz,
                                      [](auto const &ini, auto const &oneInp) { return std::lcm(ini, oneInp.at(0)); });
    }
};
} // namespace

auto
day20_0(std::string dataFile) {
    auto wplus_ctre = ctre::search<R"((&|%|\w)+)">;
    auto tmpRes     = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, wplus_ctre);

    ankerl::unordered_dense::set<std::string, incstd::hashing::XXH3Hasher> nameSet;
    for (auto const &oneLine : tmpRes) {
        if (oneLine.empty()) { continue; }
        nameSet.insert(std::string(oneLine.front().begin() + 1, oneLine.front().end()));
        for (auto const &tars : std::views::drop(oneLine, 1)) { nameSet.insert(tars); }
    }

    auto getID = [&](auto const &str) -> std::optional<size_t> {
        std::optional<size_t> res;
        auto                  found = nameSet.find(str);
        if (found != nameSet.end()) { res = found - nameSet.begin(); }
        return res;
    };


    Program p{};
    for (auto const &oneName : nameSet) {
        if (auto itToLine = std::ranges::find_if(tmpRes,
                                                 [&](auto const &line) {
                                                     if (line.size() == 0uz) { return false; }
                                                     else if (std::string_view(line.front().begin() + 1,
                                                                               line.front().end()) == oneName) {
                                                         return true;
                                                     }
                                                     return false;
                                                 });
            itToLine != tmpRes.end()) {
            auto const &oneLine = *itToLine;

            std::vector<size_t> targetIDX{};

            // Processing of links to its target modules
            for (auto const &tars : std::views::drop(oneLine, 1)) {
                if (auto optID = getID(tars)) { targetIDX.push_back(std::move(optID.value())); }
                else { assert(false); }
            }

            // Designation of this module
            std::string firstStr(oneLine.front().begin() + 1, oneLine.front().end());
            if (oneLine.front().front() == '%') {
                p.instrs.push_back(
                    FlipFlop{{std::move(firstStr), std::views::drop(oneLine, 1) | std::ranges::to<std::vector>(),
                              std::move(targetIDX)},
                             false});
            }
            else if (oneLine.front().front() == '&') {
                ankerl::unordered_dense::set<std::string, incstd::hashing::XXH3Hasher> ns;
                for (auto const &line : tmpRes) {
                    for (auto const &oneName : std::views::drop(line, 1)) {
                        if (oneName == firstStr) {
                            ns.insert(std::string(line.front().begin() + 1, line.front().end()));
                        }
                    }
                }
                p.instrs.push_back(
                    Conjunction{{std::move(firstStr), std::views::drop(oneLine, 1) | std::ranges::to<std::vector>(),
                                 std::move(targetIDX)},
                                std::views::transform(ns, [&](auto const &item) {
                                    return std::make_pair(getID(item).value(), false);
                                }) | std::ranges::to<std::vector>()});
            }
            else if (oneLine.front().front() == 'b') {
                p.instrs.push_back(
                    Broadcast{{std::string(oneLine.front().begin(), oneLine.front().end()),
                               std::views::drop(oneLine, 1) | std::ranges::to<std::vector>(), std::move(targetIDX)}});
            }
        }
        else {
            size_t rr;
            p.instrs.push_back(NoModule{{oneName, {}, {}}});
        }
    }

    size_t a;
    return p;
}


size_t
day20_1(std::string dataFile) {

    auto prog = day20_0(dataFile);

    auto broadcasterIT = std::ranges::find_if(prog.instrs, [](auto const &var) {
        auto ol_obj = incstd::variant_utils::Overloads{
            [&](const auto &a) -> bool { return a.name == "broadcaster"; },
        };
        return std::visit(ol_obj, var);
    });

    auto &broadcaster = std::get<2>(*broadcasterIT);
    auto  startSignals =
        std::vector(1, Program::Signal{.source = std::nullopt,
                                       .target = static_cast<size_t>(std::distance(prog.instrs.begin(), broadcasterIT)),
                                       .value  = false});

    auto [low, high] = prog.part1_execute(1000, startSignals);
    return low * high;
}

size_t
day20_2(std::string dataFile) {
    auto prog = day20_0(dataFile);

    auto broadcasterIT = std::ranges::find_if(prog.instrs, [](auto const &var) {
        auto ol_obj = incstd::variant_utils::Overloads{
            [&](const auto &a) -> bool { return a.name == "broadcaster"; },
        };
        return std::visit(ol_obj, var);
    });

    auto &broadcaster = std::get<2>(*broadcasterIT);
    auto  startSignals =
        std::vector(1, Program::Signal{.source = std::nullopt,
                                       .target = static_cast<size_t>(std::distance(prog.instrs.begin(), broadcasterIT)),
                                       .value  = false});

    auto res = prog.part2_execute(startSignals);
    return res;
}

} // namespace AOC2023