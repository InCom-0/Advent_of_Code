#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
    long long day25_1(long long startVal, long long multiplyBy, long long divideBy, long long row,
    long long column) {

        long long countOfOps = (row*column-1) + (column * ((column-1)/2.0)) + (row * ((row-1)/2.0)) - (row-1);

        while (countOfOps != 0) {
            startVal = (startVal*multiplyBy) % divideBy;
            countOfOps--;
        }

        return startVal;
    }
}