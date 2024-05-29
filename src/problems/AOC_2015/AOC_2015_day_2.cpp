#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day2_1(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);

        std::string oneStr;

        if (not iStream.is_open()) return -1;

        int accu = 0;
        int small;
        std::vector<int> lwh;

        while (std::getline(iStream, oneStr)) {
            flux::chunk_by(flux::ref(oneStr), [](auto &a, auto &b) {
                        return !(a == 'x' || b == 'x');
                    })
                    .filter([](auto &&a) { return (a.read_at(a.first()) != 'x'); })
                    .for_each([&](auto &&b) {
                        lwh.push_back(std::stoi(flux::to<std::string>(b)));
                        return;
                    });

            lwh.emplace_back(lwh.front());
            small = INT_MAX;
            accu += 2 * flux::ref(lwh)
                    .adjacent_map<2>([](auto &&a, auto &&b) {
                        return a * b;
                    })
                    .take_while([&](auto &&a) {
                        small = std::min(small, a);
                        return true;
                    })
                    .sum();

            accu += small;
            lwh.clear();
        }

        iStream.clear();
        iStream.close();

        return accu;
    }

    int day2_2(std::string dataFile) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);

        std::string oneStr;

        if (not iStream.is_open()) return -1;

        int accu = 0;
        int small;
        std::vector<int> lwh;

        while (std::getline(iStream, oneStr)) {
            flux::chunk_by(flux::ref(oneStr), [](auto &a, auto &b) {
                        return !(a == 'x' || b == 'x');
                    })
                    .filter([](auto &&a) { return (a.read_at(a.first()) != 'x'); })
                    .for_each([&](auto &&b) {
                        lwh.push_back(std::stoi(flux::to<std::string>(b)));
                        return;
                    });

            accu += flux::ref(lwh)
                    .fold_first([](auto &&acc, auto &&b) { return acc * b; })
                    .value();

            flux::sort(lwh, std::less());
            accu += 2 * flux::ref(lwh)
                    .take(2)
                    .sum();

            lwh.clear();
        }

        iStream.clear();
        iStream.close();

        return accu;
    }
}
