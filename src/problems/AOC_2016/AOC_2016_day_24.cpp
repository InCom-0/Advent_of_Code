#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <bitlib/bitlib.hpp>

#include <AOC_2016_day_24.h>

namespace AOC2016 {

        /*
        Helper function which creates an uoMap of shortest distances within the 'maze' from each point of interest
        to each other point of interest.
        Creates a 'locations' vector as well for convenience.

        Using return parameters, because the alternative is some annoying tuple or a struct of complex types.
        */
    bool day24_0(std::string dataFile, std::vector<day_24_location_t> &locations, std::unordered_map<std::pair<int,int>, int, AOC_commons::XXH3Hasher> &shotestDistances) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string oneStr;

        std::vector<bit::bit_vector<unsigned char>> map;
        auto mapCopy = map;
        std::vector<std::queue<day_24_location_t>> curQueue;
        std::vector<std::pair<int, int>> directions = {{-1,0}, {0, 1}, {1,0}, {0,-1}};

        // CORE ALGO
        int line = 0;
        auto routeFinder = [&] (day_24_location_t const &targetLoc, day_24_location_t const &ql) -> bool {
            if (targetLoc.rowID == ql.rowID && targetLoc.colID == ql.colID) return true;
            for (int i = 0; i < 4; ++i) {
                // Must be unexplored, otherwise no need to consider it.
                if (mapCopy[ql.rowID + directions[i].first][ql.colID + directions[i].second] == bit::bit1) {
                    if (std::abs(ql.rowID + directions[i].first - targetLoc.rowID) > std::abs(ql.rowID - targetLoc.rowID) ||
                        std::abs(ql.colID + directions[i].second - targetLoc.colID) > std::abs(ql.colID - targetLoc.colID)) {
                            curQueue[line+1].push(day_24_location_t{ql.locID+1, ql.rowID + directions[i].first, ql.colID + directions[i].second});
                        }
                    else curQueue[line].push(day_24_location_t{ql.locID+1, ql.rowID + directions[i].first, ql.colID + directions[i].second});
                    mapCopy[ql.rowID + directions[i].first][ql.colID + directions[i].second].set(false); // explored positions will not be checked again ever.
                }
            }
            return false;
        };

        // GET THE DATA
        while (std::getline(iStream, oneStr)) {
            map.push_back(bit::bit_vector<unsigned char>());
            for (auto &letter: oneStr) {
                if (letter >= 48 && letter < 58) locations.push_back({letter-48,(int)map.size()-1,(int)map.back().size()});
                map.back().push_back(bit::bit_value((bool)(letter - '#')));
            }
        }

        // PREP THE DATA
        std::ranges::sort(locations, [] (auto &&a, auto &&b) {return a.locID < b.locID;});
        std::vector<int> perm (locations.size(), 0);
        perm[0] = 1; perm[1] = 1;

        // MAIN LOGIC LOOP
        /*
        To avoid computing duplicates where source is switched with target ... using permuted helper vector
        which selects only the relevant pairs from the locations vector.
        The algo idea is to only explore possibilities that 'marched away' from target the least each time.

        Enables visiting each place in the maze only once and furthermore prioritizes exploration of
        'more likely' avenues for a typical maze (that is non-edgecases) and usually avoids most of the maze entirely.
        */
        do {
            flux::zip(flux::ref(locations), flux::ref(perm))
                                            .filter([] (auto &&a) {return a.second == 1;})
                                            .chunk(2)
                                            .for_each([&] (auto &&a) {
                                                mapCopy = map;
                                                curQueue.clear();
                                                curQueue.push_back(std::queue<day_24_location_t>());
                                                curQueue.front().emplace(day_24_location_t {0,
                                                flux::read_at(a, a.first()).first.rowID,
                                                flux::read_at(a, a.first()).first.colID});
                                                
                                                mapCopy[curQueue.front().front().rowID][curQueue.front().front().colID].set(false);

                                                auto sec = a.first();
                                                flux::inc(a, sec);

                                                bool brk = false;
                                                line = 0;
                                                while (true) {
                                                    curQueue.push_back(std::queue<day_24_location_t>());
                                                    while (not curQueue[line].empty()) {
                                                        if (routeFinder (flux::read_at(a, sec).first, curQueue[line].front())) {brk = true; break;}
                                                        curQueue[line].pop();
                                                    }
                                                    if (brk) break;
                                                    line++;
                                                }
                                                shotestDistances.insert({{flux::read_at(a, a.first()).first.locID,
                                                flux::read_at(a, sec).first.locID}, curQueue[line].front().locID});
                                            });

        } while (std::ranges::prev_permutation(perm).found);

        return true;
    }

    int day24_1(std::string dataFile) {

        std::vector<day_24_location_t> locations;
        std::unordered_map<std::pair<int,int>, int, AOC_commons::XXH3Hasher> shotestDistances;
        day24_0(dataFile, locations,shotestDistances);

        std::vector<int> locIDs (locations.size(), 0);
        std::ranges::iota(locIDs, 0);
        std::vector<std::pair<std::vector<int>, int>> results;

        do {
            int accu = 0;
            for (int i = 0; i < locIDs.size()-1; ++i) {
                std::pair<int,int> pr {locIDs[i], locIDs[i+1]};
                if (pr.first > pr.second) std::swap(pr.first, pr.second);
                accu += shotestDistances.at(pr);
            }
            results.push_back({locIDs, accu});
        } while (std::next_permutation(locIDs.begin()+1, locIDs.end()));

        return flux::ref(results).min([] (auto &&a, auto &&b) {return a.second < b.second;})->second;
    }
    int day24_2(std::string dataFile) {
        std::vector<day_24_location_t> locations;
        std::unordered_map<std::pair<int,int>, int, AOC_commons::XXH3Hasher> shotestDistances;
        day24_0(dataFile, locations,shotestDistances);

        std::vector<int> locIDs (locations.size()+1, 0);
        std::ranges::iota(locIDs, 0);
        locIDs.back() = 0;
        std::vector<std::pair<std::vector<int>, int>> results;

        do {
            int accu = 0;
            for (int i = 0; i < locIDs.size()-1; ++i) {
                std::pair<int,int> pr {locIDs[i], locIDs[i+1]};
                if (pr.first > pr.second) std::swap(pr.first, pr.second);
                accu += shotestDistances.at(pr);
            }
            results.push_back({locIDs, accu});
        } while (std::next_permutation(locIDs.begin()+1, locIDs.end()-1));

        return flux::ref(results).min([] (auto &&a, auto &&b) {return a.second < b.second;})->second;
    }

}
