#pragma once

#include <incom_commons.h>
#include <string>
#include <unordered_map>
#include <vector>


namespace AOC2016 {

int
day1_1(std::string dataFile);
int
day1_2(std::string dataFile);

int
day2_1(std::string dataFile);
std::string
day2_2(std::string dataFile);

int
day3_1(std::string dataFile);
int
day3_2(std::string dataFile);

int
day4_1(std::string dataFile);
int
day4_2(std::string dataFile);

std::string
day5_1(const std::string &&doorID);
std::string
day5_2(const std::string &&doorID);

std::string
day6_1(std::string &&dataFile);
std::string
day6_2(std::string &&dataFile);

int
day7_1(std::string &&dataFile);
int
day7_2(std::string &&dataFile);

int
day8_1(std::string &&dataFile);
std::vector<std::vector<int>>
day8_2(std::string &&dataFile);

int
day9_1(std::string &&dataFile);
long long
day9_2(std::string &&dataFile);

int
day10_1(std::string &&dataFile, std::pair<int, int> chipToFind);
int
day10_2(std::string &&dataFile, std::pair<int, int> chipToFind);

std::vector<int>
day12_1(std::string dataFile, int regA, int regB, int regC, int regD);
std::vector<int>
day12_2(std::string dataFile);

int
day13_1(std::string dataFile, int favNumber, const int targetCol, const int targetRow);
int
day13_2(std::string dataFile, int favNumber, const int maxSteps);

int
day14_1(std::string salt, const int keysToProduce, const int keyStretchCount);

int
day15_1(std::string &&dataFile);
int
day15_2(std::string &&dataFile);

std::string
day16_1(std::string input, int discLength);
std::string
day16_2(std::string input, int discLength);

std::string
day17_1(std::string input, int roomRows, int roomCols);
std::string
day17_2(std::string input, int roomRows, int roomCols);

int
day18_1(std::string dataFile, int rowsToConsider);

int
day19_1(int numOfElves);
int
day19_2(int numOfElves);

unsigned long
day20_1(std::string dataFile);
unsigned long
day20_2(std::string dataFile);

std::string
day21_1(std::string dataFile, std::string &&input);
std::string
day21_2(std::string dataFile, std::string &&input);

struct day_22_oneDisc_t {
    int m_xCoord;
    int m_yCoord;
    int m_sz;
    int m_used;
    int m_avail;
    int m_used_pct;
};

std::vector<std::pair<day_22_oneDisc_t, day_22_oneDisc_t>>
day22_0(std::string dataFile);
int
day22_1(std::string dataFile);
int
day22_2(std::string dataFile);

std::vector<int>
day23_1(std::string dataFile, int regA, int regB, int regC, int regD);

struct day_24_location_t {
    int locID;
    int rowID;
    int colID;
};
bool
day24_0(std::string dataFile, std::vector<day_24_location_t> &locations,
        std::unordered_map<std::pair<int, int>, int, incstd::hashing::XXH3Hasher> &shotestDistances);
int
day24_1(std::string dataFile);
int
day24_2(std::string dataFile);

int
day25_1(std::string dataFile);

} // namespace AOC2016
