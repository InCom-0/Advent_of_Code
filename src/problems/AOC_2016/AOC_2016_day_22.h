#pragma once

#include <vector>
#include <string>

namespace AOC2016 {
    struct day_22_oneDisc_t {
        int m_xCoord;
        int m_yCoord;
        int m_sz;
        int m_used;
        int m_avail;
        int m_used_pct;
    };

    std::vector<std::pair<day_22_oneDisc_t,day_22_oneDisc_t>> day22_0 (std::string dataFile);
    int day22_1(std::string dataFile);
    int day22_2(std::string dataFile);

}