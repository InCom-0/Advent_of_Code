#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>



namespace AOC2016 {
    int day2_1(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        struct POS {
            int row = 1;
            int col = 1;

            void updateSelf(std::pair<int,int>& with) {
                row = std::clamp(row + with.first, 0,2);
                col = std::clamp(col + with.second, 0,2);
            }
        };
        std::vector<std::vector<int>> keypad;
        keypad.push_back(std::vector<int>{1,2,3});
        keypad.push_back(std::vector<int>{4,5,6});
        keypad.push_back(std::vector<int>{7,8,9});


        std::string oneStr;
        std::vector<std::vector<std::pair<int,int>>> instructionsVofV;

        while (std::getline(iStream, oneStr)) {
            instructionsVofV.push_back(std::vector<std::pair<int,int>>());
            for (auto& oneChar: oneStr) {
                if (oneChar == 'U') instructionsVofV.back().push_back(std::pair<int,int>(-1,0));
                else if (oneChar == 'R') instructionsVofV.back().push_back(std::pair<int,int>(0,1));
                else if (oneChar == 'D') instructionsVofV.back().push_back(std::pair<int,int>(1,0));
                else if (oneChar == 'L') instructionsVofV.back().push_back(std::pair<int,int>(0,-1));
            }
        }
        std::string ans;
        POS curPosOnKeypad;
        for (auto& instrLine: instructionsVofV) {
            for (auto& instr: instrLine) {
                curPosOnKeypad.updateSelf(instr);
            }
            ans.push_back(std::to_string(keypad[curPosOnKeypad.row][curPosOnKeypad.col])[0]);
        }
        return std::stoi(ans);

    }
    std::string day2_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return "";

    struct POS {
        int row = 2;
        int col = 0;

        void updateSelf(std::pair<int,int>& with) {
            std::pair<int,int> clampV {0,4};
            std::pair<int,int> clampH {0,4};
            if (col == 0 || col == 4) {
                clampV.first = 2;
                clampV.second = 2;
            }
            if (row == 0 || row == 4) {
                clampH.first = 2;
                clampH.second = 2;
            }
            if ((row % 2 == 1) && (col % 2 == 1)) {
                clampV.first = 1;
                clampV.second = 3;
                clampH.first = 1;
                clampH.second = 3;
            }

            row = std::clamp(row + with.first, clampV.first,clampV.second);
            col = std::clamp(col + with.second, clampH.first,clampH.second);
        }
    };
    std::vector<std::vector<char>> keypad;
    keypad.push_back(std::vector<char>{'X','X','1','X','X'});
    keypad.push_back(std::vector<char>{'X','2','3','4','X'});
    keypad.push_back(std::vector<char>{'5','6','7','8','9'});
    keypad.push_back(std::vector<char>{'X','A','B','C','X'});
    keypad.push_back(std::vector<char>{'X','X','D','X','X'});


    std::string oneStr;
    std::vector<std::vector<std::pair<int,int>>> instructionsVofV;

    while (std::getline(iStream, oneStr)) {
        instructionsVofV.push_back(std::vector<std::pair<int,int>>());
        for (auto& oneChar: oneStr) {
            if (oneChar == 'U') instructionsVofV.back().push_back(std::pair<int,int>(-1,0));
            else if (oneChar == 'R') instructionsVofV.back().push_back(std::pair<int,int>(0,1));
            else if (oneChar == 'D') instructionsVofV.back().push_back(std::pair<int,int>(1,0));
            else if (oneChar == 'L') instructionsVofV.back().push_back(std::pair<int,int>(0,-1));
        }
    }

    std::string ans;
    POS curPosOnKeypad;
    for (auto& instrLine: instructionsVofV) {
        for (auto& instr: instrLine) {
            curPosOnKeypad.updateSelf(instr);
        }
        ans.push_back(keypad[curPosOnKeypad.row][curPosOnKeypad.col]);
    }
    return ans;

}
}