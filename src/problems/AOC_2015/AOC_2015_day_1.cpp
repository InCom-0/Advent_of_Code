#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day1(std::string input) {
        int curFloor = 0;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == '(') curFloor++;
            else curFloor--;
            if (curFloor < 0) return i + 1;
        }
        return INT_MIN;
    }
}