#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>
#include <unordered_map>
#include <variant>


namespace AOC2019 {

struct IstrBase {
    std::vector<std::reference_wrapper<long long>> m_refs;
    virtual constexpr long long get_numOfParams() = 0;
};

template <typename... instrT>
class INT_PQA {
private:
    std::unordered_map<long long, std::variant<instrT...>, incom::commons::XXH3Hasher> m_instrTypeMap;

    static std::unordered_map<long long, std::variant<instrT...>, incom::commons::XXH3Hasher> instrTypeMapCreator(
        std::vector<long long> const &instrCodes) {
        assert(sizeof...(instrT) == instrCodes.size());
        std::unordered_map<long long, std::variant<instrT...>, incom::commons::XXH3Hasher> res;

        long long id = 0;

        (res.insert({instrCodes[id++], instrT()}), ...);
        return res;
    }

public:
    long long              m_cursor = 0;
    std::vector<long long> m_program;

    // CONSTRUCTION
    INT_PQA() = default;
    INT_PQA(std::vector<long long> const &instrCodes, std::vector<long long> const &programCodes, long long cursor = 0)
        : m_instrTypeMap(instrTypeMapCreator(instrCodes)), m_program(programCodes), m_cursor(cursor) {};

    // IS FUNCTIONS
    bool is_cursorValid() { return ((m_cursor >= 0) && (m_cursor < m_program.size())); }

    // CONSTRUCTING VARIANTS
    auto get_externalCursorInstr(long long const &progCursor) -> std::variant<instrT...> {
        long long diviRes = (m_program[progCursor] / 100);
        long long instrID = (m_program[progCursor] % 100);

        std::vector<int> params;
        while (diviRes != 0) {
            params.push_back(diviRes % 10);
            diviRes /= 10;
        }

        std::variant<instrT...> constructedVar(m_instrTypeMap.at(instrID));

        int  paramIDx     = 1;
        auto finishTheVar = overloaded{
            [&](auto &a) -> void {
                for (auto &param : params) {
                    // Position mode
                    if (param == 0) { a.m_refs.push_back(m_program[m_program[progCursor + paramIDx]]); }

                    // Immediate mode
                    else if (param == 1) { a.m_refs.push_back(m_program[progCursor + paramIDx]); }

                    // Can't do undefined parameter ID
                    else { assert(false); }
                    paramIDx++;
                }

                for (; paramIDx < a.get_numOfParams() + 1; ++paramIDx) {
                    // Position mode - defaulted because of missing explicit param
                    a.m_refs.push_back(m_program[m_program[progCursor + paramIDx]]);
                }
            },
        };

        std::visit(finishTheVar, constructedVar);
        return constructedVar;
    }
    auto get_pointedToInstr() -> std::variant<instrT...> { return get_externalCursorInstr(m_cursor); }

    // EXECUTION OF INSTRUCTIONS
    auto exe_externalPointedToInstr(long long &externalID, auto const &ol_set) -> long long {
        auto instruction = get_externalCursorInstr(externalID);

        auto ol_set_numOfParams = overloaded{[](auto &instr) { return instr.get_numOfParams() + 1; }};

        std::visit(ol_set, instruction);
        return externalID += std::visit(ol_set_numOfParams, instruction);
    }
    auto exe_pointedToInstr(auto const &ol_set) -> long long {
        return exe_externalPointedToInstr(m_cursor, std::forward<decltype(ol_set)>(ol_set));
    }
};


long long day5_1(std::string dataFile, long long inputVal) {
    struct add : IstrBase {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct mul : IstrBase {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct end : IstrBase {
        constexpr long long get_numOfParams() override { return 0; }
    };
    struct inp : IstrBase {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct out : IstrBase {
        constexpr long long get_numOfParams() override { return 1; }
    };

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    auto ol_set = overloaded{[&](add &a) {}};

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4};

    INT_PQA<add, mul, end, inp, out> pqa{instrCodes, data};

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
    struct add : IstrBase {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct mul : IstrBase {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct end : IstrBase {
        constexpr long long get_numOfParams() override { return 0; }
    };
    struct inp : IstrBase {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct out : IstrBase {
        constexpr long long get_numOfParams() override { return 1; }
    };
    struct jit : IstrBase {
        constexpr long long get_numOfParams() override { return 2; }
    };
    struct jif : IstrBase {
        constexpr long long get_numOfParams() override { return 2; }
    };
    struct lth : IstrBase {
        constexpr long long get_numOfParams() override { return 3; }
    };
    struct eql : IstrBase {
        constexpr long long get_numOfParams() override { return 3; }
    };

    auto                   d_ctre = ctre::search<R"(-?\d+)">;
    auto                   input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);
    std::vector<long long> data;

    auto ol_set = overloaded{[&](add &a) {}};

    for (auto &str : input.front()) { data.push_back(std::stoll(str)); }
    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8};

    INT_PQA<add, mul, end, inp, out, jit, jif, lth, eql> pqa{instrCodes, data};

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