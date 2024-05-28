#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <md5.h>
#include <glaze/binary/header.hpp>


namespace AOC2016 {
    std::vector<int> day23_1(std::string dataFile, int regA, int regB, int regC, int regD) {
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

        std::vector<std::variant<cpy, cpyInput, inc, dec, jnz, jnzInput, jnzOffsetTarget, jnzCondOffsetTarget, tgl, skip1, skip2>> instrVect;

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
        if (not iStream.is_open()) return std::vector<int>();
        std::string oneStr;
        std::string tmpStr;

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

            else {return std::vector<int>();}
        }

        int togleInstrID = 0;
        int curInstrID = 0;

        auto toggleOverload  = overloaded {
            [&] (const cpy& a) {instrVect[togleInstrID] = jnzCondOffsetTarget{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const cpyInput& a) {instrVect[togleInstrID] = jnzOffsetTarget{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const inc& a) {instrVect[togleInstrID] = dec{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const dec& a) {instrVect[togleInstrID] = inc{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const jnz& a) {instrVect[togleInstrID] = skip1{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const jnzInput& a) {instrVect[togleInstrID] = skip2{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const jnzOffsetTarget& a) {instrVect[togleInstrID] = cpyInput{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const jnzCondOffsetTarget& a) {instrVect[togleInstrID] = cpy{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const tgl& a) {instrVect[togleInstrID] = inc{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const skip1& a) {instrVect[togleInstrID] = jnz{a.valInput_1, a.valInput_2, a.source, a.target};},
            [&] (const skip2& a) {instrVect[togleInstrID] = jnzInput{a.valInput_1, a.valInput_2, a.source, a.target};},
            };

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
                                    [&] (const tgl& a) {
                                                                    togleInstrID = curInstrID + (*a.target);
                                                                    if (togleInstrID >=0 && togleInstrID < instrVect.size()) {
                                                                        std::visit(toggleOverload, instrVect[togleInstrID]);
                                                                    }
                                                                    curInstrID++;
                                                                },
                                    [&] (const skip1& a) {curInstrID++;},
                                    [&] (const skip2& a) {curInstrID++;},
                                }, instrVect[curInstrID]);
        }
        return std::vector<int>{regA,regB,regC,regD};

    }

}
