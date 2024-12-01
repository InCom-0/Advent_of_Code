#include <ankerl/unordered_dense.h>
#include <array>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/format.h>
#include <incom_commons.h>
#include <optional>
#include <string>
#include <utility>


namespace AOC2019 {


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

    size_t accu = 0;
    for (int row = 1; row < (height - 1); ++row) {
        for (int col = 1; col < (length - 1); ++col) {
            if (scafMap[row][col] == '#' && scafMap[row - 1][col] == '#' && scafMap[row][col + 1] == '#' &&
                scafMap[row + 1][col] == '#' && scafMap[row][col - 1] == '#') {
                accu += (row - 1) * (col - 1);
            }
        }
    }
    for (auto &line : scafMap) { fmt::print("{}\n", fmt::join(line, "")); }
    return accu;
}

long long day17_2(std::string dataFile) {

    auto                      scafMap = day17_0(dataFile);
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

    std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::array<char, 3>               dirMap{'L', '_', 'R'};

    // Get the instruction in one-piece
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

    using Robo_Instr         = std::pair<char, int>;
    auto robo_instr_2_string = [&](Robo_Instr const &ri) {
        std::string res;
        res.push_back(ri.first);
        res.push_back(',');
        for (auto &chr : std::to_string(ri.second)) {
            res.push_back(chr);
            res.push_back(',');
        }
        return res;
    };
    std::vector<Robo_Instr> onePieceInstructions;

    auto buildInstructions = [&]() -> bool {
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

    auto build_sequenceMap = [&](size_t const minSize2consider) -> auto {
        std::vector dp(onePieceInstructions.size() + 1, std::vector(onePieceInstructions.size() + 1, 0));
        ankerl::unordered_dense::map<std::vector<Robo_Instr>,
                                     ankerl::unordered_dense::set<size_t, incom::commons::XXH3Hasher>,
                                     incom::commons::XXH3Hasher>
            mapToBuild;

        for (int i = onePieceInstructions.size() - 1; i >= 0; --i) {
            for (int j = onePieceInstructions.size() - 1; j > i; --j) {

                if (onePieceInstructions[i] == onePieceInstructions[j]) {
                    dp[i][j] = 1 + std::min(dp[i + 1][j + 1], j - i - 1);
                    for (int add = dp[i][j]; add >= minSize2consider; --add) {
                        std::vector<Robo_Instr> tmpVect(onePieceInstructions.begin() + i,
                                                        onePieceInstructions.begin() + i + add);

                        size_t tmpVectSZ = 0;
                        for (auto const &pr : tmpVect) { tmpVectSZ += 4 + 2*(pr.second > 9); }

                        if ((tmpVectSZ - 1) < 21) {
                            mapToBuild[tmpVect].emplace(i);
                            mapToBuild[tmpVect].emplace(j);
                        }
                    }
                }
            }
        }
        return mapToBuild;
    };
    auto mp_subseq_2_ids = build_sequenceMap(2);

    auto solver = [](decltype(mp_subseq_2_ids) const &mp_subseq_2_ids, size_t onePieceLength,
                     size_t maxNumOfUniqueSubseq) -> std::optional<std::vector<std::vector<Robo_Instr>>> {
        ankerl::unordered_dense::map<size_t, std::vector<std::vector<Robo_Instr>>, incom::commons::XXH3Hasher>
            mp_pos_2_subseq;

        for (auto const &mpItem : mp_subseq_2_ids) {
            for (auto const &posItem : mpItem.second) {
                mp_pos_2_subseq.insert({posItem, std::vector<std::vector<Robo_Instr>>()});
                mp_pos_2_subseq[posItem].push_back(mpItem.first);
            }
        }
        size_t                                                                                    curHead = 0;
        ankerl::unordered_dense::map<std::vector<Robo_Instr>, size_t, incom::commons::XXH3Hasher> selTracking;

        // Recursive solver.
        // Explores in a DFS manner all the possible arrangements of subsequences from the beginning
        // Respects how many different subsequences can be used (therefore short circuits on most of the unsuitable
        // parts of the tree)
        auto __inside_solver = [&](this auto &self) -> std::optional<std::vector<std::vector<Robo_Instr>>> {
            for (auto const &selOption : mp_pos_2_subseq[curHead]) {

                if (selTracking.contains(selOption)) { selTracking.at(selOption)++; }
                else if (selTracking.size() < maxNumOfUniqueSubseq) { selTracking.emplace(selOption, 1); }
                else { continue; }

                curHead += selOption.size();
                if (curHead == onePieceLength) { return std::vector(1, selOption); }

                // Recursive call
                std::optional<std::vector<std::vector<Robo_Instr>>> res = self();
                if (res != std::nullopt) {
                    res.value().push_back(selOption);
                    return res.value();
                }

                curHead -= selOption.size();

                selTracking.at(selOption)--;
                if (selTracking.at(selOption) == 0) { selTracking.erase(selOption); }
            }
            return std::nullopt;
        };

        auto res = __inside_solver();
        return res.and_then([](auto &val) -> std::optional<std::vector<std::vector<Robo_Instr>>> {
            std::ranges::reverse(val);
            return val;
        });
    };

    auto solverRes = solver(mp_subseq_2_ids, onePieceInstructions.size(), 3).value();

    ankerl::unordered_dense::map<std::vector<Robo_Instr>, char, incom::commons::XXH3Hasher> mp_subseq_2_charName;
    for (char seqChar = 'A'; auto &resLine : solverRes) {
        if (mp_subseq_2_charName.emplace(resLine, seqChar).second) { seqChar++; }
    }


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
        [&](out &a) {
            dustCollected = a.m_refs[0].get();
            fmt::print("{}", static_cast<char>(a.m_refs[0].get()));
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
    return dustCollected;
}
} // namespace AOC2019