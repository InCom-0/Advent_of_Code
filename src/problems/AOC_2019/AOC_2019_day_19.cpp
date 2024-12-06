#include <algorithm>
#include <array>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/core.h>
#include <incom_commons.h>
#include <ranges>
#include <string>


namespace AOC2019 {
std::vector<std::vector<int>> day19_0(std::string dataFile, std::pair<int, int> const exploreSize) {
    struct add : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct mul : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct end : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 0; }
    };
    struct inp : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct out : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct jit : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 2; }
    };
    struct jif : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 2; }
    };
    struct lth : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct eql : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct arb : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 1; }
    };

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8, 9};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa_orig(instrCodes,
                                                                                                             data);
    auto                                                                                            pqa = pqa_orig;

    // even positions are X coords, odd positions are Y coords
    std::vector<int> inpCoords;
    for (int x = 0; x < exploreSize.first; ++x) {
        for (int y = 0; y < exploreSize.second; ++y) {
            inpCoords.push_back(x);
            inpCoords.push_back(y);
        }
    }

    size_t                   nextInp = 0;
    size_t                   counter = 0;
    std::array<long long, 2> valInput;
    bool                     inBeam = false;

    std::vector res(exploreSize.first, std::vector<int>());

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) { a.m_refs[0].get() = inpCoords[nextInp++]; },
        [&](out &a) {
            counter += a.m_refs[0].get();
            if (a.m_refs[0].get()) { res[inpCoords[nextInp - 2]].push_back(inpCoords[nextInp - 1]); }

            // std::cout << "out says: " << a.m_refs[0].get() << '\n';
        },
        [&](jit &a) {
            if (a.m_refs[0] != 0) { pqa.m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](jif &a) {
            if (a.m_refs[0] == 0) { pqa.m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
        [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
        [&](arb &a) { pqa.m_relativeBaseOffset += a.m_refs[0].get(); },
    };

    size_t lowY      = 0;
    size_t skipCount = 1;

    for (size_t x = 0; x < exploreSize.first; ++x) {
        for (size_t y = lowY; y < exploreSize.second; ++y) {

            pqa = pqa_orig;
            while (pqa.is_cursorValid()) { pqa.exe_pointedToInstr(instructions); }
        }
    }
    return res;
}

long long day19_1(std::string dataFile) {

    auto        beamedLocations = day19_0(dataFile, {50, 50});
    return std::ranges::fold_left(beamedLocations, 0ll, [](auto &&ini, auto const &item) { return ini + item.size(); });
}

long long day19_2(std::string dataFile) {
    struct add : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct mul : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct end : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 0; }
    };
    struct inp : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct out : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct jit : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 2; }
    };
    struct jif : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 2; }
    };
    struct lth : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct eql : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct arb : incom::commons::PQA::_instrBase_INT {
        constexpr long long get_numOfParams() override { return 1; }
    };

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8, 9};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa_orig(instrCodes,
                                                                                                             data);
    auto                                                                                            pqa = pqa_orig;

    auto fiftyByFifty = day19_0(dataFile, {10, 10});

    auto firstIt =
        std::ranges::find_if(std::views::drop(fiftyByFifty, 1), [&](auto const &line) { return line.size() > 0; });
    auto secondIt = std::ranges::find_if(std::views::drop(fiftyByFifty, firstIt - fiftyByFifty.begin() + 1),
                                         [&](auto const &line) { return line.size() > 0; });


    std::array<int, 2> step, bot, mid, top, target;

    mid.front() = (secondIt - fiftyByFifty.begin()) + 99;
    mid.back()  = secondIt->front() + 99;

    step = {1, 1};

    auto stepOrig = step;

    size_t             nextInp = 0;
    bool               inBeam  = 0;
    std::array<int, 2> microAdj{0, 0};

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) { a.m_refs[0].get() = target[nextInp++]; },
        [&](out &a) {
            inBeam = a.m_refs[0].get();
            // std::cout << "out says: " << a.m_refs[0].get() << '\n';
        },
        [&](jit &a) {
            if (a.m_refs[0] != 0) { pqa.m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](jif &a) {
            if (a.m_refs[0] == 0) { pqa.m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
        [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
        [&](arb &a) { pqa.m_relativeBaseOffset += a.m_refs[0].get(); },
    };
    
    // LAMBDA for testing the coordinates
    auto eva_fitCustom = [&](std::array<int, 2> const &topLeft, std::array<int, 2> const adjustment) {
        target          = topLeft;
        target.front() += adjustment.front();
        target.back()  += adjustment.back();
        pqa             = pqa_orig;
        nextInp         = 0;
        while (pqa.is_cursorValid()) { pqa.exe_pointedToInstr(instructions); }
        return inBeam;
    };

    bot = mid;
    bool testRes;

    // BINARY SEARCH ... finding the 'top' border of problem space
    do {
        for (auto &stepDim : step) { stepDim *= 2; }
        mid.front() += step.front();
        mid.back()  += step.back();

        microAdj = {0, 0};
        while (not eva_fitCustom(mid, microAdj)) { microAdj.front()--; };

        std::array<int, 2> adjUPRIGHT{-99 + microAdj.front(), 99};

        testRes = eva_fitCustom(mid, adjUPRIGHT);
    } while (not testRes);

    // REAL BINARY SEARCH, 'top' will be the result we want before adjustments
    top = mid;
    while (true) {
        for (int i = 0; i < 2; i++) { mid[i] = (top[i] + bot[i]) / 2; }
        if (bot == mid) { break; }

        microAdj = {0, 0};
        while (not eva_fitCustom(mid, microAdj)) { microAdj.front()--; };

        std::array<int, 2> adjUPRIGHT{-99 + microAdj.front(), 99};

        testRes = eva_fitCustom(mid, adjUPRIGHT);
        if (testRes) { top = mid; }
        else { bot = mid; }
    }

    microAdj = {0, 0};
    while (not eva_fitCustom(top, microAdj)) { microAdj.front()--; };

    return (10'000*(top.front() + microAdj.front()-99) + (top.back() + microAdj.back()));
}
} // namespace AOC2019