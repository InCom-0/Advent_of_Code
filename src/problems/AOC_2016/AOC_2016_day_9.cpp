#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"

#include "AOC_2016_day_9.h"



namespace AOC2016 {

int day9_1(std::string &&dataFile) {

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    std::getline(iStream, oneStr);

    auto findMarker = ctre::search<R"(\(\d+x\d+\))">;
    auto beforeX = ctre::search<R"(\d+(?=x))">;
    auto afterX = ctre::search<R"((?<=x)\d+)">;

    auto bg = oneStr.begin();
    auto end = oneStr.end();

    std::string newStr;

    auto rgxResult = findMarker(bg, end);

    while (rgxResult) {
        newStr.append(bg, rgxResult.begin());
        std::string marker = rgxResult.to_string();

        int numOfLetters = beforeX(marker.begin(),marker.end()).to_number();
        int multiple = afterX(marker.begin(),marker.end()).to_number();

        bg = rgxResult.get_end_position()+numOfLetters;
        for (int i = 0; i < multiple; ++i) {
            newStr.append(rgxResult.get_end_position(), bg);
        }
        rgxResult = findMarker(bg, end);
    }
    newStr.append(bg,end);

    return newStr.size();
}

long long day9_2(std::string &&dataFile) {

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    std::getline(iStream, oneStr);

    auto findMarker = ctre::search<R"(\(\d+x\d+\))">;
    auto beforeX = ctre::search<R"(\d+(?=x))">;
    auto afterX = ctre::search<R"((?<=x)\d+)">;

    /*
    Recursive lambda definition (supermodern 'deducing this' pattern of C++23).
    It receives a range and a multiplicator (ie. how many times that range will repeat) ... same on other levels.

    Fairly simple (and reasonably fast).
    */
    auto lam = [&] (this auto& self, int multiplicator, std::string::iterator&& beg, std::string::iterator&& end) -> long long {
        long long letterCounter = 0;

        auto rgxResult = findMarker(beg, end);
        while (rgxResult) {
            letterCounter += rgxResult.begin()-beg;
            std::string marker = rgxResult.to_string();

            int numOfLetters = beforeX(marker.begin(),marker.end()).to_number();
            int multiple = afterX(marker.begin(),marker.end()).to_number();

            beg = rgxResult.get_end_position()+numOfLetters;

            // recursing into another 'marker' range of letters
            letterCounter += self(multiple, rgxResult.get_end_position(), rgxResult.get_end_position()+numOfLetters);

            rgxResult = findMarker(beg, end);
        }
        letterCounter += end-beg;
        return letterCounter*multiplicator;
    };
    // Initiating recursion
    auto ans = lam(1, oneStr.begin(),oneStr.end());
    return ans;
}

}