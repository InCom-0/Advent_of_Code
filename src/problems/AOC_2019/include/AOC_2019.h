#pragma once

#include <string>
#include <vector>

namespace AOC2019 {

long long
day1_1(std::string dataFile);
long long
day1_2(std::string dataFile);

long long
day2_1(std::string dataFile);
long long
day2_2(std::string dataFile);

long long
day3_1(std::string dataFile);
long long
day3_2(std::string dataFile);

long long
day4_1(std::string dataFile);
long long
day4_2(std::string dataFile);

long long
day5_1(std::string dataFile, long long inputVal);
long long
day5_2(std::string dataFile, long long inputVal);

long long
day6_1(std::string dataFile);
long long
day6_2(std::string dataFile);

long long
day7_1(std::string dataFile);
long long
day7_2(std::string dataFile);

long long
day8_1(std::string dataFile, std::pair<size_t, size_t> imgSizeWH);
std::string
day8_2(std::string dataFile, std::pair<size_t, size_t> imgSizeWH);

long long
day9_1(std::string dataFile, long long inputValue_1);

std::pair<std::pair<long long, long long>, long long>
day10_0(std::string dataFile);
long long
day10_1(std::string dataFile);
long long
day10_2(std::string dataFile);

long long
day11_1(std::string dataFile);
std::string
day11_2(std::string dataFile);

long long
day12_1(std::string dataFile);
long long
day12_2(std::string dataFile);

long long
day13_1(std::string dataFile);
long long
day13_2(std::string dataFile);

long long
day14_1(std::string dataFile);
long long
day14_2(std::string dataFile);

auto
day15_0(std::string dataFile, std::array<long long, 2> &k2Oxy);
long long
day15_1(std::string dataFile);
long long
day15_2(std::string dataFile);

long long
day16_1(std::string dataFile, size_t num_phases);
long long
day16_2(std::string dataFile, size_t num_phases);

std::vector<std::vector<char>>
day17_0(std::string dataFile);
long long
day17_1(std::string dataFile);
long long
day17_2(std::string dataFile);

long long
day18_1(std::string dataFile);
long long
day18_2(std::string dataFile);

std::vector<std::vector<int>>
day19_0(std::string dataFile, std::pair<int, int> const exploreSize);
long long
day19_1(std::string dataFile);
long long
day19_2(std::string dataFile);

long long
day20_1(std::string dataFile);
long long
day20_2(std::string dataFile);

long long
day21_1(std::string dataFile);
long long
day21_2(std::string dataFile);

typedef __int128 int128_t;

long long
day22_0(std::string dataFile, int128_t numOfCards, size_t cardToTrack, size_t shuffleReps = 1,
        bool reverseInstr = false);
long long
day22_1(std::string dataFile);
long long
day22_2(std::string dataFile);

long long
day23_1(std::string dataFile);
long long
day23_2(std::string dataFile);

long long
day24_1(std::string dataFile);
long long
day24_2(std::string dataFile, size_t const iterations = 200);

std::string
day25_1(std::string dataFile);
std::string
day25_2(std::string dataFile);

} // namespace AOC2019