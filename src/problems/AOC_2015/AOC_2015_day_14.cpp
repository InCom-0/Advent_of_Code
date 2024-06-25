#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day14_1(std::string dataFile, int raceDuration) {
        struct VectorHasher {
            int operator()(const std::vector<int> &V) const {
                int hash = V.size();
                for (auto &i: V) {
                    hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                }
                return hash;
            }
        };

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        std::string oneStr;
        std::vector<std::string> names;
        std::vector<int> speed;
        std::vector<int> flightDur;
        std::vector<int> restDur;

        while (std::getline(iStream, oneStr)) {
            auto bg = oneStr.begin();
            auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
            names.push_back(re.to_string());

            bg = re.get_end_position();
            re = ctre::search<R"(\d+)">(bg, oneStr.end());
            speed.push_back(re.to_number());

            bg = re.get_end_position();
            re = ctre::search<R"(\d+)">(bg, oneStr.end());
            flightDur.push_back(re.to_number());

            bg = re.get_end_position();
            re = ctre::search<R"(\d+)">(bg, oneStr.end());
            restDur.push_back(re.to_number());
        }

        std::vector<int> calculatedTravel(names.size(), 0);

        int raceCounter = 1;
        while (raceCounter <= raceDuration) {
            for (int i = 0; i < names.size(); ++i) {
                int modu = raceCounter % (flightDur[i] + restDur[i]);
                if (modu <= flightDur[i] && modu > 0) calculatedTravel[i] += speed[i] * 1; // So that it is obvious that we mean 1 second here.
            }
            raceCounter++;
        }
        return std::ranges::max_element(calculatedTravel).operator*();
    }

    int day14_2(std::string dataFile, int raceDuration) {
        struct VectorHasher {
            int operator()(const std::vector<int> &V) const {
                int hash = V.size();
                for (auto &i: V) {
                    hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                }
                return hash;
            }
        };

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        std::string oneStr;
        std::vector<std::string> names;
        std::vector<int> speed;
        std::vector<int> flightDur;
        std::vector<int> restDur;

        while (std::getline(iStream, oneStr)) {
            auto bg = oneStr.begin();
            auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
            names.push_back(re.to_string());

            bg = re.get_end_position();
            re = ctre::search<R"(\d+)">(bg, oneStr.end());
            speed.push_back(re.to_number());

            bg = re.get_end_position();
            re = ctre::search<R"(\d+)">(bg, oneStr.end());
            flightDur.push_back(re.to_number());

            bg = re.get_end_position();
            re = ctre::search<R"(\d+)">(bg, oneStr.end());
            restDur.push_back(re.to_number());
        }

        std::vector<int> calculatedTravel(names.size(), 0);


        int raceCounter = 1;
        while (raceCounter <= raceDuration) {
            int maxDist = INT_MIN;
            std::vector<int> maxPoss;

            for (int i = 0; i < names.size(); ++i) {
                int multiplier = raceCounter / (flightDur[i] + restDur[i]);
                int modu = raceCounter % (flightDur[i] + restDur[i]);

                if (modu > flightDur[i]) modu = flightDur[i];

                if (maxDist < multiplier * speed[i] * flightDur[i] + modu * speed[i]) {
                    maxDist = multiplier * speed[i] * flightDur[i] + modu * speed[i];
                    maxPoss.clear();
                    maxPoss.push_back(i);
                } else if (maxDist == multiplier * speed[i] * flightDur[i] + modu * speed[i]) {
                    maxPoss.push_back(i);
                }
            }
            for (auto &idxToIncrement: maxPoss) {
                calculatedTravel[idxToIncrement]++;
            }

            raceCounter++;
        }

        return std::ranges::max_element(calculatedTravel).operator*();
    }
}