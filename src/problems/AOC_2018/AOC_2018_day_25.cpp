#include "bitlib/bit-containers/bit_vector.hpp"
#include "bitlib/bit-iterator/bit_value.hpp"
#include <algorithm>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <ctre/wrapper.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <queue>
#include <string>
#include <vector>


namespace AOC2018 {

long long day25_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // DATA PREP
    std::vector<std::vector<int>> coords;
    for (auto &line : input) {
        coords.push_back({std::stoi(line[0]), std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3])});
    }


    // LAMBDA DEFINITION
    auto calc_pointDistance = [&](std::vector<int> const &point_a, std::vector<int> const &point_b) {
        int accuManhDist = 0;
        for (int i = 0; i < point_a.size(); ++i) { accuManhDist += std::abs(point_a[i] - point_b[i]); }
        return accuManhDist;
    };

    // MAIN LOGIC
    std::queue<int>                qu;
    bit::bit_vector<unsigned char> explored(coords.size(), bit::bit0);
    long long                      ans = 0;

    auto firstUnExpIter = std::ranges::find_if(explored, [&](auto &&a) { return a == bit::bit0; });
    while (firstUnExpIter != explored.end()) {
        qu.push(firstUnExpIter - explored.begin());
        explored[qu.front()] = bit::bit1;
        ans++;
        while (not qu.empty()) {
            for (int tarID = 0; tarID < coords.size(); ++tarID) {
                if (explored[tarID] == bit::bit0) {
                    if (calc_pointDistance(coords[qu.front()], coords[tarID]) < 4) {
                        explored[tarID] = bit::bit1;
                        qu.push(tarID);
                    }
                }
            }
            qu.pop();
        }
        firstUnExpIter = std::ranges::find_if(explored, [&](auto &&a) { return a == bit::bit0; });
    }


    return ans;
}

} // namespace AOC2018