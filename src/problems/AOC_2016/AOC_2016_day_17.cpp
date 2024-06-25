#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <md5.h>


namespace AOC2016 {

    std::string day17_1(std::string input, int roomRows, int roomCols) {
        struct branchPoint {
            int row;
            int col;
            std::string passPlusPath;
        };
        MD5 md5;
        std::vector<std::queue<branchPoint>> VofQueues;

        roomRows -= 1;
        roomCols -= 1;

        auto evaluateBP = [&] (branchPoint &bp, int callingLvl) -> bool {
            if (bp.row == roomRows && bp.col == roomCols) return true;

            std::string calcMD5 = md5(bp.passPlusPath);

            if (calcMD5[1] > 97 && bp.row < roomRows) VofQueues[callingLvl].push(branchPoint{bp.row + 1, bp.col, bp.passPlusPath + 'D'});
            if (calcMD5[3] > 97 && bp.col < roomCols) VofQueues[callingLvl].push(branchPoint{bp.row, bp.col + 1, bp.passPlusPath + 'R'});

            if (calcMD5[0] > 97 && bp.row > 0) VofQueues[callingLvl+1].push(branchPoint{bp.row - 1, bp.col, bp.passPlusPath + 'U'});
            if (calcMD5[2] > 97 && bp.col > 0) VofQueues[callingLvl+1].push(branchPoint{bp.row, bp.col - 1, bp.passPlusPath + 'L'});
            return false;
        };

        std::queue<branchPoint> oneQ;
        oneQ.push(branchPoint{0,0,input});
        VofQueues.push_back(oneQ);
        oneQ.pop();

        int line = 0;
        while (true) {
            VofQueues.push_back(oneQ);
            while (not VofQueues[line].empty()) {
                if (evaluateBP(VofQueues[line].front(),line)) return VofQueues[line].front().passPlusPath.substr(input.size());
                VofQueues[line].pop();
            }
            line++;
        }
    }

    std::string day17_2(std::string input, int roomRows, int roomCols) {
        struct branchPoint {
            int row;
            int col;
            std::string passPlusPath;
        };
        roomRows -= 1;
        roomCols -= 1;

        MD5 md5;
        std::queue<branchPoint> onePQ;
        onePQ.push(branchPoint{0,0, input});

        std::string maxPath = "";

        auto evaluateBP = [&] () -> bool {
            if (onePQ.empty()) return true;
            if (onePQ.front().row == roomRows && onePQ.front().col == roomCols) {
                maxPath = maxPath.size() < onePQ.front().passPlusPath.size() - input.size() ? onePQ.front().passPlusPath.substr(input.size()) : maxPath;
                onePQ.pop();
                return false;
            }

            auto &bp = onePQ.front();
            std::string calcMD5 = md5(bp.passPlusPath);

            if (calcMD5[1] > 97 && bp.row < roomRows) onePQ.push(branchPoint{bp.row + 1, bp.col, bp.passPlusPath + 'D'});
            if (calcMD5[3] > 97 && bp.col < roomCols) onePQ.push(branchPoint{bp.row, bp.col + 1,bp.passPlusPath + 'R'});

            if (calcMD5[0] > 97 && bp.row > 0) onePQ.push(branchPoint{bp.row - 1, bp.col,bp.passPlusPath + 'U'});
            if (calcMD5[2] > 97 && bp.col > 0) onePQ.push(branchPoint{bp.row, bp.col - 1,bp.passPlusPath + 'L'});

            onePQ.pop();
            return false;
        };

        while (not evaluateBP()) {}
        return maxPath;
    }


}
