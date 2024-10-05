#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <queue>
#include <string>

#include <ankerl/unordered_dense.h>


namespace AOC2018 {

long long day23_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::array<long long, 4>> data;
    for (auto &inpLine : input) {
        data.push_back(
            {std::stoll(inpLine[0]), std::stoll(inpLine[1]), std::stoll(inpLine[2]), std::stoll(inpLine[3])});
    }

    std::array<long long, 4> strongest =
        flux::ref(data).max([](auto &&a, auto &&b) { return a.back() < b.back(); }).value();

    long long res = flux::ref(data).count_if([&](auto &&a) {
        long long accu = 0;
        for (int i = 0; i < 3; ++i) { accu += std::abs(strongest[i] - a[i]); }
        return accu <= strongest[3];
    });

    return res;
}

long long day23_2(std::string dataFile) {
    // TYPE DEFINTIIONS
    struct Box {
        std::array<long long, 3> bl_corner;
        std::array<long long, 3> tr_corner;
        long long                numOfOverlap;
    };

    struct Bot {
        std::array<long long, 3> loc;
        long long                radius;
    };

    // DATA PREP
    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<Bot> bots;
    for (auto &inpLine : input) {
        bots.push_back(
            {std::stoll(inpLine[0]), std::stoll(inpLine[1]), std::stoll(inpLine[2]), std::stoll(inpLine[3])});
    }

    auto xMinMax = flux::ref(bots).minmax([](auto &&a, auto &&b) { return a.loc[0] < b.loc[0]; });
    auto yMinMax = flux::ref(bots).minmax([](auto &&a, auto &&b) { return a.loc[1] < b.loc[1]; });
    auto zMinMax = flux::ref(bots).minmax([](auto &&a, auto &&b) { return a.loc[2] < b.loc[2]; });

    Box startingBox{Box{{xMinMax->min.loc[0], yMinMax->min.loc[1], zMinMax->min.loc[2]},
                        {xMinMax->max.loc[0], yMinMax->max.loc[1], zMinMax->max.loc[2]},
                        LLONG_MIN}};

    // LAMDA DEFINITIONS
    auto cmp = [](auto &&a, auto &&b) { return a.numOfOverlap < b.numOfOverlap; };
    std::priority_queue<Box, std::vector<Box>, decltype(cmp)> queue;

    auto computeManhDistance = [&](std::array<long long, 3> const &point_a, std::array<long long, 3> const &point_b) {
        long long res = 0;
        for (int i = 0; i < 3; ++i) { res += std::abs(point_b[i] - point_a[i]); }
        return res;
    };

    auto isOverlap = [&](Bot const &bot, Box const &box, long long const &boxDiagSize) {
        long long distToBox  = 0;
        distToBox           += computeManhDistance(bot.loc, box.tr_corner);
        distToBox           += computeManhDistance(bot.loc, box.bl_corner);

        distToBox -= boxDiagSize;
        distToBox /= 2;

        return distToBox <= bot.radius;
    };

    auto computeNumOfOverlappingBots = [&](Box const &box, std::vector<Bot> const &botsVect) -> long long {
        long long boxDiagSize         = computeManhDistance(box.bl_corner, box.tr_corner);
        long long overlappingBotsNums = 0;
        for (auto &oneBot : botsVect) {
            if (isOverlap(oneBot, box, boxDiagSize)) { overlappingBotsNums++; }
        }
        return overlappingBotsNums;
    };


    auto splitBox_8way = [&]() {
        // 8-way split of multiplier to get the right bl_corner of 8 boxes created out of 1 bigger box.
        constexpr const std::array<std::array<std::array<std::array<long long, 3>, 2>, 2>, 2> way8{
            0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1};

        Box box = queue.top();
        queue.pop();

        std::array<std::array<long long, 2>, 3> edgeSz;
        for (int i = 0; i < 3; ++i) {
            edgeSz[i][0] = (box.tr_corner[i] - box.bl_corner[i]) / 2;
            edgeSz[i][1] = (box.tr_corner[i] - box.bl_corner[i]) - edgeSz[i][0];
        }

        ankerl::unordered_dense::set<std::array<long long, 3>, incom::commons::XXH3Hasher> tempSet;
        int splitID = 0;
        
        for (int x = 0; x < 2; ++x) {
            for (int y = 0; y < 2; ++y) {
                for (int z = 0; z < 2; ++z) {
                    std::array<long long, 3> bl_c{box.bl_corner[0] + (way8[x][y][z][0] * edgeSz[0][0]),
                                                  box.bl_corner[1] + (way8[x][y][z][1] * edgeSz[1][0]),
                                                  box.bl_corner[2] + (way8[x][y][z][2] * edgeSz[2][0])};
                    std::array<long long, 3> tr_c{bl_c[0] + edgeSz[0][x], bl_c[1] + edgeSz[1][y],
                                                  bl_c[2] + edgeSz[2][z]};

                    if (tempSet.insert(bl_c).second) {
                        Box tempBox{bl_c, tr_c, LLONG_MIN};
                        tempBox.numOfOverlap = computeNumOfOverlappingBots(tempBox, bots);
                        queue.push(tempBox);
                        // if (not (box.bl_corner == tempBox.bl_corner && box.tr_corner == tempBox.tr_corner)) {
                        //
                        // }
                    }
                    splitID++;
                }
            }
        }
        return;
    };

    // MAIN LOGIC
    startingBox.numOfOverlap = computeNumOfOverlappingBots(startingBox, bots);
    queue.push(std::move(startingBox));

    // Execute until the top 'box' is actually just 'one point'
    while (queue.top().bl_corner != queue.top().tr_corner) { splitBox_8way(); }


    std::array<long long, 3> point_zero{0, 0, 0};
    return computeManhDistance(point_zero, queue.top().bl_corner);
}
} // namespace AOC2018