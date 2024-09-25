#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
int day1_1(std::string input) {
    int curFloor = 0;
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(') { curFloor++; }
        else { curFloor--; }
    }
    return curFloor;
}


int day1_2(std::string input) {
    int curFloor = 0;
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(') { curFloor++; }
        else { curFloor--; }
        if (curFloor < 0) { return i + 1; }
    }
    return INT_MIN;
}
} // namespace AOC2015
