
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <omni.h>


namespace AOC2019 {
long long day11_1(std::string dataFile) {
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

    incom::omni::MD_ChunkVector<OnePanel, 2, OnePanel{}, 8> panels;
    using KT = decltype(panels)::Key_Type;

    KT curKey{0, 0};
    struct Dir {
        unsigned int dir : 2;
    } dir;

    dir.dir = 0;

    std::vector<KT> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);

    long long panelAccessedCount = 0;
    auto      instructions       = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) { a.m_refs[0].get() = panels.get(curKey).pan; },
        [&](out &a) {
            if (bitSwitch == bit::bit0) {
                auto &pn            = panels.get(curKey);

                panelAccessedCount += (pn.track);
                pn.track = false;
                pn.pan   = a.m_refs[0].get();

                bitSwitch.flip();
            }
            else if (bitSwitch == bit::bit1) {
                dir.dir   += ((a.m_refs[0].get() * 2) - 1);
                curKey[0] += dirs[dir.dir][0];
                curKey[1] += dirs[dir.dir][1];

                bitSwitch.flip();
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

    long long counter = 0;
    while (pqa.is_cursorValid()) {
        pqa.exe_pointedToInstr(instructions);
        counter++;
    }

    std::cout << "out says: " << counter << '\n';
    return panelAccessedCount;
}

long long day11_2(std::string dataFile) {
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

    incom::omni::MD_ChunkVector<OnePanel, 2, OnePanel{}, 8> panels;
    using KT = decltype(panels)::Key_Type;
    KT curKey{0, 0};
    panels.get(curKey).pan = true;

    struct Dir {
        unsigned int dir : 2 = 0;
    } dir;

    std::vector<KT> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);


    long long panelAccessedCount = 0;
    auto      instructions       = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) { a.m_refs[0].get() = panels.get(curKey).pan; },
        [&](out &a) {
            if (bitSwitch == bit::bit0) {
                panelAccessedCount       += (panels.get(curKey).track);
                panels.get(curKey).track  = false;
                panels.get(curKey).pan    = a.m_refs[0].get();

                yMin = std::min(yMin, curKey[0]);
                yMax = std::max(yMax, curKey[0]);
                xMin = std::min(xMin, curKey[1]);
                xMax = std::max(xMax, curKey[1]);

                bitSwitch.flip();
            }
            else if (bitSwitch == bit::bit1) {
                dir.dir   += ((a.m_refs[0].get() * 2) - 1);
                curKey[0] += dirs[dir.dir][0];
                curKey[1] += dirs[dir.dir][1];

                bitSwitch.flip();
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

    for (long long y = yMin; y <= yMax; ++y) {
        for (long long x = xMin; x <= xMax; ++x) {
            bool pn = panels.get({y, x}).pan;
            std::cout << (pn ? '#' : ' ');
        }
        std::cout << '\n';
    }

    return panelAccessedCount;
}

} // namespace AOC2019