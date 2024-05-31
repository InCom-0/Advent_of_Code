#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <md5.h>


namespace AOC2016 {
    int day15_1(std::string &&dataFile) {
        struct oneDisc_t {
            int discID;
            std::vector<bool> position;
        };

        auto getNumOfPos = ctre::search<R"(\d+(?= positions))">;
        auto getStartingPos = ctre::search<R"(\d+(?=\.))">;

        auto VofV_NumOfPos_StartPos = AOC_commons::parseInputUsingCTRE::processFile(
            dataFile, getNumOfPos, getStartingPos);

        std::vector<oneDisc_t> discs;


        // Early exit if false is provided by the algo itself.
        auto testAllDiscs = [](auto &&dscs, const int &&posToCheck) -> bool {
            return flux::ref(dscs).all([&](auto &&a) { return a.position[posToCheck] == true; });
        };

        auto turnDiscByVal = [](auto &&dsc, const int &val) {
            std::rotate(dsc.position.rbegin(), dsc.position.rbegin() + ((val) % dsc.position.size()),
                        dsc.position.rend());
        };
        auto turnAllDiscsByVal = [&](auto &&dscs, const int &val) {
            for (auto &a: dscs) turnDiscByVal(a, val);
        };

        // INITIAL SETUP OF DATA
        for (int i = 0; i < VofV_NumOfPos_StartPos.front().size(); ++i) {
            discs.push_back(oneDisc_t{i, std::vector<bool>(std::stoi(VofV_NumOfPos_StartPos[0][i]), false)});
            discs[i].position[std::stoi(VofV_NumOfPos_StartPos[1][i])] = true;

            /*
            Rotate discs into positions so that desired output is for all to be TRUE at pos[0].
            Note: This doesn't count towards answer as it is only 'as-if' rotation to make the algo simpler.
            */
            std::rotate(discs[i].position.rbegin(), discs[i].position.rbegin() + ((i + 1) % discs[i].position.size()),
                        discs[i].position.rend());
        }
        std::sort(discs.begin(), discs.end(), [](auto &&a, auto &&b) { return a.position.size() > b.position.size(); });
        int turnBy = std::find(discs.front().position.rbegin(), discs.front().position.rend(), true)
                     - discs.front().position.rbegin() + 1;

        turnAllDiscsByVal(discs, turnBy);
        int turningCounter = turnBy;
        turnBy = discs.front().position.size();

        // MAIN LOGIC LOOP
        while (not testAllDiscs(discs, 0)) {
            turnAllDiscsByVal(discs, turnBy);
            turningCounter += turnBy;
        }

        return turningCounter;
    }
}
