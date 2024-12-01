#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <incom_omni.h>
#include <queue>
#include <stack>


namespace AOC2019 {
auto day15_0(std::string dataFile, std::array<long long, 2> &k2Oxy) {
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

    incom::omni::MD_ChunkVector<char, 2, '_', 8> roomMap;
    using KT = decltype(roomMap)::Key_Type;
    KT curKey{0, 0};

    struct SelDir {
        unsigned int m_selDir : 2 = 0;
        long long    getDir() { return std::array<long long, 4>{1, 4, 2, 3}[m_selDir]; }
        long long    getReverse() { return std::array<long long, 4>{2, 3, 1, 4}[m_selDir]; }

        KT applySelDir2Key(KT const &theOtherKey) {
            KT                                             res = theOtherKey;
            std::array<std::pair<long long, long long>, 4> pr{
                std::make_pair(1ll, 0ll), {0ll, 1ll}, {-1ll, 0ll}, {0ll, -1ll}};
            res.front() += pr[m_selDir].first;
            res.back()  += pr[m_selDir].second;
            return res;
        }
    };

    SelDir                                                                                          selDir{};
    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);

    std::stack<std::pair<KT, std::pair<long long, SelDir>>> stck;

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) { a.m_refs[0].get() = stck.top().second.second.getDir(); },
        [&](out &a) {
            KT    newKey     = stck.top().second.second.applySelDir2Key(stck.top().first);
            auto &newRoomRef = roomMap.get(newKey);

            stck.top().second.second.m_selDir++;
            stck.top().second.first++;

            if (stck.top().second.first == 4) { stck.pop(); }
            else if (a.m_refs[0].get() == 0) { newRoomRef = '#'; }
            else {
                if (a.m_refs[0].get() == 2) { k2Oxy = newKey; }

                if (newRoomRef == '_') {
                    stck.push({newKey, {0, stck.top().second.second}});
                    stck.top().second.second.m_selDir -= 2;
                    newRoomRef                         = '.';
                }
                else if (newRoomRef == '.') {
                    stck.push({newKey, {3, stck.top().second.second}});
                    stck.top().second.second.m_selDir++;
                }
                else { assert(false); }
            }
            return;

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

    stck.push({{0, 0}, {0, SelDir{1}}});

    while (pqa.is_cursorValid() && not stck.empty()) { pqa.exe_pointedToInstr(instructions); }

    return roomMap;
}

long long day15_1(std::string dataFile) {
    std::array<long long, 2> k2Oxy;
    auto                     roomMap = day15_0(dataFile, k2Oxy);

    using KT = decltype(roomMap)::Key_Type;

    roomMap.get(k2Oxy) = 'T';
    KT evaluated{0, 0};
    roomMap.get(evaluated) = 'E';

    incom::commons::doubleBuffer<std::queue<KT>> doubleQue;
    doubleQue.getCurrent().push(KT{0, 0});
    std::vector<KT> dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    long long counter = 0;
    while (true) {
        if (doubleQue.getCurrent().empty()) {
            doubleQue.swapBuffers();
            counter++;
            if (doubleQue.getCurrent().empty()) { break; }
        }

        for (auto &oneDir : dirs) {
            evaluated          = doubleQue.getCurrent().front();
            evaluated.front() += oneDir.front();
            evaluated.back()  += oneDir.back();
            char &thisPos      = roomMap.get(evaluated);
            if (thisPos == 'T') {
                doubleQue.getCurrent() = std::queue<KT>{};
                doubleQue.getCurrent().push(evaluated);
                doubleQue.getNext() = std::queue<KT>{};
                break;
            }
            else if (thisPos == '.') {
                doubleQue.getNext().push(evaluated);
                thisPos = 'E';
            }
        }
        doubleQue.getCurrent().pop();
    }
    return counter;
}

long long day15_2(std::string dataFile) {

    std::array<long long, 2> k2Oxy;
    auto                     roomMap = day15_0(dataFile, k2Oxy);

    using KT = decltype(roomMap)::Key_Type;

    roomMap.get(k2Oxy) = 'T';
    KT evaluated{0, 0};

    incom::commons::doubleBuffer<std::queue<KT>> doubleQue;
    doubleQue.getCurrent().push(k2Oxy);
    std::vector<KT> dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    long long counter = 0;
    while (true) {
        if (doubleQue.getCurrent().empty()) {
            doubleQue.swapBuffers();
            counter++;
            if (doubleQue.getCurrent().empty()) { break; }
        }

        for (auto &oneDir : dirs) {
            evaluated          = doubleQue.getCurrent().front();
            evaluated.front() += oneDir.front();
            evaluated.back()  += oneDir.back();
            char &thisPos      = roomMap.get(evaluated);
            if (thisPos == '.') {
                doubleQue.getNext().push(evaluated);
                thisPos = 'E';
            }
        }
        doubleQue.getCurrent().pop();
    }

    return counter - 1;
}

} // namespace AOC2019