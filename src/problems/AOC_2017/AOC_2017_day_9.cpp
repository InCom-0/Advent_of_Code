#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"


namespace AOC2017 {
    int day9_1(std::string dataFile) {

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string input;
        std::getline(iStream, input);

        int groupNesting = 0;
        bool inGarbage = false;
        bool cancel = false;
        int accu = 0;

        for (auto &chr: input) {
            if (inGarbage) {
                if (cancel) {cancel = false;}
                else if (chr == '!') {cancel = true;}
                else if (chr == '>') {inGarbage = false;}
                continue;
            }
            if (chr == '{')  groupNesting++;
            else if (chr == '}') accu += groupNesting--;
            else if (chr == '<') inGarbage = true;
        }
        return accu;
    }
    int day9_2(std::string dataFile) {

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string input;
        std::getline(iStream, input);

        int groupNesting = 0;
        bool inGarbage = false;
        bool cancel = false;
        int accu = 0;
        int removedChars = 0;

        for (auto &chr: input) {
            if (inGarbage) {
                if (cancel) {cancel = false;}
                else if (chr == '!') {cancel = true;}
                else if (chr == '>') {inGarbage = false;}
                else removedChars++;
                continue;
            }
            if (chr == '{')  groupNesting++;
            else if (chr == '}') accu += groupNesting--;
            else if (chr == '<') inGarbage = true;
        }
        return removedChars;
    }

}