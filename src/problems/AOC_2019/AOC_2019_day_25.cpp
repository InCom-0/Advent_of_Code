#include <bitset>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>
#include <optional>


namespace AOC2019 {
std::string day25_1(std::string dataFile) {
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

    std::vector<std::string>           droidOutput(1, std::string());
    std::vector<decltype(droidOutput)> pastDroidOutputs;

    static constexpr std::array<std::string, 4> const dirs_str{"north", "east", "south", "west"};


    struct Room {
        std::bitset<4>           m_doors;
        std::vector<std::string> m_items;

        std::string roomName;
        std::string description;
    };

    struct Command {
        std::string commandString;
        std::size_t curPos = 0;
    };


    struct Droid {
        std::vector<std::string>      badItems;
        std::vector<std::vector<int>> m_mazeVisited = std::vector(32, std::vector(32, 0));
        std::vector<std::string>      m_carrying;
        std::vector<size_t>           m_routeFromStart;

        std::array<int, 2> m_droidPos{16, 16};
        Command            m_curCommand;
        Room               m_curRoom;

        void parse_roomRawInfo(std::vector<std::string> const &rawInfo) {
            static constexpr std::array<std::string, 4> const dirs_str{"north", "east", "south", "west"};

            m_curRoom = Room();
            for (int lineID = 0; lineID < rawInfo.size(); ++lineID) {
                if (rawInfo[lineID].front() == '=') {
                    m_curRoom.roomName    = rawInfo[lineID++];
                    m_curRoom.description = rawInfo[lineID++];
                }
                else if (rawInfo[lineID] == "Doors here lead:") {

                    while (rawInfo[++lineID].front() == '-') {
                        m_curRoom.m_doors.set(std::ranges::find(dirs_str, rawInfo[lineID].substr(2)) - dirs_str.begin(),
                                              true);
                    }
                }

                else if (rawInfo[lineID] == "Items here:") {
                    while (rawInfo[++lineID].front() == '-') { m_curRoom.m_items.push_back(rawInfo[lineID].substr(2)); }
                }
            }
        }

        void move_outcomeOfEjected() {
            static constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
            m_droidPos[0] += dirs[(m_routeFromStart.back() + 2) % 4][0];
            m_droidPos[1] += dirs[(m_routeFromStart.back() + 2) % 4][1];
            m_routeFromStart.pop_back();
        }

        std::optional<int> _create_itemPickupCommand() {
            if (m_curRoom.m_items.empty()) { return std::nullopt; }
            else { return std::nullopt; }
        }
        std::optional<int> _create_itemPutdownCommand() { return std::nullopt; }

        std::optional<int> _create_goForthCommand() {
            static constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};
            static constexpr std::array<std::string, 4> const        dirs_str{"north", "east", "south", "west"};

            for (int i = 0; i < 4; ++i) {
                if (m_mazeVisited[m_droidPos[0] + dirs[i][0]][m_droidPos[1] + dirs[i][1]] == 0 &&
                    m_curRoom.m_doors.test(i)) {
                    // Found valid unexplored direction
                    m_curCommand.commandString = dirs_str[i];
                    m_curCommand.curPos        = 0;
                    m_routeFromStart.push_back(i);

                    m_droidPos[0]                               += dirs[i][0];
                    m_droidPos[1]                               += dirs[i][1];
                    m_mazeVisited[m_droidPos[0]][m_droidPos[1]]  = 1;
                    return std::optional<int>(3);
                }
            }
            return std::nullopt;
        }

        std::optional<int> _create_goBackCommand() {
            static constexpr std::array<std::array<int, 2>, 4> const dirs{-1, 0, 0, 1, 1, 0, 0, -1};

            // Going back ie. not found valid unexplored direction
            m_curCommand.commandString  = dirs_str.at((m_routeFromStart.back() + 2) % 4);
            m_curCommand.curPos         = 0;
            m_droidPos[0]              += dirs[(m_routeFromStart.back() + 2) % 4][0];
            m_droidPos[1]              += dirs[(m_routeFromStart.back() + 2) % 4][1];
            if (m_routeFromStart.empty()) { std::cout << "END"; }
            m_routeFromStart.pop_back();

            return std::optional<int>(4);
        }

        auto create_nextCommand() {
            auto const pickUp  = [&]() { return _create_itemPickupCommand(); };
            auto const putDown = [&]() { return _create_itemPutdownCommand(); };
            auto const goForth = [&]() { return _create_goForthCommand(); };
            auto const goBack  = [&]() { return _create_goBackCommand(); };

            return pickUp().or_else(putDown).or_else(goForth).or_else(goBack);
        }

        char get_nextCharInDecision() {
            if (m_curCommand.curPos == m_curCommand.commandString.size()) {
                m_curCommand.curPos = 0;
                return '\n';
            }
            else { return m_curCommand.commandString.at(m_curCommand.curPos++); }
        }
    };

    Droid droid;

    droid.m_mazeVisited[16][16] = 1;

    std::string_view const commandQuestionStr("Command?");
    std::string_view const ejectedStr("A loud, robotic voice says \"Alert! Droids on this ship are heavier than the "
                                      "detected value!\" and you are ejected back to the checkpoint.");

    long long outValue;
    auto      instructions = overloaded{
        [&](add &a) { a.m_refs[2].get() = a.m_refs[0] + a.m_refs[1]; },
        [&](mul &a) { a.m_refs[2].get() = a.m_refs[0] * a.m_refs[1]; },
        [&](end &a) { pqa.m_cursor = LLONG_MIN; },
        [&](inp &a) {
            if (droid.m_curCommand.curPos == 0) { droid.create_nextCommand(); }
            // No 'else' because we need to input char even if we were making a decision in this iteration

            a.m_refs[0].get() = droid.get_nextCharInDecision();

            std::cout << static_cast<char>(a.m_refs[0].get());
        },
        [&](out &a) {
            if (droidOutput.back() == commandQuestionStr) {
                pastDroidOutputs.push_back(droidOutput);
                droidOutput.clear();
                droid.parse_roomRawInfo(pastDroidOutputs.back());
            }
            else if (droidOutput.back() == ejectedStr) {
                pastDroidOutputs.push_back(droidOutput);
                droidOutput.clear();
                droid.move_outcomeOfEjected();
            }

            if (static_cast<char>(a.m_refs[0].get()) == '\n') { droidOutput.push_back(std::string()); }
            else { droidOutput.back().push_back(static_cast<char>(a.m_refs[0].get())); }

            std::cout << static_cast<char>(a.m_refs[0].get());

            if (droid.m_routeFromStart.empty() &&
                droid.m_mazeVisited[droid.m_droidPos.front()][droid.m_droidPos.back()]) {
                pqa.m_cursor = LLONG_MIN;
            }
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

    return "TEMP OUT";
}

std::string day25_2(std::string dataFile) {

    return "TEMP OUT";
}
} // namespace AOC2019
