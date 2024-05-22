#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day5_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);

    std::string oneStr;

    if (not iStream.is_open()) return -1;

    int accu = 0;
    bool isThis;
    std::string tempStr;

    while (std::getline(iStream, oneStr)) {
        isThis = false;

        auto vvv = flux::ref(oneStr)
                .slide(2)
                .fold([&](auto &&acc, auto &&a) {
                    if (*a.front() == *a.back()) acc.first = true;
                    tempStr = flux::to<std::string>(a);
                    if (tempStr == "ab" || tempStr == "cd" || tempStr == "pq" || tempStr == "xy") acc.second = false;
                    return acc;
                }, std::make_pair<bool, bool>(false, true));
        if (!vvv.first || !vvv.second) continue;

        auto vowelCount = flux::ref(oneStr)
                .fold([](auto &&acc, auto &&a) {
                    if (a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u') return ++acc;
                    return acc;
                }, 0);

        if (vowelCount > 2) accu++;
    }

    iStream.clear();
    iStream.close();

    return accu;
}

int day5_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);

    std::string oneStr;

    if (not iStream.is_open()) return -1;

    int accu = 0;

    std::string tempStr = "x";
    std::string lastString = "x";
    std::unordered_set<std::string> st;

    while (std::getline(iStream, oneStr)) {
        bool isThis = true;
        flux::ref(oneStr)
                .slide(3)
                .for_each_while([&](auto &&a) {
                    if (*a.front() == *a.back()) isThis = false;
                    return isThis;
                });
        if (isThis) continue;

        isThis = true;
        flux::ref(oneStr)
                .slide(2)
                .for_each_while([&](auto &&a) {
                    tempStr = flux::to<std::string>(a);
                    if (st.contains(tempStr) && lastString != tempStr) isThis = false;
                    st.insert(tempStr);
                    lastString = tempStr;
                    return isThis;
                });


        if (!isThis) accu++;
        st.clear();
    }

    iStream.clear();
    iStream.close();

    return accu;
}
}