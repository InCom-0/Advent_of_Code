#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    std::vector<std::vector<int>> day17_0(std::string dataFile, int eggnogLiters) {
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
        if (not iStream.is_open()) return std::vector<std::vector<int>>();

        std::vector<int> containers;
        std::string oneStr;
        std::string lastRe = "";

        while (std::getline(iStream, oneStr)) {
            auto bg = oneStr.begin();
            auto re = ctre::search<R"(\d+)">(bg, oneStr.end());
            containers.push_back(re.to_number());
        }

        std::vector<std::unordered_map<std::vector<int>, int, VectorHasher> > arp;
        arp.push_back(std::unordered_map<std::vector<int>, int, VectorHasher>());
        std::vector<std::vector<int>> ans;

        std::vector<int> toInsert(containers.size(), 0);
        for (int i = 0; i < containers.size(); ++i) {
            toInsert[i] = 1;
            arp[0].emplace(toInsert, containers[i]);
            toInsert[i] = 0;
        }

        int lvlIdx = 0;
        do {
            arp.push_back(std::unordered_map<std::vector<int>, int, VectorHasher>());
            for (auto &option: arp[lvlIdx]) {
                for (int j = 0; j < containers.size(); ++j) {
                    if (option.first[j] == 0 && option.second + containers[j] < eggnogLiters+1) {
                        toInsert = option.first;
                        toInsert[j] = 1;
                        if (option.second + containers[j] == eggnogLiters && not arp[lvlIdx + 1].contains(toInsert))
                            ans.push_back(
                                toInsert);
                        arp[lvlIdx + 1].emplace(toInsert, option.second + containers[j]);
                    }
                }
            }
            lvlIdx++;
        } while ((containers.size() - lvlIdx > 1) && (arp[lvlIdx].size() > 0));

        return ans;
    }
    int day17_1(std::string dataFile, int eggnogLiters) {
        return day17_0(dataFile, eggnogLiters).size();
    }
    int day17_2(std::string dataFile, int eggnogLiters) {

        auto const ans = day17_0(dataFile, eggnogLiters);

        int minEleNew = INT_MAX;
        auto const minEle = flux::ref(ans)
                .map([](auto &a) { return flux::ref(a).count_if([](auto &b) { return b == 1; }); })
                .filter([&](auto &&a) {
                    minEleNew = std::min(minEleNew, (int) a);
                    return true;
                })
                .count_if([&](auto &&b) { return b == minEleNew; });
        return minEle;
    }
}