#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <omni.h>


namespace AOC2019 {
long long day13_1(std::string dataFile) {
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

    auto bitSwitch = bit::bit0;
    struct OnePanel {
        bool pan   = false;
        bool track = true;
    };

    long long yMin = 0, yMax = 0, xMin = 0, xMax = 0;

    incom::omni::MD_ChunkVector<char, 2, 0, 8> screen;
    using KT = decltype(screen)::Key_Type;
    KT curKey{0, 0};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);

    int       outSwitch          = 0;
    auto      instructions       = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) {
            a.m_refs[0].get() = screen.get(curKey);
            assert(false);
        },
        [&](out &a) {
            if (outSwitch == 0) {
                curKey[0] = a.m_refs[0].get();
                outSwitch++;
            }
            else if (outSwitch == 1) {
                curKey[1] = a.m_refs[0].get();
                outSwitch++;
            }
            else if (outSwitch == 2) {
                screen.get(curKey) = a.m_refs[0].get();
                outSwitch          = 0;
            }

            else { assert(false); }

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

    while (pqa.is_cursorValid()) { pqa.exe_pointedToInstr(instructions); }

    long long countOfBlocks = 0;

    auto chunkMap = screen.get_mp();
    for (auto &chnk : chunkMap) {
        for (size_t i = 0; i < 64ll; ++i) { countOfBlocks += (chnk.second._get_dataItemBySizeT(i) == 2); }
    }

    return countOfBlocks;
}

long long day13_2(std::string dataFile) {
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
    // Set mem address 0 to '2' so that we can 'play for free' as instructed
    data[0] = 2;

    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8, 9};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);

    int                             outSwitch          = 0;
    long long                       curScore           = 0;
    std::pair<long long, long long> lastBallPos;
    std::pair<long long, long long> lastPaddlePos;
    std::pair<long long, long long> lastPos;

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) {
            if (lastBallPos.first < lastPaddlePos.first) { a.m_refs[0].get() = -1; }
            else if (lastBallPos.first > lastPaddlePos.first) { a.m_refs[0].get() = 1; }
            else { a.m_refs[0].get() = 0; }
        },
        [&](out &a) {
            if (outSwitch == 0) {
                lastPos.first = a.m_refs[0].get();
                outSwitch++;
            }
            else if (outSwitch == 1) {
                lastPos.second = a.m_refs[0].get();
                outSwitch++;
            }
            else if (outSwitch == 2) {
                long long tileID = a.m_refs[0].get();

                if (lastPos.first == -1 && lastPos.second == 0) { curScore = tileID; }
                else if (tileID == 4) {
                    lastBallPos.first  = lastPos.first;
                    lastBallPos.second = lastPos.second;
                }
                else if (tileID == 3) {
                    lastPaddlePos.first  = lastPos.first;
                    lastPaddlePos.second = lastPos.second;
                }
                outSwitch = 0;
            }

            else { assert(false); }

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

    while (pqa.is_cursorValid()) { pqa.exe_pointedToInstr(instructions); }

    return curScore;
}
} // namespace AOC2019