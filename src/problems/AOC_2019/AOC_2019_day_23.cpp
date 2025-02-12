#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <array>
#include <cassert>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>
#include <queue>
#include <vector>


namespace AOC2019 {
namespace incc = incom::commons;
long long day23_1(std::string dataFile) {
    // TYPE DEFS
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


    std::vector programs(
        50, incc::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb>(instrCodes, data));

    struct ProgramState {
        unsigned int outCycle = 0;
        unsigned int inpCycle = 0;

        std::pair<long long, std::array<long long, 2>> outPacket{0, {0, 0}};
        std::queue<std::array<long long, 2>>           inpQueue;

        bool booted = false;

        void constructPacket(std::vector<ProgramState> &pStates, long long const &outputValue) {
            if (outCycle == 0) { outPacket.first = outputValue; }
            else if (outCycle == 1) { outPacket.second.front() = outputValue; }
            else if (outCycle == 2) {
                outPacket.second.back() = outputValue;
                pStates.at(outPacket.first).inpQueue.push(outPacket.second);
            }
            outCycle = (++outCycle) % 3;
        }

        void inputPacket(long long &storageRegister) {
            if (inpQueue.empty()) {
                storageRegister = -1;
                return;
            }
            else if (inpCycle == 0) { storageRegister = inpQueue.front().front(); }
            else if (inpCycle == 1) {
                storageRegister = inpQueue.front().back();
                inpQueue.pop();
            }
            inpCycle = (++inpCycle) % 2;
        }
    };

    std::vector progStates(256, ProgramState());
    size_t      nextAddress = 0;

    while (progStates.at(255).inpQueue.empty()) {
        for (size_t curProg = 0; curProg < programs.size(); ++curProg) {
            auto instructions = overloaded{
                [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
                [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
                [&](end &a) { programs[curProg].m_cursor = LLONG_MIN; },
                [&](inp &a) {
                    if (progStates[curProg].booted) { progStates[curProg].inputPacket(a.m_refs[0].get()); }
                    else {
                        a.m_refs[0].get()          = nextAddress++;
                        progStates[curProg].booted = true;
                    }
                },
                [&](out &a) { progStates.at(curProg).constructPacket(progStates, a.m_refs[0].get()); },
                [&](jit &a) {
                    if (a.m_refs[0] != 0) { programs[curProg].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
                },
                [&](jif &a) {
                    if (a.m_refs[0] == 0) { programs[curProg].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
                },
                [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
                [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
                [&](arb &a) { programs[curProg].m_relativeBaseOffset += a.m_refs[0].get(); },
            };

            // Execute one instruction from the currently selected program
            if (programs[curProg].is_cursorValid()) { programs[curProg].exe_pointedToInstr(instructions); }

            if (not progStates.at(255).inpQueue.empty()) { break; }
        }
    }

    return progStates.at(255).inpQueue.front().back();
}

long long day23_2(std::string dataFile) {
    // TYPE DEFS
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


    std::vector programs(
        50, incc::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb>(instrCodes, data));


    std::optional<std::array<long long, 2>> NAT{std::nullopt};

    struct ProgramState {
        std::optional<std::array<long long, 2>> &NAT_ref;
        unsigned int                             outCycle = 0;
        unsigned int                             inpCycle = 0;

        std::pair<long long, std::array<long long, 2>> outPacket{0, {0, 0}};
        std::queue<std::array<long long, 2>>           inpQueue;

        bool booted = false;

        bool constructPacket(std::vector<ProgramState> &pStates, long long const &outputValue) {
            if (outCycle == 0) { outPacket.first = outputValue; }
            else if (outCycle == 1) { outPacket.second.front() = outputValue; }
            else if (outCycle == 2) {
                outPacket.second.back() = outputValue;
                if (outPacket.first == 255) { NAT_ref = outPacket.second; }
                else {
                    pStates.at(outPacket.first).inpQueue.push(outPacket.second);
                    outCycle = (++outCycle) % 3;
                    return true;
                }
            }
            outCycle = (++outCycle) % 3;
            return false;
        }

        bool inputPacket(long long &storageRegister) {
            if (inpQueue.empty()) {
                storageRegister = -1;
                return false;
            }
            else if (inpCycle == 0) {
                storageRegister = inpQueue.front().front();
                inpCycle        = (++inpCycle) % 2;
                return false;
            }
            else if (inpCycle == 1) {
                storageRegister = inpQueue.front().back();
                inpCycle        = (++inpCycle) % 2;
                inpQueue.pop();
                return true;
            }
            else { assert(false); }
        }
    };

    std::vector progStates(50, ProgramState(NAT));
    size_t      nextAddress = 0;

    long long lastYDeliveredByNAT = LLONG_MIN;
    size_t emptyQueues = std::ranges::count_if(progStates, [](auto &&a) { return a.inpQueue.empty(); });

    while (true) {
        for (size_t curProg = 0; curProg < programs.size(); ++curProg) {
            auto instructions = overloaded{
                [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
                [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
                [&](end &a) { programs[curProg].m_cursor = LLONG_MIN; },
                [&](inp &a) {
                    if (progStates[curProg].booted) {
                        emptyQueues += progStates[curProg].inputPacket(a.m_refs[0].get());
                    }
                    else {
                        a.m_refs[0].get()          = nextAddress++;
                        progStates[curProg].booted = true;
                    }
                },
                [&](out &a) {
                    emptyQueues -= progStates.at(curProg).constructPacket(progStates, a.m_refs[0].get());
                },
                [&](jit &a) {
                    if (a.m_refs[0] != 0) { programs[curProg].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
                },
                [&](jif &a) {
                    if (a.m_refs[0] == 0) { programs[curProg].m_cursor = (a.m_refs[1] - (a.get_numOfParams() + 1)); }
                },
                [&](lth &a) { a.m_refs[2].get() = (a.m_refs[0].get() < a.m_refs[1].get()); },
                [&](eql &a) { a.m_refs[2].get() = (a.m_refs[0].get() == a.m_refs[1].get()); },
                [&](arb &a) { programs[curProg].m_relativeBaseOffset += a.m_refs[0].get(); },
            };

            // Execute one instruction from the currently selected program
            if (programs[curProg].is_cursorValid()) { programs[curProg].exe_pointedToInstr(instructions); }
        }

        if (NAT.has_value() && emptyQueues == 50) {
            if (lastYDeliveredByNAT == NAT.value().back()) { break; }
            lastYDeliveredByNAT = NAT.value().back();

            progStates.at(0).inpQueue.push(NAT.value());
            emptyQueues--;
            NAT = std::nullopt;
        }
    }
    return lastYDeliveredByNAT;
}
} // namespace AOC2019