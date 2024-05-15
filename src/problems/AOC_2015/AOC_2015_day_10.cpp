#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day10_1(std::string input, int rep) {
        auto lambda = [&](auto const &self, std::string lamInput) -> int {
            int identicalCount = 0;
            char prev = lamInput[0];
            std::string outString = "";

            for (auto const &itemChar: lamInput) {
                if (prev != itemChar) {
                    outString += std::to_string(identicalCount);
                    outString.push_back(prev);
                    identicalCount = 1;
                } else {
                    identicalCount++;
                }
                prev = itemChar;
            }
            outString += std::to_string(identicalCount);
            outString.push_back(prev);

            rep--;
            if (rep > 0) return self(self, outString);
            else return outString.size();
        };

        return lambda(lambda, input);
    }

    int day10_2(std::string input, int rep) {
        auto lambda = [&](auto const &self, std::string const &lamInput) -> int {
            std::string outString = "";

            flux::ref(lamInput)
                    .chunk_by([](auto &a, auto &b) { return a == b; })
                    .for_each([&](auto &&a) {
                        outString += std::to_string(a.size());
                        outString.push_back(a.read_at(a.first()));
                    });
            rep--;
            if (rep > 0) return self(self, outString);
            else return outString.size();
        };

        return lambda(lambda, input);
    }

    int day10_3(std::string input, int rep) {
        auto lambda = [&](auto const &self, std::string lamInput) -> int {
            auto test = flux::ref(lamInput)
                    .chunk_by([](auto &a, auto &b) { return a == b; })
                    .map([&](auto &&a) {
                        return std::to_string(a.size()) + a.read_at(a.first());
                    })
                    .flatten();

            std::string outString = flux::to<std::string>(test);

            rep--;
            if (rep > 0) return self(self, outString);
            else return outString.size();
        };

        return lambda(lambda, input);
    }
}