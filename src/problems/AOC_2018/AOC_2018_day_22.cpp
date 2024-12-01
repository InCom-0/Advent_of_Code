#include "incom_commons.h"
#include <ankerl/unordered_dense.h>
#include <climits>
#include <incom_omni.h>
#include <queue>
#include <string>


namespace AOC2018 {
long long day22_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;

    auto input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    long long depth = std::stoll(input.front().front());

    long long eroModulo  = 20183;
    long long typeModulo = 3;
    long long riskAccu   = 0;

    incom::omni::MD_ChunkVector<long long, 2, LLONG_MIN, 8> md_cv;

    using KT = decltype(md_cv)::Key_Type;

    KT const targetLoc{std::stoll(input.back().back()), std::stoll(input.back().front())};
    KT const entranceLoc{0, 0};

    KT cur_loc{0, 0};

    auto &pos0  = md_cv.get(cur_loc);
    pos0        = depth % eroModulo;
    riskAccu   += (pos0 % typeModulo);

    for (int i = 1; i <= targetLoc.back(); ++i) {
        cur_loc.back()  = i;
        auto &posXaxis  = md_cv.get(cur_loc);
        posXaxis        = ((i * 16807) + depth) % eroModulo;
        riskAccu       += (posXaxis % typeModulo);
    }
    cur_loc = {0, 0};

    for (int i = 1; i <= targetLoc.front(); ++i) {
        cur_loc.front()  = i;
        auto &posYaxis   = md_cv.get(cur_loc);
        posYaxis         = ((i * 48271) + depth) % eroModulo;
        riskAccu        += (posYaxis % typeModulo);
    }
    cur_loc = {0, 0};


    for (int x = 1; x <= targetLoc.back(); ++x) {
        cur_loc.back() = x;
        for (int y = 1; y <= targetLoc.front(); ++y) {
            if (y + x == (targetLoc.back() + targetLoc.front())) { return riskAccu; }
            cur_loc.front() = y;
            KT locAbove{cur_loc.front() - 1, cur_loc.back()};
            KT locLeft{cur_loc.front(), cur_loc.back() - 1};

            auto &pos  = md_cv.get(cur_loc);
            pos        = ((md_cv.get(locAbove) * md_cv.get(locLeft)) + depth) % eroModulo;
            riskAccu  += pos % typeModulo;
        }
    }


    return riskAccu;
}

long long day22_2(std::string dataFile) {

    // TYPE DEFINITIONS
    struct Room {
        long long erosionIndex = LLONG_MIN;
        int       roomType     = INT_MIN; // INT_MIN = default, 0 = rocky, 1 = wet, 2 = narrow

        bool isToolXValid(int toolX) {
            if (roomType == toolX) { return true; }
            else if ((roomType + 1) % 3 == toolX) { return true; }
            else { return false; }
            std::unreachable();
        }
        int getTheOtherValidTool(int validTool) {
            if (roomType == validTool) { return (validTool + 1) % 3; }
            else { return roomType; }
        };
    };


    // DATA PREP
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    long long                                    depth      = std::stoll(input.front().front());
    long long                                    eroModulo  = 20183;
    long long                                    typeModulo = 3;
    std::vector<std::pair<long long, long long>> dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    incom::omni::MD_ChunkVector<Room, 2, Room{}, 8> md_cv;
    using KT = decltype(md_cv)::Key_Type;


    KT const targetLoc{std::stoll(input.back().back()), std::stoll(input.back().front())};
    KT const entranceLoc{0, 0};

    auto &entranceRoom        = md_cv.get(entranceLoc);
    entranceRoom.erosionIndex = depth % eroModulo;
    entranceRoom.roomType     = entranceRoom.erosionIndex % typeModulo;

    auto &targetRoom        = md_cv.get(targetLoc);
    targetRoom.erosionIndex = depth % eroModulo;
    targetRoom.roomType     = targetRoom.erosionIndex % typeModulo;

    auto cmp = [](auto &&a, auto &&b) { return a.second.back() > b.second.back(); };

    std::priority_queue<std::pair<KT, KT>, std::vector<std::pair<KT, KT>>, decltype(cmp)> roomsBeingEvaluated;
    ankerl::unordered_dense::set<std::pair<KT, int>, incom::commons::XXH3Hasher>          seen;

    roomsBeingEvaluated.push({entranceLoc, {0, 0}});

    // LAMBDAS
    auto calcRoomParameters = [&](this auto &self, KT const &roomKey, Room &rm) -> void {
        if (roomKey[0] == 0) { rm.erosionIndex = ((roomKey[1] * 16807) + depth) % eroModulo; }
        else if (roomKey[1] == 0) { rm.erosionIndex = ((roomKey[0] * 48271) + depth) % eroModulo; }
        else {
            KT locAbove{roomKey.front() - 1, roomKey.back()};
            KT locLeft{roomKey.front(), roomKey.back() - 1};

            auto &rmAbove = md_cv.get(locAbove);
            if (rmAbove.erosionIndex == LLONG_MIN) { self(locAbove, rmAbove); }

            auto &rmLeft = md_cv.get(locLeft);
            if (rmLeft.erosionIndex == LLONG_MIN) { self(locLeft, rmLeft); }

            rm.erosionIndex =
                ((md_cv.get(locAbove).erosionIndex * md_cv.get(locLeft).erosionIndex) + depth) % eroModulo;
        }
        rm.roomType = rm.erosionIndex % typeModulo;
    };

    auto trySurroundings = [&](KT const &roomKey, KT &toolDistance) {
        for (auto &dir : dirs) {
            KT loc = {roomKey.front() + dir.first, roomKey.back() + dir.second};

            if (loc[0] >= 0 && loc[1] >= 0) {
                if (not seen.contains({loc, toolDistance.front()})) {
                    auto &roomInCV = md_cv.get(loc);
                    if (roomInCV.erosionIndex == LLONG_MIN) { calcRoomParameters(loc, roomInCV); }
                    if (roomInCV.isToolXValid(toolDistance.front())) {
                        roomsBeingEvaluated.push({loc, {toolDistance.front(), toolDistance.back() + 1}});
                    }
                }
            }
        }
    };

    auto executeRoom = [&](std::pair<KT, KT> &curRef) {
        if (not seen.contains({curRef.first, curRef.second.front()})) {
            trySurroundings(curRef.first, curRef.second);
            seen.insert({curRef.first, curRef.second.front()});

            auto &rm               = md_cv.get(curRef.first);
            curRef.second.back()  += 7;
            curRef.second.front()  = rm.getTheOtherValidTool(curRef.second.front());

            if (not seen.contains({curRef.first, curRef.second.front()})) {
                roomsBeingEvaluated.push({curRef.first, curRef.second});
            }
        }
    };

    // MAIN LOGIC

    while (true) {

        std::pair<KT, KT> cur = roomsBeingEvaluated.top();
        roomsBeingEvaluated.pop();

        if (cur.first == targetLoc && cur.second.front() == 0) { return cur.second.back(); }

        executeRoom(cur);
    }


    return -999;
}
} // namespace AOC2018