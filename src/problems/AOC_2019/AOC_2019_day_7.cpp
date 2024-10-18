#include <algorithm>
#include <array>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2019 {
long long day7_1(std::string dataFile) {

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

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8};


    std::array<incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql>, 5> amps;
    amps.fill({instrCodes, data});


    std::array<long long, 5> phases{0, 1, 2, 3, 4};

    size_t    selAmp         = 0;
    long long lastOutput     = 0;
    long long maxFinalOutput = LLONG_MIN;

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { amps[selAmp].m_cursor = LLONG_MIN; },
        [&](inp &a) {
            if (amps[selAmp].m_cursor == 0) { a.m_refs[0].get() = phases[selAmp]; }
            else { a.m_refs[0].get() = lastOutput; }
        },
        [&](out &a) {
            lastOutput            = a.m_refs[0].get();
            amps[selAmp].m_cursor = LLONG_MIN;
        },
        [&](jit &a) {
            if (a.m_refs[0] != 0) { amps[selAmp].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](jif &a) {
            if (a.m_refs[0] == 0) { amps[selAmp].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
        [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
    };


    do {
        lastOutput = 0;
        for (selAmp = 0; selAmp < amps.size(); ++selAmp) {
            while (amps[selAmp].is_cursorValid()) { amps[selAmp].exe_pointedToInstr(instructions); }
            amps[selAmp].m_cursor  = 0;
            amps[selAmp].m_program = data;
        }
        maxFinalOutput = std::max(maxFinalOutput, lastOutput);
    } while (std::ranges::next_permutation(phases).found);

    return maxFinalOutput;
}

long long day7_2(std::string dataFile) {
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

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8};


    std::array<incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql>, 5> amps;
    amps.fill({instrCodes, data});


    std::array<long long, 5> phases{5, 6, 7, 8, 9};

    size_t    selAmp         = 0;
    long long lastOutput     = 0;
    long long maxFinalOutput = LLONG_MIN;

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { amps[selAmp].m_cursor = LLONG_MIN; },
        [&](inp &a) {
            if (amps[selAmp].m_cursor == 0) { a.m_refs[0].get() = phases[selAmp]; }
            else { a.m_refs[0].get() = lastOutput; }
        },
        [&](out &a) {
            lastOutput = a.m_refs[0].get();
            selAmp++; // output instruction means we should continue with the next amplifier
        },
        [&](jit &a) {
            if (a.m_refs[0] != 0) { amps[selAmp].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](jif &a) {
            if (a.m_refs[0] == 0) { amps[selAmp].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
        [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
    };

    // Cycle based on permutation of 'phases' array
    do {
        lastOutput = 0;
        // Cycle through amplifiers until the last one halts
        do {
            for (selAmp = 0; selAmp < amps.size(); ++selAmp) {
                while (amps[selAmp].is_cursorValid() && selAmp < amps.size()) {
                    amps[selAmp].exe_pointedToInstr(instructions);
                }
            }
        } while (amps.back().is_cursorValid());
        maxFinalOutput = std::max(maxFinalOutput, lastOutput);

        // Reset all programs to initial state
        for (auto &oneAmp : amps) {
            oneAmp.m_cursor  = 0;
            oneAmp.m_program = data;
        }
    } while (std::ranges::next_permutation(phases).found);

    return maxFinalOutput;
}
} // namespace AOC2019