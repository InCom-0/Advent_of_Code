#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/format.h>
#include <optional>

#include <incom_commons.h>
#include <incom_seq.h>


namespace AOC2019 {

// Helper function to 'build the maze' which is non-trivial and reused for both parts
std::vector<std::vector<char>> day17_0(std::string dataFile) {
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

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);

    std::vector<std::vector<char>> scafMap;
    scafMap.push_back(std::vector<char>(1, '.'));
    scafMap.push_back(std::vector<char>(1, '.'));


    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) {},
        [&](out &a) {
            if (a.m_refs[0].get() == 35) { scafMap.back().push_back('#'); }
            else if (a.m_refs[0].get() == 46) { scafMap.back().push_back('.'); }
            else if (a.m_refs[0].get() == 10) {
                scafMap.back().push_back('.');
                scafMap.push_back(std::vector<char>(1, '.'));
            }
            else if (a.m_refs[0].get() == 88) { scafMap.back().push_back('X'); }
            else if (a.m_refs[0].get() == 60) { scafMap.back().push_back('<'); }
            else if (a.m_refs[0].get() == 62) { scafMap.back().push_back('>'); }
            else if (a.m_refs[0].get() == 86) { scafMap.back().push_back('v'); }
            else if (a.m_refs[0].get() == 94) { scafMap.back().push_back('^'); }
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

    scafMap.front() = std::vector<char>(scafMap.at(1).size(), '.');
    scafMap.pop_back();
    scafMap.back() = std::vector<char>(scafMap.at(1).size(), '.');
    return scafMap;
}

long long day17_1(std::string dataFile) {

    auto   scafMap = day17_0(dataFile);
    size_t height  = scafMap.size();
    size_t length  = scafMap.at(1).size();

    for (auto &line : scafMap) {
        for (auto &chr : line) {
            if (chr > 46) { chr = '#'; }
        }
    }
    // Simple 'lookaround' to find if '#' is on all 4 sides thereby indication a 'junction'
    size_t accu = 0;
    for (int row = 1; row < (height - 1); ++row) {
        for (int col = 1; col < (length - 1); ++col) {
            if (scafMap[row][col] == '#' && scafMap[row - 1][col] == '#' && scafMap[row][col + 1] == '#' &&
                scafMap[row + 1][col] == '#' && scafMap[row][col - 1] == '#') {
                accu += (row - 1) * (col - 1);
            }
        }
    }
    return accu;
}


long long day17_2(std::string dataFile) {

    // Get the map of the maze
    auto scafMap = day17_0(dataFile);

    // Find the rover location and its direction
    std::pair<size_t, size_t> rovLoc;
    struct Dir {
        unsigned int dir : 2;
    } rovDir;

    for (int row = 0; row < scafMap.size(); ++row) {
        for (int col = 0; col < scafMap.at(row).size(); ++col) {
            if (scafMap[row][col] == '<') { rovDir.dir = 3; }
            else if (scafMap[row][col] == '>') { rovDir.dir = 1; }
            else if (scafMap[row][col] == 'v') { rovDir.dir = 2; }
            else if (scafMap[row][col] == '^') { rovDir.dir = 0; }
            else { continue; }
            rovLoc = std::make_pair(row, col);
        }
    }

    // Helper variables to deal with turning directions in other functions
    constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    constexpr std::array<char, 3> const               dirMap{'L', '_', 'R'};

    // Simple type for storing the instructions + its helper
    using Robo_Instr         = std::pair<char, int>;
    auto robo_instr_2_string = [&](Robo_Instr const &ri) {
        std::string res;
        res.push_back(ri.first);
        res.push_back(',');
        res.append(std::to_string(ri.second));
        res.push_back(',');
        return res;
    };

    // 'Explore' the maze and build the 'vector of instructions' along the way
    std::vector<Robo_Instr> onePieceInstructions;
    auto                    buildInstructions = [&]() -> bool {
        // Find out whether to turn left or right (or end of maze)
        auto getTurnDir = [&]() -> int {
            rovDir.dir--;
            if (scafMap[rovLoc.first + dirs[rovDir.dir].front()][rovLoc.second + dirs[rovDir.dir].back()] == '#') {
                rovDir.dir++;
                return -1;
            }
            rovDir.dir += 2;
            if (scafMap[rovLoc.first + dirs[rovDir.dir].front()][rovLoc.second + dirs[rovDir.dir].back()] == '#') {
                rovDir.dir--;
                return 1;
            }
            else { return 0; }
        };
        int newDir = getTurnDir();
        if (newDir == 0) { return false; }
        rovDir.dir += newDir;

        int steps = 0;
        while (scafMap[rovLoc.first + dirs[rovDir.dir].front()][rovLoc.second + dirs[rovDir.dir].back()] == '#') {
            rovLoc.first  += dirs[rovDir.dir].front();
            rovLoc.second += dirs[rovDir.dir].back();
            steps++;
        }
        onePieceInstructions.push_back({dirMap.at(newDir + 1), steps});
        return true;
    };

    while (buildInstructions()) {}

    // Filter for subsequences that are unuseable (in this case of length more than 20 chars)
    auto filter_forUnusableSubseq = [](auto const &subSeq) -> bool {
        size_t accu = 0;
        for (auto &instr : subSeq) { accu += (3 + std::to_string(instr.second).size()); }
        return (accu - 1) < 21;
    };

    // MAIN PART - Calling the dedicated but sufficiently generic solver
    auto solverRes =
        incom::seq::solve_seqFromRepUniqueSubseq(onePieceInstructions, 2, 3, filter_forUnusableSubseq).value();

    // Create a map of identifiers ('A', 'B', 'C', ... etc.) for the selected subsequences
    ankerl::unordered_dense::map<std::vector<Robo_Instr>, char, incom::commons::XXH3Hasher> mp_subseq_2_charName;
    for (char seqChar = 'A'; auto &resLine : solverRes) {
        if (mp_subseq_2_charName.emplace(resLine, seqChar).second) { seqChar++; }
    }

    // Build the character sequence for input
    std::string inputSequence;
    for (auto const &seq : solverRes) {
        inputSequence.push_back(mp_subseq_2_charName.at(seq));
        inputSequence.push_back(',');
    }
    inputSequence.back() = '\n';

    for (auto const &subseq : mp_subseq_2_charName) {
        for (auto const &ri : subseq.first) { inputSequence.append(robo_instr_2_string(ri)); }
        inputSequence.back() = '\n';
    }
    inputSequence.push_back('n');
    inputSequence.push_back('\n');

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
    data.front() = 2;
    std::vector<long long> instrCodes{1, 2, 99, 3, 4, 5, 6, 7, 8, 9};

    incom::commons::PQA::ProgramQuasiAssembly_INT<add, mul, end, inp, out, jit, jif, lth, eql, arb> pqa(instrCodes,
                                                                                                        data);

    size_t inpCursor     = 0;
    size_t dustCollected = 0;

    auto instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) { a.m_refs[0].get() = inputSequence.at(inpCursor++); },
        [&](out &a) { dustCollected = a.m_refs[0].get(); },
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
    return dustCollected;
}
} // namespace AOC2019