#include <queue>

#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2017 {

/*
Very much simplified 'data setup' logic for these types of problems
Much simpler to write.

Complexity of manual writing of the conditions is cut.
It is probably also more efficient as virtually all of the data is stored in 1 vector.
*/
long long day18_1(std::string dataFile) {
    // CUSTOM LOCAL TYPES DEFINITIONS
    struct snd : incom::commons::PQA::_instrBase {};
    struct set : incom::commons::PQA::_instrBase {};
    struct add : incom::commons::PQA::_instrBase {};
    struct mul : incom::commons::PQA::_instrBase {};
    struct mod : incom::commons::PQA::_instrBase {};
    struct rcv : incom::commons::PQA::_instrBase {};
    struct jgz : incom::commons::PQA::_instrBase {};

    auto getWord_ctre = ctre::search<R"([[:^blank:]]+)">;
    auto VofV         = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, getWord_ctre);

    // DATA PREP
    incom::commons::PQA::ProgramQuasiAssembly<snd, set, add, mul, mod, rcv, jgz> theProgram(VofV, 0);

    // LAMBDA DEFINITIONS
    long long lastSoundPlayed = LLONG_MIN;
    auto      overload_obj    = overloaded{
        [&](const snd &a) { lastSoundPlayed = a.source.get(); },
        [&](const set &a) { a.source.get() = a.target; },
        [&](const add &a) { a.source += a.target; },
        [&](const mul &a) { a.source *= a.target; },
        [&](const mod &a) { a.source %= a.target; },
        [&](const rcv &a) {
            if (a.source != 0) { theProgram.instructionID = LLONG_MAX; }
        },
        [&](const jgz &a) {
            if (a.source > 0) { theProgram.instructionID += (a.target - 1); }
        },
    };

    // MAIN LOGIC
    while (theProgram.test_isInstructionIDvalid()) { std::visit(overload_obj, theProgram.getCurrentAndIncrement()); }

    return lastSoundPlayed;
}

long long day18_2(std::string dataFile, int numOfPrograms) {
    // CUSTOM LOCAL TYPES DEFINITIONS
    struct snd : incom::commons::PQA::_instrBase {};
    struct set : incom::commons::PQA::_instrBase {};
    struct add : incom::commons::PQA::_instrBase {};
    struct mul : incom::commons::PQA::_instrBase {};
    struct mod : incom::commons::PQA::_instrBase {};
    struct rcv : incom::commons::PQA::_instrBase {};
    struct jgz : incom::commons::PQA::_instrBase {};

    auto getWord_ctre = ctre::search<R"([[:^blank:]]+)">;
    auto VofV         = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, getWord_ctre);

    // DATA AND LAMBDA PREP
    std::vector<incom::commons::PQA::ProgramQuasiAssembly<snd, set, add, mul, mod, rcv, jgz>> programs;
    for (int i = 0; i < numOfPrograms; ++i) { programs.emplace_back(VofV, i); }

    std::vector<unsigned short>        switch_Xs(numOfPrograms, false);
    std::vector<long long>             senderCounter(numOfPrograms, 0);
    std::vector<std::queue<long long>> queues(numOfPrograms, std::queue<long long>());

    int  progID        = 0;
    bool queuesChanged = true;
    
    auto overload_obj  = overloaded{
        // If the 'snd' instruction specified ID of where to send the data, it would be possible to use arbitrary num of
        // programs. In this case ... it is just switching to 'the other one' 0 or 1;
        [&](const snd &a) {
            queues[not (progID)].push(a.source);
            senderCounter[progID]++;
            queuesChanged = true;
        },
        [&](const set &a) { a.source.get() = a.target; },
        [&](const add &a) { a.source += a.target; },
        [&](const mul &a) { a.source *= a.target; },
        [&](const mod &a) { a.source %= a.target; },
        [&](const rcv &a) {
            if (queues[progID].empty()) {
                switch_Xs[progID] = true;
                programs[progID].instructionID--;
            }
            else {
                a.source.get() = queues[progID].front();
                queues[progID].pop();
                queuesChanged = true;
            }
        },
        [&](const jgz &a) {
            if (a.source > 0) { programs[progID].instructionID += (a.target - 1); }
            switch_Xs[progID] = false;
        },
    };

    // MAIN LOGIC
    while (queuesChanged) { // If no queue changed it must be a deadlock
        queuesChanged = false;
        for (progID = 0; progID < programs.size(); ++progID) {
            while (programs[progID].test_isInstructionIDvalid() && switch_Xs[progID] == false) {
                std::visit(overload_obj, programs[progID].getCurrentAndIncrement());
            }
        }
        std::ranges::fill(switch_Xs, false);
    }

    return senderCounter[1];
}

} // namespace AOC2017