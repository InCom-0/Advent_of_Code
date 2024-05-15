#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
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
            curRawSize = oneStr.size() + 2;

            while (auto re = ctre::search<R"(\\|")">(bg, oneStr.end())) {
                //            if (re == R"(\\)" || re == R"(\")") curRawSize--;
                //            else curRawSize -= 3;
                std::cout << re << '\n';
                curRawSize++;
                std::cout << curRawSize << '\n';
                bg = re.get_end_position();
            }
            ansSize += curRawSize;
        }

        return literalSize - ansSize;
    }
}