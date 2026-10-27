#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/hashing.hpp>
#include <incstd/polyfills/mdspan.hpp>
#include <incstd/solvers/explorers.hpp>
#include <limits>
#include <ranges>
#include <string>
#include <utility>


namespace AOC2023 {

auto
day22_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(\d+)">;

    auto res = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, any_ctre);
    return std::views::transform(res,
                                 [](auto const &resLine) {
                                     return std::views::transform(resLine,
                                                                  [](auto const &item) { return std::stoull(item); }) |
                                            std::ranges::to<std::vector>();
                                 }) |
           std::ranges::to<std::vector>();
}

size_t
day22_1(std::string dataFile) {
    auto input = day22_0(dataFile);

    size_t maxY{}, maxX{}, maxZ{};
    for (auto const &oneBrick : input) {
        for (size_t i{}; i < 4; i += 3) {
            maxY = std::max(maxY, oneBrick.at(0 + i));
            maxX = std::max(maxX, oneBrick.at(1 + i));
            maxZ = std::max(maxZ, oneBrick.at(2 + i));
        }
    }
    maxY++, maxX++, maxZ++;

    constexpr auto SZ_MAX = std::numeric_limits<size_t>::max();

    std::vector<size_t> grid(maxY * maxX * maxZ, SZ_MAX);
    incpf::mdspan       mdspnOfGrid(grid.data(), maxY, maxX, maxZ);

    std::vector<std::vector<std::array<size_t, 3uz>>> cubesInBricks;
    cubesInBricks.reserve(input.size());

    std::vector<std::vector<size_t>> supportedBy(input.size(), std::vector<size_t>{});

    for (size_t brickID{}; auto const &inpBrick : input) {
        ankerl::unordered_dense::set<std::array<size_t, 3uz>, incstd::hashing::XXH3Hasher> st;
        for (size_t yID = inpBrick.at(0); yID < inpBrick.at(3) + 1; ++yID) {
            st.insert({yID, inpBrick.at(1), inpBrick.at(2)});
        }
        for (size_t xID = inpBrick.at(1); xID < inpBrick.at(4) + 1; ++xID) {
            st.insert({inpBrick.at(0), xID, inpBrick.at(2)});
        }
        for (size_t zID = inpBrick.at(2); zID < inpBrick.at(5) + 1; ++zID) {
            st.insert({inpBrick.at(0), inpBrick.at(1), zID});
        }
        cubesInBricks.push_back(st | std::ranges::to<std::vector>());

        for (auto const &cube : cubesInBricks.back()) { mdspnOfGrid[cube] = brickID; }
        brickID++;
    }

    auto find_settleBy = [&](std::vector<std::array<size_t, 3uz>> const &posX) {
        if (posX.empty()) { return 0uz; }

        size_t      maxDownSettle = std::numeric_limits<size_t>::max();
        auto const &brickID       = mdspnOfGrid[posX.front()[0], posX.front()[1], posX.front()[2]];

        for (auto onePos : posX) {
            size_t thisSettle{};
            while (onePos[2] > 0) {
                onePos[2]--;
                if (onePos[2] > 0 && (mdspnOfGrid[onePos] == SZ_MAX || mdspnOfGrid[onePos] == brickID)) {
                    thisSettle++;
                }
                else { break; }
            }
            if (thisSettle < maxDownSettle) { supportedBy.at(brickID).clear(); }

            if (thisSettle <= maxDownSettle) {
                if (mdspnOfGrid[onePos] != SZ_MAX &&
                    std::ranges::find(supportedBy.at(brickID), mdspnOfGrid[onePos[0], onePos[1], onePos[2]]) ==
                        supportedBy.at(brickID).end()) {
                    supportedBy.at(brickID).push_back(mdspnOfGrid[onePos]);
                }
                maxDownSettle = thisSettle;
            }
        }
    RET:
        return maxDownSettle;
    };

    auto settleBy = [&](std::vector<std::array<size_t, 3uz>> &posX, size_t const by) {
        size_t totalSettleSteps{};
        if (posX.empty() || by == 0uz) { goto RET; }

        for (auto const &onePos : posX) {
            mdspnOfGrid[onePos[0], onePos[1], onePos[2] - by] = mdspnOfGrid[onePos[0], onePos[1], onePos[2]];
            mdspnOfGrid[onePos[0], onePos[1], onePos[2]]      = SZ_MAX;
        }
        for (auto &onePos : posX) { onePos[2] -= by; }
    RET:
        return totalSettleSteps;
    };
    for (size_t z{}; z < maxZ; ++z) {
        for (size_t y{}; y < maxY; ++y) {
            for (size_t x{}; x < maxX; ++x) {
                if (mdspnOfGrid[y, x, z] != SZ_MAX) {
                    std::vector<std::array<size_t, 3uz>> &oneBrickCubexPosX = cubesInBricks.at(mdspnOfGrid[y, x, z]);
                    settleBy(oneBrickCubexPosX, find_settleBy(oneBrickCubexPosX));
                }
            }
        }
    }

    std::vector<size_t> counter(input.size(), 0uz);
    for (auto const &sb : supportedBy) {
        if (sb.size() < 2) {
            for (auto const supporting : sb) { counter.at(supporting)++; }
        }
    }


    return std::ranges::count(counter, 0uz);
}

size_t
day22_2(std::string dataFile) {
    auto input = day22_0(dataFile);

    size_t maxY{}, maxX{}, maxZ{};
    for (auto const &oneBrick : input) {
        for (size_t i{}; i < 4; i += 3) {
            maxY = std::max(maxY, oneBrick.at(0 + i));
            maxX = std::max(maxX, oneBrick.at(1 + i));
            maxZ = std::max(maxZ, oneBrick.at(2 + i));
        }
    }
    maxY++, maxX++, maxZ++;

    constexpr auto SZ_MAX = std::numeric_limits<size_t>::max();

    std::vector<size_t> grid(maxY * maxX * maxZ, SZ_MAX);
    incpf::mdspan       mdspnOfGrid(grid.data(), maxY, maxX, maxZ);

    std::vector<std::vector<std::array<size_t, 3uz>>> cubesInBricks;
    cubesInBricks.reserve(input.size());

    std::vector<std::vector<size_t>> supportedBy(input.size(), std::vector<size_t>{});
    std::vector<size_t>              supportedBy_count(input.size(), 0uz);

    for (size_t brickID{}; auto const &inpBrick : input) {
        ankerl::unordered_dense::set<std::array<size_t, 3uz>, incstd::hashing::XXH3Hasher> st;
        for (size_t yID = inpBrick.at(0); yID < inpBrick.at(3) + 1; ++yID) {
            st.insert({yID, inpBrick.at(1), inpBrick.at(2)});
        }
        for (size_t xID = inpBrick.at(1); xID < inpBrick.at(4) + 1; ++xID) {
            st.insert({inpBrick.at(0), xID, inpBrick.at(2)});
        }
        for (size_t zID = inpBrick.at(2); zID < inpBrick.at(5) + 1; ++zID) {
            st.insert({inpBrick.at(0), inpBrick.at(1), zID});
        }
        cubesInBricks.push_back(st | std::ranges::to<std::vector>());

        for (auto const &cube : cubesInBricks.back()) { mdspnOfGrid[cube] = brickID; }
        brickID++;
    }

    auto find_settleBy = [&](std::vector<std::array<size_t, 3uz>> const &posX) {
        if (posX.empty()) { return 0uz; }

        size_t      maxDownSettle = std::numeric_limits<size_t>::max();
        auto const &brickID       = mdspnOfGrid[posX.front()[0], posX.front()[1], posX.front()[2]];

        for (auto onePos : posX) {
            size_t thisSettle{};
            while (onePos[2] > 0) {
                onePos[2]--;
                if (onePos[2] > 0 && (mdspnOfGrid[onePos] == SZ_MAX || mdspnOfGrid[onePos] == brickID)) {
                    thisSettle++;
                }
                else { break; }
            }
            if (thisSettle <= maxDownSettle) {
                if (thisSettle < maxDownSettle) { supportedBy.at(brickID).clear(); }

                if (mdspnOfGrid[onePos] != SZ_MAX &&
                    std::ranges::find(supportedBy.at(brickID), mdspnOfGrid[onePos]) == supportedBy.at(brickID).end()) {
                    supportedBy.at(brickID).push_back(mdspnOfGrid[onePos]);
                }
                maxDownSettle = thisSettle;
            }
        }
        supportedBy_count.at(brickID) = supportedBy.at(brickID).size();
    RET:
        return maxDownSettle;
    };

    auto settleBy = [&](std::vector<std::array<size_t, 3uz>> &posX, size_t const by) {
        if (posX.empty() || by == 0uz) { return; }

        for (auto const &onePos : posX) {
            mdspnOfGrid[onePos[0], onePos[1], onePos[2] - by] = mdspnOfGrid[onePos[0], onePos[1], onePos[2]];
            mdspnOfGrid[onePos[0], onePos[1], onePos[2]]      = SZ_MAX;
        }
        for (auto &onePos : posX) { onePos[2] -= by; }
    };

    // Settling main loops
    for (size_t z{}; z < maxZ; ++z) {
        for (size_t y{}; y < maxY; ++y) {
            for (size_t x{}; x < maxX; ++x) {
                if (mdspnOfGrid[y, x, z] != SZ_MAX) {
                    std::vector<std::array<size_t, 3uz>> &oneBrickCubexPosX = cubesInBricks.at(mdspnOfGrid[y, x, z]);
                    settleBy(oneBrickCubexPosX, find_settleBy(oneBrickCubexPosX));
                }
            }
        }
    }

    std::vector<std::vector<size_t>> bricksAbove(input.size(), std::vector<size_t>{});
    for (size_t supported_id{}; auto const &suppBricks : supportedBy) {
        for (auto const suppB : suppBricks) { bricksAbove.at(suppB).push_back(supported_id); }
        supported_id++;
    }

    size_t              accuRes{};
    std::vector<size_t> supportedBy_count_cpy;

    auto recu = [&](this auto const &self, size_t const idToEva) -> size_t {
        size_t locRes{};
        for (auto const &supportedID : bricksAbove.at(idToEva)) {
            if (supportedBy_count_cpy.at(supportedID) > 1) { supportedBy_count_cpy.at(supportedID)--; }
            else if (supportedBy_count_cpy.at(supportedID) == 1) {
                supportedBy_count_cpy.at(supportedID)--;
                locRes++;
                locRes += self(supportedID);
            }
        }
        return locRes;
    };

    for (size_t supportingID{}; auto const &supportedIDX : bricksAbove) {
        supportedBy_count_cpy  = supportedBy_count;
        accuRes               += recu(supportingID++);
    }


    return accuRes;
}

} // namespace AOC2023