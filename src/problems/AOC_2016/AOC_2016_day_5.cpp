#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <md5.h>



namespace AOC2016 {

    std::string day5_1(const std::string&& doorID) {
        MD5 md5;

        std::string condition = "00000";
        std::string tmp;
        std::string ans;
        int additiveID = 0;

        while (ans.size() < 8) {
            tmp = md5(doorID + std::to_string(additiveID++));
            if (tmp.substr(0,5) == condition) ans.push_back(tmp[5]);
        }

        return ans;
    }

    std::string day5_2(const std::string&& doorID) {
        MD5 md5;

        std::string condition = "00000";
        std::string tmp;
        std::string ans = "________";
        std::vector<bool> posSet (8, false);
        int additiveID = 0;
        int accu = 0;

        while (accu < 8) {
            tmp = md5(doorID + std::to_string(additiveID++));
            if (tmp.substr(0,5) == condition && tmp[5] >= '0' && tmp[5] <= '7') {
                if(not posSet[tmp[5]-48]) {
                    ans[tmp[5]-48] = tmp[6];
                    posSet[tmp[5]-48] = true;
                    accu++;
                }
            }
        }

        return ans;
    }


}