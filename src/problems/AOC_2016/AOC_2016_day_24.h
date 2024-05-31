#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <AOC_commons.h>

namespace AOC2016 {
    struct day_24_location_t {
        int locID;
        int rowID;
        int colID;
    };
    bool day24_0(std::string dataFile, std::vector<day_24_location_t> &locations, std::unordered_map<std::pair<int,int>, int, AOC_commons::XXH3Hasher> &shotestDistances);
    int day24_1(std::string dataFile);
    int day24_2(std::string dataFile);

}