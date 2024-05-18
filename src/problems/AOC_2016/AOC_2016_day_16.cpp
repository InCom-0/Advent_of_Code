#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <md5.h>


namespace AOC2016 {
    std::string day16_1(std::string input, int discLength) {

        auto oneStep = [] (std::vector<bool> &inp) {
            std::vector<bool> hlprVect = std::vector<bool>(inp.rbegin(), inp.rend());
            hlprVect.flip();
            inp.push_back(false);
            inp.insert(inp.end(), hlprVect.begin(), hlprVect.end());
        };

        auto checkSumCalc = [] (std::string &inp) -> std::string {
            auto rtrn = flux::ref(inp)
                                    .take(inp.size() - (inp.size() % 2))
                                    .chunk(2)
                                    .map([] (auto&& a) -> char {return '0' + (flux::read_at(a, flux::first(a)) == flux::read_at(a, flux::prev(a, flux::last(a))));})
                                    .to<std::string>();
            return rtrn;
        };

        std::vector<bool> boolVect;
        for (auto& a: input) boolVect.push_back(a-48);

        while (boolVect.size() < discLength) oneStep(boolVect);
        input = flux::from_range(boolVect)
                                            .take(discLength)
                                            .map([] (auto &&a) {return '0' + a;})
                                            .to<std::string>();

        input = checkSumCalc(input);
        while (input.size() % 2 == 0) input = checkSumCalc(input);

        return input;
    }
}
