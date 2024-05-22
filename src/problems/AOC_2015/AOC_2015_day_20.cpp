#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {

    int day20_1(int numberInput) {
        auto divisorsOfN = [](auto &n) -> int {
            int sumAns = 0;
            for (int i = 1; i <= n / 2; ++i) {
                if (n % i == 0) sumAns += i;
            }
            sumAns += n;
            return sumAns;
        };

        int sum = 0;
        int bottom = 0;
        int top = 0;
        int next = 2;

        while (true) {
            if (sum < numberInput) {
                next *= 2;
                sum = divisorsOfN(next);
            } else {
                top = next;
                bottom = next / 2;
                while (sum != numberInput) {
                    next = bottom + (top - bottom) / 2;
                    if (top - bottom == 0) break;
                    sum = divisorsOfN(next);

                    if (sum > numberInput) {
                        top = next;
                    } else {
                        bottom = next + 1;
                    }
                }
                break;
            }
        }
        return next;
    }

    int day20_2(int numberInput) {
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