#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <md5.h>


namespace AOC2016 {
    struct oneDisc_t {
        int m_xCoord;
        int m_yCoord;
        int m_sz;
        int m_used;
        int m_avail;
        int m_used_pct;
    };

    int day22_1(std::string dataFile) {
        auto xCoord_ctre = ctre::search<R"((?<=x)\d+)">;
        auto yCoord_ctre = ctre::search<R"((?<=y)\d+)">;
        auto digit_T_ctre = ctre::search<R"(\d+(?=T))">;
        auto digit_pct_ctre = ctre::search<R"(\d+(?=%))">;

        auto VofV = LC_commons::parseInputUsingCTRE::processFile(dataFile, xCoord_ctre, yCoord_ctre, digit_T_ctre, digit_T_ctre, digit_T_ctre, digit_pct_ctre);

        std::vector<std::vector<oneDisc_t>> discVect;

        for (int i = 0; i < VofV.front().size(); ++i) {
            if (std::stoi(VofV[0][i]) == discVect.size()) discVect.push_back(std::vector<oneDisc_t>());
            discVect[std::stoi(VofV[0][i])].push_back(oneDisc_t{std::stoi(VofV[0][i]), std::stoi(VofV[1][i]),
                                                    std::stoi(VofV[2][i]), std::stoi(VofV[3][i]), std::stoi(VofV[4][i]), std::stoi(VofV[5][i])});
        }

        auto discVect_joined_seq = flux::from_range(discVect | std::views::join);

        auto res = flux::cartesian_product(discVect_joined_seq, discVect_joined_seq)
                                            .filter([] (auto &&a) {
                                                return     (std::get<0>(a).m_used <= std::get<1>(a).m_avail &&
                                                           (std::get<0>(a).m_xCoord != std::get<1>(a).m_xCoord ||
                                                            std::get<0>(a).m_yCoord != std::get<1>(a).m_yCoord) &&
                                                            std::get<0>(a).m_used != 0);
                                            })
                                            .count();
        return res;
    }

    int day22_2(std::string dataFile) {

        return -999;
    }

}
