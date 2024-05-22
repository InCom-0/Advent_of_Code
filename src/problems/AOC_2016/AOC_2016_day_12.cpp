#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"

#include "AOC_2016_day_12.h"


namespace AOC2016 {

std::vector<int> day12_1(std::string dataFile, int regA, int regB, int regC, int regD) {
    struct cpy {
        int* source;
        int* target;
    };
    struct cpyInput {
        int valInput;
        int* target;
    };
    struct inc {
        int* target;
    };
    struct dec {
        int* target;
    };
    struct jnz {
        int* conditionTarget;
        int offset;
    };
    struct jnzInput {
        int valInput;
        int offset;
    };

    std::vector<std::variant<cpy,cpyInput,inc,dec,jnz,jnzInput>> instrVect;

    std::unordered_map<char, int*> mapping;
    mapping['a'] = &regA;
    mapping['b'] = &regB;
    mapping['c'] = &regC;
    mapping['d'] = &regD;

    auto getWord_ctre = ctre::search<R"([a-z]+)">;
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
                instrVect.push_back(cpyInput(re.to_number(),
                                        mapping.at(getWord_ctre(re.get_end_position(), oneStr.end()).to_string()[0])));
            }
            else {
                re = getWord_ctre(bg, oneStr.end());
                instrVect.push_back(cpy(mapping.at(re.to_string()[0]),
                                        mapping.at(getWord_ctre(re.get_end_position(), oneStr.end()).to_string()[0])));
            }
        }
        else if (tmpStr == "dec") {
            instrVect.push_back(dec(mapping.at(getWord_ctre(bg, oneStr.end()).to_string()[0])));
        }
        else if (tmpStr == "inc") {
            instrVect.push_back(inc(mapping.at(getWord_ctre(bg, oneStr.end()).to_string()[0])));
        }
        else if (tmpStr == "jnz") {
            re = getWord_ctre(bg, oneStr.end());
            if (re) {
                instrVect.push_back(jnz(mapping.at(re.to_string()[0]),
                                                        getNumber_ctre(re.get_end_position(), oneStr.end()).to_number()));
            }
            else {
                re = getNumber_ctre(bg, oneStr.end());
                instrVect.push_back(jnzInput(re.to_number(),
                                                        getNumber_ctre(re.get_end_position(), oneStr.end()).to_number()));
            }
        }
        else {return std::vector<int>();}
    }

    int curInstrID = 0;
    while (curInstrID < instrVect.size()) {
        std::visit(overloaded {
                                [&] (const cpy& a) {*a.target = *a.source; curInstrID++;},
                                [&] (const cpyInput& a) {*a.target = a.valInput; curInstrID++;},
                                [&] (const inc& a) {(*a.target)++; curInstrID++;},
                                [&] (const dec& a) {(*a.target)--; curInstrID++;},
                                [&] (const jnz& a) {curInstrID = (*a.conditionTarget != 0 ? curInstrID+a.offset : curInstrID+1);},
                                [&] (const jnzInput& a) {curInstrID = (a.valInput != 0 ? curInstrID+a.offset : curInstrID+1);},
                            }, instrVect[curInstrID]);
    }
    return std::vector<int>{regA,regB,regC,regD};
}

}