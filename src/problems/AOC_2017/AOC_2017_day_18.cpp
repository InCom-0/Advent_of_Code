#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"


namespace AOC2017 {

    /*
    Very much simplified 'data setup' logic for these types of problems
    Much simpler to write.

    Complexity of manual writing of the conditions is cut.
    It is probably also more efficient as virtually all of the data is stored in 1 vector.
    */
    long long day18_1(std::string dataFile) {
        // CUSTOM LOCAL TYPES DEFINITIONS
        struct _instrBase {
            long long &source;
            long long &target;
        };
        struct snd : _instrBase {};
        struct set : _instrBase {};
        struct add : _instrBase {};
        struct mul : _instrBase {};
        struct mod : _instrBase {};
        struct rcv : _instrBase {};
        struct jgz : _instrBase {};

        auto getWord_ctre = ctre::search<R"([[:^blank:]]+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, getWord_ctre);

        // DATA PREP
        std::unordered_map<char, std::reference_wrapper<long long>, AOC_commons::XXH3Hasher> mapping;
        long long fakeRegister = LLONG_MIN;
        std::vector<long long> registers;
        registers.reserve(VofV.size() * 2);         // Must NEVER reallocate.

        for (auto &line: VofV) {
            for (long long i = 1; i < line.size(); ++i) {
                if (line[i].front() >= 'a' && line[i].front() <= 'z') {
                    // 'Named' registers are inserted long longo a vector named registers + UOmap is created to obtain the right reference later
                    if (not mapping.contains(line[i].front())) {
                        registers.push_back(0);
                        mapping.emplace(line[i].front(), registers.back());
                    }
                }
            }
        }

        std::vector<std::variant<snd, set, add, mul, mod, rcv, jgz>> instrVect;

        for (auto &line: VofV) {
            std::reference_wrapper<long long> first = fakeRegister;
            std::reference_wrapper<long long> second = fakeRegister;
            if (line.size() >= 2) {
                if (line[1].front() >= 'a' && line[1].front() <= 'z') first = mapping.at(line[1][0]);
                else {
                    // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register used only in that instruction
                    registers.push_back(std::stoi(line[1]));
                    first = registers.back();
                }
            }
            if (line.size() >= 3) {
                if (line[2].front() >= 'a' && line[2].front() <= 'z') second = mapping.at(line[2][0]);
                else {
                    // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register used only in that instruction
                    registers.push_back(std::stoi(line[2]));
                    second = registers.back();
                }
            }

            // Actually create the right instruction and push_back it long longo the instrVect.
            if (line.front() == "snd") {instrVect.push_back(snd{first,second});}
            else if (line.front() == "set") {instrVect.push_back(set{first,second});}
            else if (line.front() == "add") {instrVect.push_back(add{first,second});}
            else if (line.front() == "mul") {instrVect.push_back(mul{first,second});}
            else if (line.front() == "mod") {instrVect.push_back(mod{first,second});}
            else if (line.front() == "rcv") {instrVect.push_back(rcv{first,second});}
            else if (line.front() == "jgz") {instrVect.push_back(jgz{first,second});}
            else return LLONG_MIN;
        }

        // LAMBDA DEFINITIONS
        long long lastSoundPlayed = LLONG_MIN;
        long long curInstrID = 0;
        auto overload_obj = overloaded {
            [&] (const snd& a) {lastSoundPlayed = a.source;},
            [&] (const set& a) {a.source = a.target;},
            [&] (const add& a) {a.source += a.target;},
            [&] (const mul& a) {a.source *= a.target;},
            [&] (const mod& a) {a.source %= a.target;},
            [&] (const rcv& a) {if (a.source != 0) curInstrID = LLONG_MAX;},
            [&] (const jgz& a) {if (a.source > 0) curInstrID += (a.target - 1);},
        };

        // MAIN LOGIC
        while (curInstrID < instrVect.size()) {
            std::visit(overload_obj, instrVect[curInstrID++]);
        }

        return lastSoundPlayed;
    }

    long long day18_2(std::string dataFile, int numOfPrograms) {
        // CUSTOM LOCAL TYPES DEFINITIONS
        struct _instrBase {
            long long &source;
            long long &target;
        };
        struct snd : _instrBase {};
        struct set : _instrBase {};
        struct add : _instrBase {};
        struct mul : _instrBase {};
        struct mod : _instrBase {};
        struct rcv : _instrBase {};
        struct jgz : _instrBase {};

        auto getWord_ctre = ctre::search<R"([[:^blank:]]+)">;
        auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, getWord_ctre);

        struct prog {
            long long fakeRegister = LLONG_MIN;         // Used for instructions that only have one part
            std::vector<long long> registers;
            std::vector<std::variant<snd, set, add, mul, mod, rcv, jgz>> instrVect;

            // The one and only constructor of the 'prog' type
            prog (decltype(VofV) &input,  long long programID) {
                std::unordered_map<char, std::reference_wrapper<long long>, AOC_commons::XXH3Hasher> mapping;
                registers.reserve(input.size() * 2);             // Must NEVER reallocate.

                for (auto &line: input) {
                    for (long long i = 1; i < line.size(); ++i) {
                        if (line[i].front() >= 'a' && line[i].front() <= 'z') {
                            // 'Named' registers are inserted long longo a vector named registers + UOmap is created to obtain the right reference later
                            if (not mapping.contains(line[i].front())) {
                                if (line[i].front() == 'p') registers.push_back(programID);
                                else registers.push_back(0);
                                mapping.emplace(line[i].front(), registers.back());
                            }
                        }
                    }
                }

                for (auto &line: input) {
                    std::reference_wrapper<long long> first = fakeRegister;
                    std::reference_wrapper<long long> second = fakeRegister;
                    if (line.size() >= 2) {
                        if (line[1].front() >= 'a' && line[1].front() <= 'z') first = mapping.at(line[1][0]);
                        else {
                            // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register used only in that instruction
                            registers.push_back(std::stoi(line[1]));
                            first = registers.back();
                        }
                    }
                    if (line.size() >= 3) {
                        if (line[2].front() >= 'a' && line[2].front() <= 'z') second = mapping.at(line[2][0]);
                        else {
                            // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register used only in that instruction
                            registers.push_back(std::stoi(line[2]));
                            second = registers.back();
                        }
                    }

                    // Actually create the right instruction and push_back it into the instrVect.
                    if (line.front() == "snd") {instrVect.push_back(snd{first,second});}
                    else if (line.front() == "set") {instrVect.push_back(set{first,second});}
                    else if (line.front() == "add") {instrVect.push_back(add{first,second});}
                    else if (line.front() == "mul") {instrVect.push_back(mul{first,second});}
                    else if (line.front() == "mod") {instrVect.push_back(mod{first,second});}
                    else if (line.front() == "rcv") {instrVect.push_back(rcv{first,second});}
                    else if (line.front() == "jgz") {instrVect.push_back(jgz{first,second});}
                }
            }
        };

        // DATA PREP
        std::vector<prog> programs;
        for (int i = 0; i < numOfPrograms; ++i) programs.emplace_back(VofV, i);

        std::vector<long long> curInstrIDs (numOfPrograms, 0);
        std::vector<unsigned short> switch_Xs (numOfPrograms, false);

        std::vector<long long> senderCounter (numOfPrograms, 0);
        std::vector<std::queue<long long>> queues (numOfPrograms, std::queue<long long>());

        // LAMBDA DEFINITIONS
        int progID = 0;
        bool queuesChanged = true;
        auto overload_obj = overloaded {    // If the 'snd' instruction specified ID of where to send the data, it would be possible to use arbitrary num of programs.
                                            // In this case ... it is just switching to 'the other one' 0 or 1;
            [&] (const snd& a) {queues[(progID-1)*(progID-1)].push(a.source); senderCounter[progID]++; queuesChanged = true;},
            [&] (const set& a) {a.source = a.target;},
            [&] (const add& a) {a.source += a.target;},
            [&] (const mul& a) {a.source *= a.target;},
            [&] (const mod& a) {a.source %= a.target;},
            [&] (const rcv& a) {
                                                if (queues[progID].empty()) {
                                                    switch_Xs[progID] = true;
                                                    curInstrIDs[progID]--;
                                                }
                                                else {
                                                    a.source = queues[progID].front();
                                                    queues[progID].pop();
                                                    queuesChanged = true;
                                                }
                                            },
            [&] (const jgz& a) {if (a.source > 0) curInstrIDs[progID] += (a.target - 1);  switch_Xs[progID] = false;},
        };

        // MAIN LOGIC
        while (queuesChanged) {                                             // If no queue changed it must be a deadlock
            queuesChanged = false;
            for (progID = 0; progID < programs.size(); ++progID) {
                while (curInstrIDs[progID] < programs[progID].instrVect.size() && switch_Xs[progID] == false) {
                    std::visit(overload_obj, programs[progID].instrVect[curInstrIDs[progID]++]);
                }
            }
            std::ranges::fill(switch_Xs, false);
        }

        return senderCounter[1];
    }

}