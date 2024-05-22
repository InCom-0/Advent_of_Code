#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>


namespace AOC2016 {
    std::string day6_1(std::string &&dataFile) {
        auto ctreOneChar = ctre::search<R"(\w)">;
        auto inp = LC_commons::parseInputUsingCTRE::processFile(dataFile, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar);

        std::string ans;
        for (auto& letterVector: inp) {
            std::unordered_map<char, int> charCountMap;
            for (auto& letter: letterVector) charCountMap[letter.front()] += 1;
            auto mostCommon = std::ranges::max_element(charCountMap.begin(),charCountMap.end(),[] (auto&& a, auto&&b) {return a.second < b.second;});
            ans.push_back(mostCommon->first);
        }
        return ans;
    }
    std::string day6_2(std::string &&dataFile) {
        auto ctreOneChar = ctre::search<R"(\w)">;
        auto inp = LC_commons::parseInputUsingCTRE::processFile(dataFile, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar, ctreOneChar);

        std::string ans;
        for (auto& letterVector: inp) {
            std::unordered_map<char, int> charCountMap;
            for (auto& letter: letterVector) charCountMap[letter.front()] += 1;
            auto mostCommon = std::ranges::min_element(charCountMap.begin(),charCountMap.end(),[] (auto&& a, auto&&b) {return a.second < b.second;});
            ans.push_back(mostCommon->first);
        }
        return ans;
    }

}