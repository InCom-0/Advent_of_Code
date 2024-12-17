#include <ankerl/unordered_dense.h>
#include <array>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <vector>


namespace AOC2024 {
namespace incc = incom::commons;
long long day16_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::pair<int, int>                         end;

    struct RoutePoint {
        size_t       posR;
        size_t       posC;
        size_t       curScore;
        unsigned int dir : 2 = 0;
    };

    incc::doubleBuffer<std::vector<RoutePoint>> frontierBuf;
    incc::doubleBuffer<std::vector<RoutePoint>> frontierBufAfterRot;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = 0; j < input[i].size(); ++j) {
            if (input[i][j] == 'S') { frontierBuf.getCurrent().push_back({i, j, 0, 1}); }
            else if (input[i][j] == 'E') { end = {i, j}; }
        }
    }
    size_t minScoreReached = SIZE_MAX;

    std::vector tracking(input.size(), std::vector(input.front().size(), SIZE_MAX));
    tracking[frontierBuf.getCurrent().front().posR][frontierBuf.getCurrent().front().posC] = 0;

    while ((not frontierBuf.getCurrent().empty()) || (not frontierBufAfterRot.getCurrent().empty())) {
        for (auto &rp : frontierBuf.getCurrent()) {
            rp.curScore += 1000;
            rp.dir--;
            frontierBufAfterRot.getNext().push_back(rp);
            rp.dir += 2;
            frontierBufAfterRot.getNext().push_back(rp);
            rp.dir--;
            rp.curScore -= 999;
            rp.posR     += dirs[rp.dir].front();
            rp.posC     += dirs[rp.dir].back();
            if (rp.posR == end.first && rp.posC == end.second) {
                minScoreReached = std::min(minScoreReached, rp.curScore);
            }
            else if (tracking[rp.posR][rp.posC] > rp.curScore && input[rp.posR][rp.posC] != '#' &&
                     rp.curScore < minScoreReached) {
                tracking[rp.posR][rp.posC] = rp.curScore;
                frontierBuf.getNext().push_back(rp);
            }
        }
        for (auto &rp : frontierBufAfterRot.getCurrent()) {
            rp.curScore += 1;
            rp.posR     += dirs[rp.dir].front();
            rp.posC     += dirs[rp.dir].back();
            if (rp.posR == end.first && rp.posC == end.second) {
                minScoreReached = std::min(minScoreReached, rp.curScore);
            }
            else if (tracking[rp.posR][rp.posC] > rp.curScore && input[rp.posR][rp.posC] != '#' &&
                     rp.curScore < minScoreReached) {
                tracking[rp.posR][rp.posC] = rp.curScore;
                frontierBuf.getNext().push_back(rp);
            }
        }

        frontierBuf.getCurrent().clear();
        frontierBuf.swapBuffers();
        frontierBufAfterRot.getCurrent().clear();
        frontierBufAfterRot.swapBuffers();
    }

    return minScoreReached;
}


long long day16_2(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::pair<int, int>                         end;

    struct RoutePoint {
        size_t       posR;
        size_t       posC;
        size_t       curScore;
        unsigned int dir : 2 = 0;
    };

    incc::doubleBuffer<std::vector<std::vector<RoutePoint>>> frontierBuf;
    incc::doubleBuffer<std::vector<std::vector<RoutePoint>>> frontierBufAfterRot;
    std::vector<std::vector<RoutePoint>>                     potentialRes;

    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = 0; j < input[i].size(); ++j) {
            if (input[i][j] == 'S') {
                frontierBuf.getCurrent().push_back(std::vector<RoutePoint>(1, RoutePoint{i, j, 0, 1}));
            }
            else if (input[i][j] == 'E') { end = {i, j}; }
        }
    }
    size_t minScoreReached = SIZE_MAX;

    std::vector tracking(input.size(), std::vector(input.front().size(), SIZE_MAX));
    tracking[frontierBuf.getCurrent().front().front().posR][frontierBuf.getCurrent().front().front().posC] = 0;

    while ((not frontierBuf.getCurrent().empty()) || (not frontierBufAfterRot.getCurrent().empty())) {
        for (auto &v_rp : frontierBuf.getCurrent()) {
            v_rp.push_back(v_rp.back());
            v_rp.back().curScore += 1000;
            v_rp.back().dir--;
            frontierBufAfterRot.getNext().push_back(v_rp);
            v_rp.back().dir += 2;
            frontierBufAfterRot.getNext().push_back(v_rp);
            v_rp.back().dir--;
            v_rp.back().curScore -= 999;

            v_rp.back().posR += dirs[v_rp.back().dir].front();
            v_rp.back().posC += dirs[v_rp.back().dir].back();
            if (v_rp.back().posR == end.first && v_rp.back().posC == end.second) {
                if (minScoreReached > v_rp.back().curScore) {
                    potentialRes.clear();
                    potentialRes.push_back(v_rp);
                    minScoreReached = v_rp.back().curScore;
                }
                else if (minScoreReached == v_rp.back().curScore) { potentialRes.push_back(v_rp); }
            }
            else if (tracking[v_rp.back().posR][v_rp.back().posC] >= v_rp.back().curScore - 1000 &&
                     input[v_rp.back().posR][v_rp.back().posC] != '#' && v_rp.back().curScore < minScoreReached) {
                tracking[v_rp.back().posR][v_rp.back().posC] = v_rp.back().curScore;
                frontierBuf.getNext().push_back(v_rp);
            }
        }
        for (auto &v_rp : frontierBufAfterRot.getCurrent()) {

            v_rp.back().curScore += 1;
            v_rp.back().posR     += dirs[v_rp.back().dir].front();
            v_rp.back().posC     += dirs[v_rp.back().dir].back();
            if (v_rp.back().posR == end.first && v_rp.back().posC == end.second) {
                if (minScoreReached > v_rp.back().curScore) {
                    potentialRes.clear();
                    potentialRes.push_back(v_rp);
                    minScoreReached = v_rp.back().curScore;
                }
                else if (minScoreReached == v_rp.back().curScore) { potentialRes.push_back(v_rp); }
            }
            else if (tracking[v_rp.back().posR][v_rp.back().posC] >= v_rp.back().curScore - 1000 &&
                     input[v_rp.back().posR][v_rp.back().posC] != '#' && v_rp.back().curScore < minScoreReached) {
                tracking[v_rp.back().posR][v_rp.back().posC] = v_rp.back().curScore;
                frontierBuf.getNext().push_back(v_rp);
            }
        }

        frontierBuf.getCurrent().clear();
        frontierBuf.swapBuffers();
        frontierBufAfterRot.getCurrent().clear();
        frontierBufAfterRot.swapBuffers();
    }

    ankerl::unordered_dense::set<std::pair<size_t, size_t>, incc::XXH3Hasher> resSet;
    for (auto const &resVect : potentialRes) {
        for (auto const &resVectRP : resVect) { resSet.insert({resVectRP.posR, resVectRP.posC}); }
    }

    return resSet.size();
}
} // namespace AOC2024