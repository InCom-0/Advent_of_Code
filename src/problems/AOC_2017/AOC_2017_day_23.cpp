#include <cassert>
#include <iostream>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <vector>

namespace AOC2017 {
int day23_1(std::string dataFile) {
    // CUSTOM LOCAL TYPES
    struct set : AOC_commons::PQA::_instrBase {};
    struct sub : AOC_commons::PQA::_instrBase {};
    struct mul : AOC_commons::PQA::_instrBase {};
    struct jnz : AOC_commons::PQA::_instrBase {};


    auto getWord_ctre = ctre::search<R"([[:^blank:]]+)">;
    auto VofV         = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, getWord_ctre);

    AOC_commons::PQA::ProgramQuasiAssembly<set, sub, mul, jnz> theProgram(VofV);

    // DATA AND LAMBDA PREP
    unsigned long long mulInvocations = 0;
    auto               overload_obj   = overloaded{
        [&](const set &a) { a.source.get() = a.target; },
        [&](const sub &a) { a.source -= a.target; },
        [&](const mul &a) {
            a.source *= a.target;
            mulInvocations++;
        },
        [&](const jnz &a) {
            if (a.source != 0) { theProgram.instructionID += (a.target - 1); }
        },
    };

    std::cout << theProgram.mapping.size() << '\n';

    // MAIN LOGIC
    while (theProgram.test_isInstructionIDvalid()) { std::visit(overload_obj, theProgram.getCurrentAndIncrement()); }
    return mulInvocations;
}

// Essentially finding the number of composite (that is non-prime) numbers between 2 integers.
int day23_2(std::string dataFile) {
    // CUSTOM LOCAL TYPES
    struct set : AOC_commons::PQA::_instrBase {};
    struct sub : AOC_commons::PQA::_instrBase {};
    struct mul : AOC_commons::PQA::_instrBase {};
    struct jnz : AOC_commons::PQA::_instrBase {};

    auto getWord_ctre = ctre::search<R"([[:^blank:]]+)">;
    auto VofV         = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, getWord_ctre);

    AOC_commons::PQA::ProgramQuasiAssembly<set, sub, mul, jnz> theProgram(VofV);
    theProgram.mapping.at('a').get() = 1;

    // DATA AND LAMBDA PREP
    auto overload_obj = overloaded{
        [&](const set &a) { a.source.get() = a.target; },
        [&](const sub &a) { a.source -= a.target; },
        [&](const mul &a) { a.source *= a.target; },
        [&](const jnz &a) {
            if (a.source != 0) { theProgram.instructionID += (a.target - 1); }
        },
    };

    // MAIN LOGIC
    while (theProgram.test_isInstructionIDvalid()) { std::visit(overload_obj, theProgram.getCurrentAndIncrement()); }

    const long long startNum = theProgram.mapping.at('b').get();
    const long long endNum   = theProgram.mapping.at('c').get();
    const long long skip     = theProgram.mapping.at('f').get();

    unsigned long long counter = 0;

    for (long long i = startNum; i <= endNum; i += skip) {
        for (int j = 2; j <= endNum/2; j++) {
            if ((i % j) == 0) {
                counter++;
                break;
            }
        }
    }
    return counter;
}
} // namespace AOC2017