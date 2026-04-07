#pragma once

#include <flux.hpp>
#include <string>
#include <vector>


namespace AOC2018 {

int
day1_1(std::string dataFile);
int
day1_2(std::string dataFile);

long long
day2_1(std::string dataFile);
std::string
day2_2(std::string dataFile);

unsigned int
day3_1(std::string dataFile);
unsigned int
day3_2(std::string dataFile);

int
day4_1(std::string dataFile);
int
day4_2(std::string dataFile);

int
day5_1(std::string dataFile);
int
day5_2(std::string dataFile);

int
day6_1(std::string dataFile);
int
day6_2(std::string dataFile, int distanceLessThan);

std::string
day7_1(std::string dataFile);
int
day7_2(std::string dataFile, const int workers);

int
day8_1(std::string dataFile);
int
day8_2(std::string dataFile);

unsigned long long
day9_1(std::string dataFile);

int
day10_1(std::string dataFile);

std::string
day11_1(std::string dataFile);
std::string
day11_2(std::string dataFile);
std::string
day11_3(std::string dataFile);

int
day12_1(std::string dataFile, long long generationsToSimulate);
long long
day12_2(std::string dataFile, long long generationsToSimulate);

std::string
day13_1(std::string dataFile);
std::string
day13_2(std::string dataFile);

long long
day14_1(std::string dataFile);
long long
day14_2(std::string dataFile);
long long
day14_3(std::string dataFile);
long long
day14_4(std::string dataFile);
long long
day14_5(std::string dataFile);

int
day15_1(std::string dataFile);
int
day15_2(std::string dataFile);

int
day16_1(std::string dataFile, int behaveLikeXorMoreOpCodes);
long long
day16_2(std::string dataFile_1, std::string dataFile_2);

std::pair<size_t, size_t>
day17_0(std::string dataFile);
size_t
day17_1(std::string dataFile);
size_t
day17_2(std::string dataFile);

std::vector<std::vector<char>>
day17_alt_0(std::string dataFile);

size_t
day18_1(std::string dataFile);
size_t
day18_2(std::string dataFile);

long long
day19_1(std::string dataFile, const std::vector<long long> regStartVal);
long long
day19_2(std::string dataFile, const std::vector<long long> regStartVal);

std::pair<long long, long long>
day20_1(std::string dataFile);

long long
day21_1(std::string dataFile);
long long
day21_2(std::string dataFile);

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
day24_2(std::string dataFile);

long long
day25_1(std::string dataFile);

} // namespace AOC2018