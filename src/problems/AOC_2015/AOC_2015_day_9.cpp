#include <fstream>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
int day9_1(std::string dataFile) {
    struct VectorHasher {
        int operator()(const std::vector<int> &V) const {
            int hash = V.size();
            for (auto &i : V) { hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2); }
            return hash;
        }
    };


    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string              oneStr;
    std::vector<std::string> left;
    std::vector<std::string> right;
    std::vector<int>         values;

    std::string                          lastRe = "";
    std::unordered_map<std::string, int> lookUp;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
        left.push_back(re.to_string());
        lookUp.emplace(re, lookUp.size());

        bg = re.get_end_position();
        re = ctre::search<R"(\w+)">(bg, oneStr.end());

        bg = re.get_end_position();
        re = ctre::search<R"(\w+)">(bg, oneStr.end());
        right.push_back(re.to_string());
        lookUp.emplace(re, lookUp.size());

        bg = re.get_end_position();
        re = ctre::search<R"(\w+)">(bg, oneStr.end());
        values.push_back(re.to_number());
    }

    std::vector<std::vector<int>> table(lookUp.size(), std::vector<int>(lookUp.size(), INT_MAX));
    for (int i = 0; i < left.size(); ++i) {
        table[lookUp[left[i]]][lookUp[right[i]]] = values[i];
        table[lookUp[right[i]]][lookUp[left[i]]] = values[i];
    }

    int minSoFar = INT_MAX;

    for (int i = 0; i < lookUp.size(); ++i) {
        std::vector<std::unordered_map<std::vector<int>, int, VectorHasher>> arp;
        arp.push_back(std::unordered_map<std::vector<int>, int, VectorHasher>());

        std::vector<int> toInsert(lookUp.size() + 1, 0);
        // last item in vector is position/cityID of last added 'bit' within the same vector
        toInsert[i]     = 1;
        toInsert.back() = i; // last item in vector is position/cityID of last added 'bit' within the same vector
        arp[0].emplace(toInsert, 0);
        int lvlIdx = 0;
        do {
            arp.push_back(std::unordered_map<std::vector<int>, int, VectorHasher>());
            for (auto &option : arp[lvlIdx]) {
                for (int j = 0; j < lookUp.size(); ++j) {
                    if (option.first[j] == 0) {
                        toInsert        = option.first;
                        toInsert[j]     = 1;
                        toInsert.back() = j;
                        arp[lvlIdx + 1].emplace(toInsert, option.second + table[option.first.back()][j]);
                        arp[lvlIdx + 1].at(toInsert) =
                            std::min(arp[lvlIdx + 1].at(toInsert), option.second + table[option.first.back()][j]);
                    }
                }
            }

            lvlIdx++;
        } while ((lookUp.size() - lvlIdx) > 1);
        minSoFar = std::min(minSoFar, std::min_element(arp.back().begin(), arp.back().end(), [](auto &&a, auto &&b) {
                                          return a.second < b.second;
                                      })->second);
    }

    return minSoFar;
}
int day9_2(std::string dataFile) {
    struct VectorHasher {
        int operator()(const std::vector<int> &V) const {
            int hash = V.size();
            for (auto &i : V) { hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2); }
            return hash;
        }
    };


    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string              oneStr;
    std::vector<std::string> left;
    std::vector<std::string> right;
    std::vector<int>         values;

    std::string                          lastRe = "";
    std::unordered_map<std::string, int> lookUp;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());
        left.push_back(re.to_string());
        lookUp.emplace(re, lookUp.size());

        bg = re.get_end_position();
        re = ctre::search<R"(\w+)">(bg, oneStr.end());

        bg = re.get_end_position();
        re = ctre::search<R"(\w+)">(bg, oneStr.end());
        right.push_back(re.to_string());
        lookUp.emplace(re, lookUp.size());

        bg = re.get_end_position();
        re = ctre::search<R"(\w+)">(bg, oneStr.end());
        values.push_back(re.to_number());
    }

    std::vector<std::vector<int>> table(lookUp.size(), std::vector<int>(lookUp.size(), INT_MAX));
    for (int i = 0; i < left.size(); ++i) {
        table[lookUp[left[i]]][lookUp[right[i]]] = values[i];
        table[lookUp[right[i]]][lookUp[left[i]]] = values[i];
    }

    int minSoFar = INT_MIN;

    for (int i = 0; i < lookUp.size(); ++i) {
        std::vector<std::unordered_map<std::vector<int>, int, VectorHasher>> arp;
        arp.push_back(std::unordered_map<std::vector<int>, int, VectorHasher>());

        std::vector<int> toInsert(lookUp.size() + 1, 0);
        // last item in vector is position/cityID of last added 'bit' within the same vector
        toInsert[i]     = 1;
        toInsert.back() = i; // last item in vector is position/cityID of last added 'bit' within the same vector
        arp[0].emplace(toInsert, 0);
        int lvlIdx = 0;
        do {
            arp.push_back(std::unordered_map<std::vector<int>, int, VectorHasher>());
            for (auto &option : arp[lvlIdx]) {
                for (int j = 0; j < lookUp.size(); ++j) {
                    if (option.first[j] == 0) {
                        toInsert        = option.first;
                        toInsert[j]     = 1;
                        toInsert.back() = j;
                        arp[lvlIdx + 1].emplace(toInsert, option.second + table[option.first.back()][j]);
                        arp[lvlIdx + 1].at(toInsert) =
                            std::max(arp[lvlIdx + 1].at(toInsert), option.second + table[option.first.back()][j]);
                    }
                }
            }

            lvlIdx++;
        } while ((lookUp.size() - lvlIdx) > 1);
        minSoFar = std::max(minSoFar, std::max_element(arp.back().begin(), arp.back().end(), [](auto &&a, auto &&b) {
                                          return a.second < b.second;
                                      })->second);
    }

    return minSoFar;
}
} // namespace AOC2015