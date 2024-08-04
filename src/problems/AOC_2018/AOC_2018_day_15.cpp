#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2018 {

int day15_1(std::string dataFile) {
    // DEFINITIONS
    struct Pawn {
        int  yPos;
        int  xPos;
        char type;
        char enemyType;
        int  HP  = 200;
        int  Pow = 3;

        void changePosTo(const int &y, const int &x) {
            yPos = y;
            xPos = x;
        }
        void strike(Pawn &otherPawn) { otherPawn.HP -= Pow; }
        bool test_dead() { return HP < 1; }
    };

    auto all_ctre = ctre::search<R"(.+)">;
    auto map      = AOC_commons::parseInputUsingCTRE::processFile(dataFile, all_ctre).front();

    std::vector<Pawn>                         goblins;
    std::vector<Pawn>                         elves;
    std::vector<std::reference_wrapper<Pawn>> ordering;

    std::vector<std::pair<int, int>> dirs{{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

    // DATA PREP
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == 'G') { goblins.push_back({i, j, 'G', 'E'}); }
            else if (map[i][j] == 'E') { elves.push_back({i, j, 'E', 'G'}); }
        }
    }
    int goblinsAlive = 0;
    int elvesAlive   = 0;
    for (auto &goblin : goblins) {
        ordering.push_back(goblin);
        goblinsAlive++;
    }
    for (auto &elf : elves) {
        ordering.push_back(elf);
        elvesAlive++;
    }

    // LAMBDA DEFINITIONS
    int  round = 1;
    Pawn fakePawn{INT_MIN, INT_MIN, 'X', 'X', INT_MAX, INT_MIN};
    auto movePawnTo = [&](Pawn &pawn, const int &y, const int &x) {
        map[pawn.yPos][pawn.xPos] = '.';
        map[y][x]                 = pawn.type;
        pawn.changePosTo(y, x);
    };

    auto checkSurroundingAndAttack = [&](Pawn &pawn) {
        std::reference_wrapper<Pawn> enemyPawn = fakePawn;
        for (auto &[yDir, xDir] : dirs) {

            if (map[pawn.yPos + yDir][pawn.xPos + xDir] == pawn.enemyType) {
                auto enemyIt = std::ranges::find_if(
                    ordering, [targetYpos = pawn.yPos + yDir, targetXpos = pawn.xPos + xDir](auto &&a) {
                        return a.get().yPos == targetYpos && a.get().xPos == targetXpos;
                    });
                if (enemyIt.operator*().get().HP < enemyPawn.get().HP) { enemyPawn = enemyIt.operator*().get(); }
            }
        }
        if (enemyPawn.get().type != 'X') {
            pawn.strike(enemyPawn.get());
            if (enemyPawn.get().test_dead()) {
                map[enemyPawn.get().yPos][enemyPawn.get().xPos] = '.';
                if (enemyPawn.get().type == 'G') { goblinsAlive--; }
                else { elvesAlive--; }
            }
        }
    };

    auto explore = [&](const Pawn &pawn) -> std::pair<int, int> {
        // Track 'tracks' the positions explored in each BFS level
        // Naturally the 0-level is the current pawn position.
        std::vector<std::vector<std::pair<int, int>>> track(1,
                                                            std::vector<std::pair<int, int>>{{pawn.yPos, pawn.xPos}});

        // TrackIDs track from which position in previous BFS level
        // This effectivelly provides a 'backwards' link to backtrack when a target is found.
        std::vector<std::vector<int>> trackIDs(1, std::vector<int>{INT_MAX});

        // Temporary pair (because we need to explore the entire BFS level to see if there isn't another target closer
        // to top left corner).
        std::pair<int, int> foundOne{INT_MAX, INT_MAX};

        // Some helper variables
        int  lvl           = -1;
        int  foundID       = INT_MIN;
        bool notFound      = true;
        bool moreExpansion = true;

        while (notFound and moreExpansion) {
            lvl++;

            // reached another level, pushing back a new 'line' is each vector.
            track.push_back({});
            trackIDs.push_back({});

            // if no new position is found AND no target is found, it must mean we can't move ... this variable stays
            // true and breaks the while loop.
            moreExpansion = false;
            for (int i = 0; i < track[lvl].size(); ++i) {
                for (auto &[yDir, xDir] : dirs) {
                    if (map[track[lvl][i].first + yDir][track[lvl][i].second + xDir] == '.') {
                        track[lvl + 1].push_back({track[lvl][i].first + yDir, track[lvl][i].second + xDir});
                        trackIDs[lvl + 1].push_back(i);
                        map[track[lvl][i].first + yDir][track[lvl][i].second + xDir] = 'X';
                        moreExpansion                                                = true;
                    }
                    else if (map[track[lvl][i].first + yDir][track[lvl][i].second + xDir] == pawn.enemyType) {
                        if (track[lvl][i].first < foundOne.first) {
                            foundOne.first  = track[lvl][i].first;
                            foundOne.second = track[lvl][i].second;
                            foundID         = i;
                        }
                        else if (track[lvl][i].first == foundOne.first) {
                            if (track[lvl][i].second < foundOne.second) {
                                foundOne.first  = track[lvl][i].first;
                                foundOne.second = track[lvl][i].second;
                                foundID         = i;
                            }
                        }
                        moreExpansion = true;
                        notFound =
                            false; // reached the target, will break the while loop when reaching its conditional.
                    }
                }
            }
        }
        if (moreExpansion) {
            for (int i = lvl; i > 1; --i) { foundID = trackIDs[i][foundID]; }
        }

        // Reset the map to the original state by replacing all 'explored' positions with '.'
        for (auto &lvl : track) {
            for (auto &pr : lvl) { map[pr.first][pr.second] = '.'; }
        }
        // Also return the original position to the original pawn type char.
        map[track[0][0].first][track[0][0].second] = pawn.type;

        // If we can't move return '0, 0'.
        if (not moreExpansion) { return std::make_pair(0, 0); }
        else {
            // Return the correct move direction defined as 'found position' minus 'original pawn position'.
            return std::make_pair((int)(track[1][foundID].first) - pawn.yPos,
                                  (int)(track[1][foundID].second) - pawn.xPos);
        }
    };

    auto findPathAndReturnFirstStep = [&](const Pawn &pawn) -> std::pair<int, int> {
        // If some enemy nearby,  no step (or more accurately 0,0 move).
        for (auto &[yDir, xDir] : dirs) {
            if (map[pawn.yPos + yDir][pawn.xPos + xDir] == pawn.enemyType) { return std::make_pair(0, 0); }
        }
        return explore(pawn);
    };

    auto cleanDeadPawns = [&]() { std::erase_if(ordering, [](auto &&a) { return a.get().HP < 1; }); };

    // MAIN LOGIC
    int counter = 0;
    while (goblinsAlive != 0 && elvesAlive != 0) {
        std::ranges::sort(ordering, [](auto &&a, auto &&b) {
            if (a.get().yPos == b.get().yPos) { return a.get().xPos < b.get().xPos; }
            else { return a.get().yPos < b.get().yPos; }
        });
        int counter2 = 0;
        for (auto &pawnRef : ordering) {
            if (pawnRef.get().test_dead()) { continue; }

            auto [dirY, dirX]                            = findPathAndReturnFirstStep(pawnRef.get());
            map[pawnRef.get().yPos][pawnRef.get().xPos]  = '.';
            pawnRef.get().yPos                          += dirY;
            pawnRef.get().xPos                          += dirX;
            map[pawnRef.get().yPos][pawnRef.get().xPos]  = pawnRef.get().type;

            checkSurroundingAndAttack(pawnRef.get());
            if (goblinsAlive == 0 || elvesAlive == 0) { break; }
            counter2++;
        }
        cleanDeadPawns();
        if (goblinsAlive == 0 || elvesAlive == 0) { break; }
        counter++;
    }
    return counter * (std::ranges::fold_left(ordering, 0, [](auto &&ini, auto &&pawn) { return ini + pawn.get().HP; }));
}

int day15_2(std::string dataFile) {
    // DEFINITIONS
    struct Pawn {
        int  yPos;
        int  xPos;
        char type;
        char enemyType;
        int  HP  = 200;
        int  Pow = 3;

        void changePosTo(const int &y, const int &x) {
            yPos = y;
            xPos = x;
        }
        void strike(Pawn &otherPawn) { otherPawn.HP -= Pow; }
        bool test_dead() { return HP < 1; }
    };

    auto all_ctre = ctre::search<R"(.+)">;
    auto mapOrig  = AOC_commons::parseInputUsingCTRE::processFile(dataFile, all_ctre).front();
    auto map      = mapOrig;

    std::vector<Pawn>                         goblins;
    std::vector<Pawn>                         elves;
    std::vector<std::reference_wrapper<Pawn>> ordering;

    std::vector<std::pair<int, int>> dirs{{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

    int  goblinsAlive = 0;
    int  elvesAlive   = 0;
    Pawn fakePawn{INT_MIN, INT_MIN, 'X', 'X', INT_MAX, INT_MIN};

    // LAMBDA DEFINITIONS
    auto movePawnTo = [&](Pawn &pawn, const int &y, const int &x) {
        map[pawn.yPos][pawn.xPos] = '.';
        map[y][x]                 = pawn.type;
        pawn.changePosTo(y, x);
    };

    auto checkSurroundingAndAttack = [&](Pawn &pawn) {
        std::reference_wrapper<Pawn> enemyPawn = fakePawn;
        for (auto &[yDir, xDir] : dirs) {
            if (map[pawn.yPos + yDir][pawn.xPos + xDir] == pawn.enemyType) {
                auto enemyIt = std::ranges::find_if(
                    ordering, [targetYpos = pawn.yPos + yDir, targetXpos = pawn.xPos + xDir](auto &&a) {
                        return a.get().yPos == targetYpos && a.get().xPos == targetXpos;
                    });
                if (enemyIt.operator*().get().HP < enemyPawn.get().HP) { enemyPawn = enemyIt.operator*().get(); }
            }
        }
        if (enemyPawn.get().type != 'X') {
            pawn.strike(enemyPawn.get());
            if (enemyPawn.get().test_dead()) {
                map[enemyPawn.get().yPos][enemyPawn.get().xPos] = '.';
                if (enemyPawn.get().type == 'G') { goblinsAlive--; }
                else { elvesAlive--; }
            }
        }
    };

    auto explore = [&](const Pawn &pawn) -> std::pair<int, int> {
        // Track 'tracks' the positions explored in each BFS level
        // Naturally the 0-level is the current pawn position.
        std::vector<std::vector<std::pair<int, int>>> track(1,
                                                            std::vector<std::pair<int, int>>{{pawn.yPos, pawn.xPos}});

        // TrackIDs track from which position in previous BFS level
        // This effectivelly provides a 'backwards' link to backtrack when a target is found.
        std::vector<std::vector<int>> trackIDs(1, std::vector<int>{INT_MAX});

        // Temporary pair (because we need to explore the entire BFS level to see if there isn't another target closer
        // in reading order).
        std::pair<int, int> foundOne{INT_MAX, INT_MAX};

        // Some helper variables
        int  lvl           = -1;
        int  foundID       = INT_MIN;
        bool notFound      = true;
        bool moreExpansion = true;

        while (notFound and moreExpansion) {
            lvl++;

            // reached another level, pushing back a new 'line' is each vector.
            track.push_back({});
            trackIDs.push_back({});

            // if no new position is found AND no target is found, it must mean we can't move ... this variable stays
            // true and breaks the while loop.
            moreExpansion = false;
            for (int i = 0; i < track[lvl].size(); ++i) {
                for (auto &[yDir, xDir] : dirs) {
                    if (map[track[lvl][i].first + yDir][track[lvl][i].second + xDir] == '.') {
                        track[lvl + 1].push_back({track[lvl][i].first + yDir, track[lvl][i].second + xDir});
                        trackIDs[lvl + 1].push_back(i);
                        map[track[lvl][i].first + yDir][track[lvl][i].second + xDir] = 'X';
                        moreExpansion                                                = true;
                    }
                    else if (map[track[lvl][i].first + yDir][track[lvl][i].second + xDir] == pawn.enemyType) {
                        if (track[lvl][i].first < foundOne.first) {
                            foundOne.first  = track[lvl][i].first;
                            foundOne.second = track[lvl][i].second;
                            foundID         = i;
                        }
                        else if (track[lvl][i].first == foundOne.first) {
                            if (track[lvl][i].second < foundOne.second) {
                                foundOne.first  = track[lvl][i].first;
                                foundOne.second = track[lvl][i].second;
                                foundID         = i;
                            }
                        }
                        moreExpansion = true;
                        // reached the target, will break the while loop when reaching its conditional.
                        notFound      = false;
                    }
                }
            }
        }
        if (moreExpansion) {
            for (int i = lvl; i > 1; --i) { foundID = trackIDs[i][foundID]; }
        }

        // Reset the map to the original state by replacing all 'explored' positions with '.'
        for (auto &lvl : track) {
            for (auto &pr : lvl) { map[pr.first][pr.second] = '.'; }
        }
        // Also return the original position to the original pawn type char.
        map[track[0][0].first][track[0][0].second] = pawn.type;

        // If we can't move return '0, 0'.
        if (not moreExpansion) { return std::make_pair(0, 0); }
        else {
            // Return the correct move direction defined as 'found position' minus 'original pawn position'.
            return std::make_pair(track[1][foundID].first - pawn.yPos, track[1][foundID].second - pawn.xPos);
        }
    };

    auto findPathAndReturnFirstStep = [&](const Pawn &pawn) -> std::pair<int, int> {
        // If some enemy nearby,  no step (or more accurately 0,0 move).
        for (auto &[yDir, xDir] : dirs) {
            if (map[pawn.yPos + yDir][pawn.xPos + xDir] == pawn.enemyType) { return std::make_pair(0, 0); }
        }
        return explore(pawn);
    };

    auto cleanDeadPawns = [&]() { std::erase_if(ordering, [](auto &&a) { return a.get().HP < 1; }); };

    // MAIN LOGIC
    int elvesPOW = 2;
    do {
        map = mapOrig;
        goblins.clear();
        elves.clear();
        ordering.clear();
        goblinsAlive = 0;
        elvesAlive   = 0;
        elvesPOW++;

        for (int i = 0; i < map.size(); ++i) {
            for (int j = 0; j < map[i].size(); ++j) {
                if (map[i][j] == 'G') { goblins.push_back({i, j, 'G', 'E'}); }
                else if (map[i][j] == 'E') { elves.push_back({i, j, 'E', 'G', 200, elvesPOW}); }
            }
        }
        for (auto &goblin : goblins) {
            ordering.push_back(goblin);
            goblinsAlive++;
        }
        for (auto &elf : elves) {
            ordering.push_back(elf);
            elvesAlive++;
        }

        int maxElves = elvesAlive;
        int counter  = 0;
        while (goblinsAlive != 0 && elvesAlive != 0) {
            std::ranges::sort(ordering, [](auto &&a, auto &&b) {
                if (a.get().yPos == b.get().yPos) { return a.get().xPos < b.get().xPos; }
                else { return a.get().yPos < b.get().yPos; }
            });
            for (auto &pawnRef : ordering) {
                // Gotta provide for a situation where the round terminates before it is 'complete'
                if (goblinsAlive == 0 || elvesAlive == 0) {
                    counter--;
                    break;
                }
                if (pawnRef.get().test_dead()) { continue; }

                auto [dirY, dirX]                            = findPathAndReturnFirstStep(pawnRef.get());
                map[pawnRef.get().yPos][pawnRef.get().xPos]  = '.';
                pawnRef.get().yPos                          += dirY;
                pawnRef.get().xPos                          += dirX;
                map[pawnRef.get().yPos][pawnRef.get().xPos]  = pawnRef.get().type;

                checkSurroundingAndAttack(pawnRef.get());
            }
            counter++;
            cleanDeadPawns();
        }
        if (goblinsAlive == 0 && maxElves == elvesAlive) {
            return counter *
                   (std::ranges::fold_left(ordering, 0, [](auto &&ini, auto &&pawn) { return ini + pawn.get().HP; }));
        }
    } while (true);
}
} // namespace AOC2018