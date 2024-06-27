#include <AOC_commons.h>
#include <bits/ranges_algo.h>
#include <ctre.hpp>

#include <ankerl/unordered_dense.h>
#include <cmath>
#include <flux.hpp>

namespace AOC2017 {

unsigned long long day20_1(std::string dataFile) {
    // CUSTOM LOCAL TYPES
    struct coord {
        long long X;
        long long Y;
        long long Z;
        long long sum() const { return X + Y + Z; }
        long long sumAbs() const { return std::abs(X) + std::abs(Y) + std::abs(Z); }
    };
    auto d_ctre = ctre::search<R"(-?\d+)">;

    // DATA PREP
    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::vector<coord>> data;
    for (auto &inputLine : VofV) {
        data.push_back(std::vector<coord>());
        for (int i = 0; i < 3; ++i) {
            coord oneCoord;
            oneCoord.X = std::stoi(inputLine[3 * i + 0]);
            oneCoord.Y = std::stoi(inputLine[3 * i + 1]);
            oneCoord.Z = std::stoi(inputLine[3 * i + 2]);
            data.back().push_back(oneCoord);
        }
    }

    // MAIN LOGIC
    // The item closest to 0,0,0 must be the one with the lowest (in this case
    // summed) acceleration, regardless of original coord or speed.
    auto sumRng = flux::ref(data).map([](auto &&a) { return flux::read_at(a, flux::prev(a, flux::last(a))).sumAbs(); });
    return std::ranges::min_element(sumRng, [](auto &&a, auto &&b) { return a < b; }) - sumRng.begin();
}

unsigned long long day20_2(std::string dataFile) {
    // CUSTOM LOCAL TYPES
    struct coord {
        long long X;
        long long Y;
        long long Z;
    };
    auto d_ctre = ctre::search<R"(-?\d+)">;

    // DATA PREP
    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::vector<coord>> data;
    for (auto &inputLine : VofV) {
        data.push_back(std::vector<coord>());
        for (int i = 0; i < 3; ++i) {
            coord oneCoord;
            oneCoord.X = std::stoi(inputLine[3 * i + 0]);
            oneCoord.Y = std::stoi(inputLine[3 * i + 1]);
            oneCoord.Z = std::stoi(inputLine[3 * i + 2]);
            data.back().push_back(oneCoord);
        }
    }

    std::vector<std::vector<long long>>     VofCollisions; // 0 = collision time, 1 = idx of particle 1, 2 = idx of
                                                           // particle 2
    ankerl::unordered_dense::set<long long> collided;

    // LAMBDA DEFINITIONS
    auto findQuadRoots = [](long long &&a, long long &&b, long long &&c) -> std::vector<long long> {
        long long discrim = (b * b) - (4 * a * c);
        long long ans1    = LLONG_MIN;
        long long ans2    = LLONG_MIN;
        double    tempRes;
        if (a == 0) {
            if (b != 0) { ans1 = -c / (b - a); }
        }
        else if (discrim == 0) {
            tempRes = (-1) * ((double)b / a) / 2;
            ans1    = tempRes == (long long)tempRes ? tempRes : LLONG_MIN;
        }
        else if (discrim > 0) {
            tempRes = std::sqrt(discrim);
            if (tempRes == (long long)tempRes) {
                discrim = tempRes;
                tempRes = (((double)-b + discrim)) / (2 * a);
                ans1    = tempRes == (long long)tempRes ? tempRes : LLONG_MIN;

                tempRes = (((double)-b - discrim)) / (2 * a);
                ans2    = tempRes == (long long)tempRes ? tempRes : LLONG_MIN;
            }
        }
        return std::vector{std::move(ans1), std::move(ans2)};
    };

    auto findAxisPosAt = [](long long const &p, long long const &v, long long const &a,
                            long long const &time) -> long long {
        return ((a * time * time) + ((a + 2 * v) * time) + (2 * p)) / 2;
    };

    // MAIN LOGIC
    for (long long i = 0; i < data.size(); ++i) {
        for (long long j = i + 1; j < data.size(); ++j) {
            auto roots = findQuadRoots(data[j][2].X - data[i][2].X,
                                       (data[j][2].X - data[i][2].X) + 2 * (data[j][1].X - data[i][1].X),
                                       2 * (data[j][0].X - data[i][0].X));

            bool found = false;
            for (auto &oneRoot : roots) {
                if (found) { break; }
                if (oneRoot < 0) { continue; }

                if (findAxisPosAt(data[i][0].Y, data[i][1].Y, data[i][2].Y, oneRoot) ==
                    findAxisPosAt(data[j][0].Y, data[j][1].Y, data[j][2].Y, oneRoot)) {
                    if (findAxisPosAt(data[i][0].Z, data[i][1].Z, data[i][2].Z, oneRoot) ==
                        findAxisPosAt(data[j][0].Z, data[j][1].Z, data[j][2].Z, oneRoot)) {
                        found = true;
                        VofCollisions.push_back(std::vector{oneRoot, i, j});
                    }
                }
            }
        }
    }

    std::ranges::sort(VofCollisions, [](auto &&a, auto &&b) { return a.front() < b.front(); });

    flux::ref(VofCollisions)
        .chunk_by([](auto &&a, auto &&b) { return a.front() == b.front(); })
        .for_each([&](auto &&chnk) {
            std::vector<long long> tempCollided;
            chnk.for_each([&](auto &&chnkItem) {
                if (not collided.contains(chnkItem[1]) && not collided.contains(chnkItem[2])) {
                    tempCollided.push_back(chnkItem[1]);
                    tempCollided.push_back(chnkItem[2]);
                }
            });
            for (auto &tmpColItem : tempCollided) { collided.emplace(tmpColItem); }
        });

    long long ans = 0;
    for (long long i = 0; i < data.size(); ++i) {
        if (not collided.contains(i)) { ans++; }
    }
    return ans;
}

} // namespace AOC2017