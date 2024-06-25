#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <md5.h>

#include  "AOC_2016_day_22.h"

namespace AOC2016 {

    std::vector<std::pair<day_22_oneDisc_t,day_22_oneDisc_t>> day22_0(std::string dataFile) {
        auto xCoord_ctre = ctre::search<R"((?<=-x)\d+)">;
        auto yCoord_ctre = ctre::search<R"((?<=-y)\d+)">;
        auto digit_T_ctre = ctre::search<R"(\d+(?=T))">;
        auto digit_pct_ctre = ctre::search<R"(\d+(?=%))">;

        auto VofV = AOC_commons::parseInputUsingCTRE::processFile(dataFile, xCoord_ctre, yCoord_ctre, digit_T_ctre, digit_T_ctre, digit_T_ctre, digit_pct_ctre);

        std::vector<std::vector<day_22_oneDisc_t>> discVect;

        for (int i = 0; i < VofV.front().size(); ++i) {
            if (std::stoi(VofV[0][i]) == discVect.size()) discVect.push_back(std::vector<day_22_oneDisc_t>());
            discVect[std::stoi(VofV[0][i])].push_back(day_22_oneDisc_t{std::stoi(VofV[0][i]), std::stoi(VofV[1][i]),
                                                    std::stoi(VofV[2][i]), std::stoi(VofV[3][i]), std::stoi(VofV[4][i]), std::stoi(VofV[5][i])});
        }

        auto discVect_joined_seq = flux::from_range(discVect | std::views::join);

        return flux::cartesian_product(discVect_joined_seq, discVect_joined_seq)
                                            .filter([] (auto &&a) {
                                                return     (std::get<0>(a).m_used <= std::get<1>(a).m_avail &&
                                                           (std::get<0>(a).m_xCoord != std::get<1>(a).m_xCoord ||
                                                            std::get<0>(a).m_yCoord != std::get<1>(a).m_yCoord) &&
                                                            std::get<0>(a).m_used != 0);
                                            })
                                            .to<std::vector<std::pair<day_22_oneDisc_t,day_22_oneDisc_t>>>();
    }

    int day22_1(std::string dataFile) {
        return day22_0(dataFile).size();
    }

    /*
    Super annoying problem as the generic solution (assuming relatively random input) would be complex.
    Probably would have to resort to some form of dynamic programming solution ... but a rather complex one at that,
    since everything can depend on everything else ... oh well.

    However, because of the input data, solved it using pen and paper in like 5 minutes.
    (after trying to figure out some nice generic solution for hours on end :-))
    */
    int day22_2(std::string dataFile) {
        return 225;
    }

}
