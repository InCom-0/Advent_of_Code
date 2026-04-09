#include <cassert>
#include <cstdint>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>


namespace AOC2025 {

size_t
day10_1(std::string dataFile) {
    auto notSpace_ctre = ctre::search<R"([^\s]+)">;
    auto input         = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, notSpace_ctre);

    struct BF {
        uint32_t bf : 16 = 0;
    };

    BF                           startLamp{};
    std::vector<BF>              tarLamps(input.size());
    std::vector<std::vector<BF>> buttonsVofV(input.size());
    std::string_view             delim{","};

    size_t resAccu = 0uz;
    for (auto const &[tarLamp, buttons, inpLine] : std::views::zip(tarLamps, buttonsVofV, input)) {
        int lampSize = 0;
        for (uint32_t shift = 0;
             char     ch : std::views::reverse(std::span(inpLine.front().begin() + 1, inpLine.front().end() - 1))) {
            if (ch == '#') { tarLamp.bf |= (1 << shift); }
            shift++;
            lampSize++;
        }

        for (auto const &buttonDef : std::span(inpLine.begin() + 1, inpLine.end() - 1)) {
            buttons.push_back(BF{});
            auto numOnlyView = std::views::split(std::span(buttonDef.begin() + 1, buttonDef.end() - 1), delim);
            for (auto const &oneNum : numOnlyView) {
                uint32_t shift = std::abs(std::stoi(std::string(oneNum.begin(), oneNum.end())) - lampSize + 1);

                buttons.back().bf |= (1 << shift);
            }
        }


        auto execu = [&](size_t const level = 1) -> std::optional<size_t> {
            std::vector<size_t> const ends(std::from_range, std::views::iota(buttons.size() - level, buttons.size()));
            std::vector<size_t>       stack{};
            size_t                    seqStartPos = 0;

            do {
                while (stack.size() > 0 && stack.back() == ends.at(stack.size() - 1)) { stack.pop_back(); }

                if (stack.size() < level) {
                    if (stack.size() == 0) { stack.push_back(seqStartPos++); }
                    else { stack.back()++; }
                    while (stack.size() < level) { stack.push_back(stack.back() + 1); }
                }
                else { stack.back()++; }
                BF curLamp{};

                for (auto buttonID : stack) { curLamp.bf ^= buttons.at(buttonID).bf; }
                if (curLamp.bf == tarLamp.bf) { return stack.size(); }

            } while (stack.front() < ends.front());

            return std::nullopt;
        };

        for (size_t pressedButtons = 1; pressedButtons < buttons.size(); ++pressedButtons) {
            if (auto oneRes = execu(pressedButtons)) {
                resAccu += oneRes.value();
                break;
            }
        }
    }


    return resAccu;
}
size_t
day10_2(std::string dataFile) {
    auto notSpace_ctre = ctre::search<R"([^\s]+)">;
    auto input         = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, notSpace_ctre);

    struct BF {
        uint32_t bf : 16 = 0;
    };


    std::string_view delim{","};

    size_t resAccu = 0uz;
    for (auto const &inpLine : input) {
        int              joltSize = 0uz;
        std::vector<int> joltage;
        for (auto const &oneJolt :
             std::views::split(std::span(inpLine.back().begin() + 1, inpLine.back().end() - 1), delim)) {
            joltage.push_back(std::stoi(std::string(oneJolt.begin(), oneJolt.end())));
            joltSize++;
        }

        std::vector<std::vector<int>> buttons{};
        for (auto const &buttonDef : std::span(inpLine.begin() + 1, inpLine.end() - 1)) {
            buttons.emplace_back();
            for (auto const &oneNum : std::views::split(std::span(buttonDef.begin() + 1, buttonDef.end() - 1), delim)) {
                buttons.back().push_back(std::abs(std::stoi(std::string(oneNum.begin(), oneNum.end()))));
            }
        }

        incom::aoc::solver                                solver{};
        std::vector<incom::aoc::solver::equation_builder> ebs{};

        for (auto const oneJolt : joltage) { ebs.push_back(solver.build()); }
        for (char butName = 'a'; auto const &oneButton : buttons) {
            for (auto const oneWire : oneButton) { ebs.at(oneWire).input(butName); }
            butName++;
        }

        for (size_t id = 0; auto const oneJolt : joltage) { ebs.at(id++).equals(oneJolt); }

        auto const bestSolution = solver.solve();
        if (bestSolution) {
            resAccu += bestSolution->sum;
        }
        else {assert(false);}
    }


    return resAccu;
}

} // namespace AOC2025