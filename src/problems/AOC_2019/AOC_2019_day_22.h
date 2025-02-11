#pragma once

#include <string>

namespace AOC2019 {
typedef __int128 int128_t;

long long day22_0(std::string dataFile, int128_t numOfCards, size_t cardToTrack, size_t shuffleReps = 1,
                  bool reverseInstr = false);
long long day22_1(std::string dataFile);
long long day22_2(std::string dataFile);
} // namespace AOC2019