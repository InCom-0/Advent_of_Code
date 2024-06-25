#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <md5.h>
#include <algorithm>
#include "bitlib/bitlib.hpp"


namespace AOC2016 {
    std::string day16_1(std::string input, int discLength) {

        auto oneStep = [] (bit::bit_vector<unsigned char> &inp) {
            bit::bit_vector<unsigned char> hlprBitV = inp;

            bit::reverse(hlprBitV.begin(),hlprBitV.end());
            for (auto &&hlprBit: hlprBitV) hlprBit.flip();

            inp.push_back(bit::bit0);
            inp.insert(inp.end(), hlprBitV.begin(), hlprBitV.end());
        };

        auto checkSumCalc = [] (bit::bit_vector<unsigned char> &inp) ->void {
            for (int i = 0; i < inp.size()/2; ++i) inp[i].set(inp[2*i] == inp[2*i+1]);
            inp.resize(inp.size()/2);
        };


        bit::bit_vector<unsigned char> bv (input);

        while (bv.size() < discLength) oneStep(bv);
        bv.resize(discLength);

        checkSumCalc(bv);
        while (bv.size() % 2 == 0) checkSumCalc(bv);

        std::string ans;
        for (const auto &bvItem: bv) ans.push_back('0' + (bool)bvItem);

        return ans;
    }
}
