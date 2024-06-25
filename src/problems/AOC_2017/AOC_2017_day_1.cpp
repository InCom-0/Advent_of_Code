#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2017 {

    /*
    As Sean Parent famously said (in his infinite wisdom): 'That's a rotate' couple dozen times in 1 talk :-).
    https://www.youtube.com/watch?v=qH6sSOr-yk8&t=0s
    */
    int day1_1(std::string dataFile) {

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);

        std::string oneStr;
        std::getline(iStream, oneStr);

        std::string oneStr_rotated = oneStr;
        std::rotate(oneStr_rotated.begin(), oneStr_rotated.begin()+1, oneStr_rotated.end());

        auto ans = flux::zip(flux::ref(oneStr), flux::ref(oneStr_rotated))
                    .filter([] (auto &&a) -> bool {return (a.first == a.second);})
                    .fold([] (auto &&init, auto &&a) -> int {return init + (a.first-48);}, 0);
        return ans;

    }
    int day1_2(std::string dataFile) {

            std::ifstream iStream;
            iStream.clear();
            iStream.open(dataFile);

            std::string oneStr;
            std::getline(iStream, oneStr);

            std::string oneStr_rotated = oneStr;
            std::rotate(oneStr_rotated.begin(), oneStr_rotated.begin()+oneStr_rotated.size()/2, oneStr_rotated.end());

            auto ans = flux::zip(flux::ref(oneStr), flux::ref(oneStr_rotated))
                        .filter([] (auto &&a) -> bool {return (a.first == a.second);})
                        .fold([] (auto &&init, auto &&a) -> int {return init + (a.first-48);}, 0);
            return ans;
    }
}