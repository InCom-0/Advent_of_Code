#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>


namespace AOC2019 {

long long day5_1(std::string dataFile, long long inputVal) {
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

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    auto ol_set = overloaded{[&](add &a) {}};

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out> pqa{instrCodes, data};

    long long lastOutput   = LLONG_MIN;
    auto      instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) {
            long long inp = 0;
            std::cout << "Enter input for instrID " << pqa.m_cursor << ": ";
            // std::cin >> inp;
            std::cout << "\n";
            a.m_refs[0].get() = inputVal;
            std::cout << "\n";
        },
        [&](out &a) {
            std::cout << pqa.m_cursor << " instrID outputs value: " << a.m_refs[0].get() << '\n';
            lastOutput = a.m_refs[0].get();
        },
    };

    while (pqa.is_cursorValid()) { pqa.exe_pointedToInstr(instructions); }

    return lastOutput;
}

long long day5_2(std::string dataFile, long long inputVal) {
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

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql> pqa{instrCodes, data};

    long long lastOutput   = LLONG_MIN;
    auto      instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) {
            long long inp = 0;
            std::cout << "Enter input for instrID " << pqa.m_cursor << ": ";
            // std::cin >> inp;
            std::cout << "\n";
            a.m_refs[0].get() = inputVal;
            std::cout << "\n";
        },
        [&](out &a) {
            std::cout << pqa.m_cursor << " instrID outputs value: " << a.m_refs[0].get() << '\n';
            lastOutput = a.m_refs[0].get();
        },
        [&](jit &a) {
            if (a.m_refs[0] != 0) { pqa.m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](jif &a) {
            if (a.m_refs[0] == 0) { pqa.m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
        },
        [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
        [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
    };

    while (pqa.is_cursorValid()) { pqa.exe_pointedToInstr(instructions); }

    return lastOutput;
}
} // namespace AOC2019