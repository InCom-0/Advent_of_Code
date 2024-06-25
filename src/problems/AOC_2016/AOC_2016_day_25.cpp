#include <fstream>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2016 {

    int day25_1(std::string dataFile) {
        struct _instrBase {
            int valInput_1 = INT_MIN;
            int valInput_2 = INT_MIN;
            int* source = nullptr;
            int* target = nullptr;
        };
        struct cpy : _instrBase {};
        struct cpyInput : _instrBase {};
        struct inc : _instrBase {};
        struct dec : _instrBase {};
        struct jnz : _instrBase {};
        struct jnzInput : _instrBase {};
        struct jnzOffsetTarget : _instrBase {};
        struct jnzCondOffsetTarget : _instrBase {};
        struct tgl : _instrBase {};
        struct skip1 : _instrBase {};
        struct skip2 : _instrBase {};
        struct out : _instrBase {};
        struct pls : _instrBase {};

        std::vector<std::variant<cpy, cpyInput, inc, dec, jnz, jnzInput, jnzOffsetTarget, jnzCondOffsetTarget, tgl, skip1, skip2, out, pls>> instrVect;
        int regA = 0, regB = 0, regC = 0, regD = 0;

        std::unordered_map<char, int*> mapping;
        mapping['a'] = &regA;
        mapping['b'] = &regB;
        mapping['c'] = &regC;
        mapping['d'] = &regD;

        auto getWord_ctre = ctre::search<R"([a-z]+)">;
        auto getFirstReg_ctre = ctre::search<R"((?<= )[a-z](?= ))">;
        auto getNumber_ctre = ctre::search<R"(-?\d+)">;

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return INT_MIN;
        std::string oneStr;
        std::string tmpStr;

        // GETTING THE DATA
        while (std::getline(iStream, oneStr)) {
            auto bg = oneStr.begin();
            auto re = getWord_ctre(bg, oneStr.end());
            bg = re.get_end_position();

            tmpStr = re.to_string();
            if (tmpStr == "cpy") {
                re = getNumber_ctre(bg, oneStr.end());
                if (re) {
                    instrVect.push_back(cpyInput{re.to_number(), INT_MIN,
                                                        nullptr, mapping.at(getWord_ctre(re.get_end_position(), oneStr.end()).to_string()[0])});
                }
                else {
                    re = getWord_ctre(bg, oneStr.end());
                    instrVect.push_back(cpy{INT_MIN, INT_MIN, mapping.at(re.to_string()[0]),
                                            mapping.at(getWord_ctre(re.get_end_position(), oneStr.end()).to_string()[0])});
                }
            }
            else if (tmpStr == "dec") {
                instrVect.push_back(dec{INT_MIN, INT_MIN, nullptr, mapping.at(getWord_ctre(bg, oneStr.end()).to_string()[0])});
            }
            else if (tmpStr == "inc") {
                instrVect.push_back(inc{INT_MIN, INT_MIN, nullptr, mapping.at(getWord_ctre(bg, oneStr.end()).to_string()[0])});
            }
            else if (tmpStr == "jnz") {
                auto re2 = getFirstReg_ctre(bg, oneStr.end());
                if (re2) {
                    re = getWord_ctre(re2.get_end_position(), oneStr.end());
                    if (re) instrVect.push_back(jnzCondOffsetTarget{INT_MIN, INT_MIN,
                                                            mapping.at(re2.to_string()[0]),mapping.at(re.to_string()[0])});
                    else instrVect.push_back(jnz{INT_MIN, getNumber_ctre(re2.get_end_position(), oneStr.end()).to_number(),
                                                            mapping.at(re2.to_string()[0]), nullptr});

                }
                else {
                    re = getNumber_ctre(bg, oneStr.end());
                    re2 = getWord_ctre(re.get_end_position(), oneStr.end());

                    if (re2) instrVect.push_back(jnzOffsetTarget{re.to_number(), INT_MIN,
                                                            nullptr, mapping.at(re2.to_string()[0])});
                    else instrVect.push_back(jnzInput{re.to_number(), getNumber_ctre(re.get_end_position(), oneStr.end()).to_number()});

                }
            }
            else if (tmpStr == "tgl") {
                re = getWord_ctre(bg, oneStr.end());
                instrVect.push_back(tgl{INT_MIN, INT_MIN, nullptr, mapping.at(re.to_string()[0])});
            }
            else if (tmpStr == "out") {
                re = getWord_ctre(bg, oneStr.end());
                instrVect.push_back(out{INT_MIN, INT_MIN, nullptr, mapping.at(re.to_string()[0])});
            }
            else if (tmpStr == "pls") {
                re = getNumber_ctre(bg, oneStr.end());
                instrVect.push_back(pls{re.to_number(), INT_MIN, nullptr, mapping.at(getWord_ctre(re.get_end_position(), oneStr.end()).to_string()[0])});
            }

            else return INT_MIN;
        }

        int inputIntoRegA = -1;
        bool foundRepeating = false;

        // MAIN LOOP OF INCREMENTING ATTEMPTS
        do {
            int curInstrID = 0;
            std::vector<std::vector<int>> regStateSnapshots;                //snapshot to be made when executing 'out' instruction
            inputIntoRegA++;
            regA = inputIntoRegA; regB = 0; regC = 0; regD = 0;

            // MAIN LOOP OF INSTRUCTIONS
            while (curInstrID < instrVect.size()) {
                std::visit(overloaded {
                                        [&] (const cpy& a) {*a.target = *a.source; curInstrID++;},
                                        [&] (const cpyInput& a) {*a.target = a.valInput_1; curInstrID++;},
                                        [&] (const inc& a) {(*a.target)++; curInstrID++;},
                                        [&] (const dec& a) {(*a.target)--; curInstrID++;},
                                        [&] (const jnz& a) {curInstrID = (*a.source != 0 ? curInstrID+a.valInput_2 : curInstrID+1);},
                                        [&] (const jnzInput& a) {curInstrID = (a.valInput_1 != 0 ? curInstrID+a.valInput_2 : curInstrID+1);},
                                        [&] (const jnzOffsetTarget& a) {curInstrID = (a.valInput_1 != 0 ? curInstrID+(*a.target) : curInstrID+1);},
                                        [&] (const jnzCondOffsetTarget& a) {curInstrID = (*a.source != 0 ? curInstrID+(*a.target) : curInstrID+1);},
                                        [&] (const out& a) {
                                            regStateSnapshots.push_back(std::vector {regA, regB, regC, regD, (*a.target)});
                                            // std::cout << "regState:  " << inputIntoRegA << ' ' << regA << ' ' << regB << ' ' << regC << ' ' << regD << '\n';
                                            curInstrID++;
                                            if (regStateSnapshots.size() >= 2) {
                                                if (regStateSnapshots.rbegin()->back() == (regStateSnapshots.rbegin()+1)->back()) {
                                                    curInstrID = instrVect.size();
                                                    return;
                                                }
                                            }
                                            if (regStateSnapshots.rbegin()->front() == 0 && (regStateSnapshots.size() % 2 == 0)) {
                                                foundRepeating = true;
                                                curInstrID = instrVect.size();
                                            }
                                        },
                                        [&] (const pls& a) {*a.target = *a.target + a.valInput_1; curInstrID++;},
                                        [&] (const auto& a) ->void {curInstrID = instrVect.size();},
                                    }, instrVect[curInstrID]);
            }
        } while (not foundRepeating);
        return inputIntoRegA;
    }

}
