#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <bits/ranges_algo.h>

#include <flux.hpp>


namespace AOC2017 {
    int day10_1(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string oneStr;
        std::getline(iStream, oneStr);

        auto d_ctre = ctre::search<R"(\d+)">;
        auto Vof = AOC_commons::parseInputUsingCTRE::processOneLineRPToneVect(oneStr, d_ctre);

        std::vector<int> input;
        for (auto &strItem: Vof) input.push_back(std::stoi(strItem));

        auto const ini = std::views::iota(0,256);
        std::vector<int> lst (ini.begin(),ini.end());

        int skipSize = 0;
        int realSkipAccu = 0;

        for (auto &inpItem: input) {
            std::reverse(lst.begin(), lst.begin()+inpItem);
            std::rotate(lst.begin(), lst.begin() + (inpItem + skipSize) % lst.size(), lst.end());
            realSkipAccu += (inpItem + skipSize) % lst.size();
            skipSize++;
        }
        std::rotate(lst.rbegin(), lst.rbegin() + (realSkipAccu % lst.size()), lst.rend());
        return lst[0] * lst[1];
    }
    std::string day10_2(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string oneStr;
        std::getline(iStream, oneStr);

        auto d_ctre = ctre::search<R"(\d+)">;
        auto Vof = AOC_commons::parseInputUsingCTRE::processOneLineRPToneVect(oneStr, d_ctre);

        std::vector<char> inputRAW;
        for (auto &charItem: Vof.front()) inputRAW.push_back(charItem);
        for (auto i = Vof.begin()+1; i < Vof.end(); ++i) {
            inputRAW.push_back(',');
            for (auto &charItem: i.operator*()) inputRAW.push_back(charItem);
        }
        std::vector<int> input (inputRAW.begin(), inputRAW.end());
        std::vector<int> additionalInput {17, 31, 73, 47, 23};
        for (auto &intItem: additionalInput) input.push_back(intItem);

        auto const ini = std::views::iota(0,256);
        std::vector<unsigned char> lst (ini.begin(),ini.end());

        int skipSize = 0;
        int realSkipAccu = 0;
        for (int i = 0; i < 64; ++i) {
            for (auto &inpItem: input) {
                std::reverse(lst.begin(), lst.begin()+inpItem);
                size_t realSkip = (inpItem + skipSize) % lst.size();
                std::rotate(lst.begin(), lst.begin() + realSkip, lst.end());
                realSkipAccu += realSkip;
                skipSize++;
            }
        }
        std::rotate(lst.rbegin(), lst.rbegin() + (realSkipAccu % lst.size()), lst.rend());

        std::string ans;
        flux::ref(lst)
                        .chunk(16)
                        .for_each([&] (auto &&seq16) {
                            unsigned char oneChar = flux::from(seq16).fold_first([] (auto &&ini, auto&& item) -> unsigned char
                                {return (ini xor item);}).value();
                            ans += std::format ("{:02x}", oneChar);
                        });
        return ans;
    }

    // TO BE USED FOR DAY14
    std::string day10_3(std::string inputRAW) {
        std::vector<int> input (inputRAW.begin(), inputRAW.end());
        std::vector<int> additionalInput {17, 31, 73, 47, 23};
        for (auto &intItem: additionalInput) input.push_back(intItem);

        auto const ini = std::views::iota(0,256);
        std::vector<unsigned char> lst (ini.begin(),ini.end());

        int skipSize = 0;
        int realSkipAccu = 0;
        for (int i = 0; i < 64; ++i) {
            for (auto &inpItem: input) {
                std::reverse(lst.begin(), lst.begin()+inpItem);
                size_t realSkip = (inpItem + skipSize) % lst.size();
                std::rotate(lst.begin(), lst.begin() + realSkip, lst.end());
                realSkipAccu += realSkip;
                skipSize++;
            }
        }
        std::rotate(lst.rbegin(), lst.rbegin() + (realSkipAccu % lst.size()), lst.rend());

        std::string ans;
        flux::ref(lst)
                        .chunk(16)
                        .for_each([&] (auto &&seq16) {
                            unsigned char oneChar = std::ranges::fold_left_first(seq16, std::bit_xor()).value();
                            ans += std::format ("{:08b}", oneChar);
                        });
        return ans;
    }

}