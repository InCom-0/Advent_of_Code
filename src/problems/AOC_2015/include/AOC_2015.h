#pragma once

#include <string>

namespace AOC2015 {

int day1_1(std::string input);
int day1_2(std::string input);

int day2_1(std::string dataFile);
int day2_2(std::string dataFile);

int day3_1(std::string dataFile);
int day3_2(std::string dataFile);

int day5_1(std::string dataFile);
int day5_2(std::string dataFile);

int day6_1(std::string dataFile);
int day6_2(std::string dataFile);

int day8_1(std::string dataFile);
int day8_2(std::string dataFile);

int day9_0(std::string dataFile);
int day9_1(std::string dataFile);
int day9_2(std::string dataFile);

int day10_1(std::string dataFile, int rep);
int day10_2(std::string dataFile, int rep);
int day10_3(std::string dataFile, int rep);

int day12_1(std::string dataFile);
int day12_2(std::string dataFile);

int day13_1(std::string dataFile);

int day14_1(std::string dataFile, int raceDuration);
int day14_2(std::string dataFile, int raceDuration);

int day16_1(std::string dataFile);
int day16_2(std::string dataFile);

int day17_0(std::string dataFile, int eggnogLiters);
int day17_1(std::string dataFile, int eggnogLiters);
int day17_2(std::string dataFile, int eggnogLiters);

int day18_1(std::string dataFile, int dimension, int repeatCount);
int day18_2(std::string dataFile, int dimension, int repeatCount);

int day19_1(std::string dataFile, std::string input);
int day19_3(std::string dataFile, std::string input);

int day20_1(std::string dataFile);
int day20_2(std::string dataFile);

int day21_1(int hp, int dmg, int armor, int myHP, std::string dataFile);
int day21_2(int hp, int dmg, int armor, int myHP, std::string dataFile);

int day22_1(int Ehp, int Edmg, int hp, int mana);
int day22_2(int Ehp, int Edmg, int hp, int mana);

int day23_1(std::string dataFile, unsigned int regA, unsigned int regB);

long long day24_1(std::string dataFile, int numberOfGroups);

long long day25_1(long long startVal, long long multiplyBy, long long divideBy, long long row, long long column);

} // namespace AOC2015