#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {


int day23_1(std::string dataFile, unsigned int regA, unsigned int regB) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return -1;

    struct tpl {
        unsigned int* target;
    };
    struct hlf {
        unsigned int* target;
    };
    struct inc {
        unsigned int* target;
    };
    struct jmp {
        int offset;
    };
    struct jio {
        unsigned int* conditionTarget;
        int offset;
    };
    struct jie {
        unsigned int* conditionTarget;
        int offset;
    };

    std::string oneStr;
    std::string tmpStr;

    std::vector<std::variant<tpl,hlf,inc,jmp,jio,jie>> instrList;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
        bg = re.get_end_position();

        tmpStr = re.to_string();
        if (tmpStr == "tpl") {
            if (ctre::search<R"(\w+)">(bg, oneStr.end()).to_string() == "a") instrList.push_back(tpl(&regA));
            else instrList.push_back(tpl(&regB));
        }
        else if (tmpStr == "hlf") {
            if (ctre::search<R"(\w+)">(bg, oneStr.end()).to_string() == "a") instrList.push_back(hlf(&regA));
            else instrList.push_back(hlf(&regB));
        }
        else if (tmpStr == "inc") {
            if (ctre::search<R"(\w+)">(bg, oneStr.end()).to_string() == "a") instrList.push_back(inc(&regA));
            else instrList.push_back(inc(&regB));
        }
        else if (tmpStr == "jmp") {
            tmpStr = ctre::search<R"(.\d+)">(bg, oneStr.end()).to_string();
            instrList.push_back(jmp(stoi(tmpStr)));
        }
        else if (tmpStr == "jio") {
            re = ctre::search<R"(\w+)">(bg, oneStr.end());
            tmpStr = re.to_string();
            bg = re.get_end_position();
            re = ctre::search<R"(.\d+)">(bg, oneStr.end());

            if (tmpStr == "a") instrList.emplace_back(jio(&regA, std::stoi(re.to_string())));
            else instrList.push_back(jio(&regB, std::stoi(re.to_string())));
        }
        else if (tmpStr == "jie") {
            re = ctre::search<R"(\w+)">(bg, oneStr.end());
            tmpStr = re.to_string();
            bg = re.get_end_position();
            re = ctre::search<R"(.\d+)">(bg, oneStr.end());

            if (tmpStr == "a") instrList.push_back(jie(&regA, std::stoi(re.to_string())));
            else instrList.push_back(jie(&regB, std::stoi(re.to_string())));
        }
        else {return -999;}
    }

    int curInstrID = 0;
    while (curInstrID < instrList.size()) {
        std::visit(overloaded {
                                [&] (tpl& a) {*a.target *= 3; curInstrID++;},
                                [&] (hlf& a) {*a.target /= 2; curInstrID++;},
                                [&] (inc& a) {(*a.target)++; curInstrID++;},
                                [&] (jmp& a) {curInstrID += a.offset;},
                                [&] (jio& a) {curInstrID = (*a.conditionTarget == 1 ? curInstrID+a.offset : curInstrID+1);},
                                [&] (jie& a) {curInstrID = (*a.conditionTarget % 2 == 0 ? curInstrID+a.offset : curInstrID+1);},
                            }, instrList[curInstrID]);
    }
    return regB;
}
}