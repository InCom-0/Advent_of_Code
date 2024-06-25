#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day20_1(std::string dataFile) {
        int numberInput = std::stoi(
        AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(\d+)">).front().front());

        int n = 1;
        auto lam = [](auto &input) {
            int d = sqrt(input) + 1;
            int ans = 0;

            for (int i = 1; i <= d; ++i) {
                if (input % i == 0) {
                    ans += input / i;
                    ans += i;
                }
            }
            return ans * 10;
        };

        while (lam(n) < numberInput) n++;
        return n;
    }
    int day20_2(std::string dataFile) {
        int numberInput = std::stoi(
        AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, ctre::search<R"(\d+)">).front().front());

        int n = 1;
        auto lam = [](auto &input) {
            int d = sqrt(input) + 1;
            int ans = 0;

            for (int i = 1; i <= d; ++i) {
                if (input % i == 0) {
                    if (i <= 50) ans += input / i;
                    if (input / i <= 50) ans += i;
                }
            }
            return ans * 11;
        };

        while (lam(n) < numberInput) n++;
        return n;
    }

}