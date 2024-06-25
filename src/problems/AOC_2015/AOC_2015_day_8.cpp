#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day8_1(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        int curRawSize;
        int ansSize = 0;
        int literalSize = 0;
        std::string oneStr;

        while (std::getline(iStream, oneStr)) {
            auto bg = oneStr.begin();
            literalSize += oneStr.size();
            curRawSize = oneStr.size() - 2;

            while (auto re = ctre::search<R"(\\\\|\\"|\\x[[:xdigit:]][[:xdigit:]])">(bg, oneStr.end())) {
                if (re.to_string() == R"(\\)" || re.to_string() == R"(\")") curRawSize--;
                else curRawSize -= 3;
                bg = re.get_end_position();
            }
            ansSize += curRawSize;
        }

        return abs(literalSize - ansSize);
    }

    int day8_2(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        int curRawSize;
        int ansSize = 0;
        int literalSize = 0;
        std::string oneStr;

        while (std::getline(iStream, oneStr)) {
            auto bg = oneStr.begin();
            literalSize += oneStr.size();
            curRawSize = oneStr.size() + 2;

            while (auto re = ctre::search<R"(\\|")">(bg, oneStr.end())) {
                //            if (re == R"(\\)" || re == R"(\")") curRawSize--;
                //            else curRawSize -= 3;
                curRawSize++;
                bg = re.get_end_position();
            }
            ansSize += curRawSize;
        }

        return abs(literalSize - ansSize);
    }
}