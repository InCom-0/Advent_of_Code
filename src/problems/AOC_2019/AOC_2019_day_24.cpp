#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.h>
#include <optional>


namespace AOC2019 {
namespace incc = incom::commons;
long long day24_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(.+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    decltype(input) inputBordered;
    inputBordered.push_back(std::string(input.front().size() + 2, '.'));
    for (auto const &line : input) {
        std::string toAdd(('.') + line);
        toAdd.push_back('.');
        inputBordered.push_back(std::move(toAdd));
    }
    inputBordered.push_back(std::string(input.front().size() + 2, '.'));

    std::vector<std::pair<int, int>> dirs{{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

    ankerl::unordered_dense::set<decltype(inputBordered), incc::XXH3Hasher> setOfPast;
    incc::doubleBuffer<decltype(inputBordered)>                             buf(inputBordered);

    auto oneInteration = [&]() {
        for (int r = 1; r < buf.getCurrent().size() - 1; ++r) {
            for (int c = 1; c < buf.getCurrent().front().size() - 1; ++c) {

                size_t count_bugAround = 0;
                for (auto const &oneDir : dirs) {
                    count_bugAround += (buf.getCurrent()[r + oneDir.first][c + oneDir.second] == '#');
                }

                if (buf.getCurrent()[r][c] == '#' && not (count_bugAround == 1)) { buf.getNext()[r][c] = '.'; }
                else if (buf.getCurrent()[r][c] == '.' && (count_bugAround == 1 or count_bugAround == 2)) {
                    buf.getNext()[r][c] = '#';
                }
                else { buf.getNext()[r][c] = buf.getCurrent()[r][c]; }
            }
        }
        buf.swapBuffers();
    };

    while (setOfPast.insert(buf.getCurrent()).second) { oneInteration(); }

    size_t valueOfCurField = 1;
    size_t accu            = 0;
    for (int r = 1; r < buf.getCurrent().size() - 1; ++r) {
        for (int c = 1; c < buf.getCurrent().front().size() - 1; ++c) {
            if (buf.getCurrent()[r][c] == '#') { accu += valueOfCurField; }
            valueOfCurField *= 2;
        }
    }
    return accu;
}

long long day24_2(std::string dataFile, size_t const iterations = 200) {

    auto d_ctre = ctre::search<R"(.+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    using KT = std::pair<int, int>;
    std::vector<KT> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // TYPE DEFS
    struct OneLevel {
        std::vector<std::string> bugMap;
        unsigned int             get_reqFromBottomLvl(KT const &key) {
            if (key.first == 1) {
                return std::ranges::count_if(bugMap.front(), [](auto &&a) { return a == '#'; });
            }
            else if (key.second == -1) {
                return std::ranges::count_if(bugMap, [](auto &&a) { return a.back() == '#'; });
            }
            else if (key.first == -1) {
                return std::ranges::count_if(bugMap.back(), [](auto &&a) { return a == '#'; });
            }
            else if (key.second == 1) {
                return std::ranges::count_if(bugMap, [](auto &&a) { return a.front() == '#'; });
            }
            else { assert(false); }
        }
        unsigned int get_reqFromTopLvl(KT const &key) {
            return (bugMap[bugMap.size() / 2 + key.first][bugMap.front().size() / 2 + key.second] == '#');
        };
    };

    struct Maze {
        OneLevel              levelZero;
        std::vector<OneLevel> levels_down = std::vector(1, OneLevel());
        std::vector<OneLevel> levels_up   = std::vector(1, OneLevel());

        auto get_vecOfBugMaps() {
            std::vector<std::vector<std::string>> res;
            for (auto iter = levels_down.rbegin(); iter != levels_down.rend() - 1; ++iter) {
                res.push_back(iter->bugMap);
            }
            res.push_back(levelZero.bugMap);
            for (auto iter = levels_up.begin() + 1; iter != levels_up.end(); ++iter) { res.push_back(iter->bugMap); }
            return res;
        }

        auto get_oneLevel(int const lvlID) -> std::optional<std::reference_wrapper<OneLevel>> {
            if (lvlID >= static_cast<int>(levels_up.size())) { return std::nullopt; }
            else if ((-lvlID) >= static_cast<int>(levels_down.size())) { return std::nullopt; }

            else if (lvlID == 0) { return std::optional<std::reference_wrapper<OneLevel>>{levelZero}; }
            else if (lvlID > 0) { return std::optional<std::reference_wrapper<OneLevel>>{levels_up[lvlID]}; }
            else { return std::optional<std::reference_wrapper<OneLevel>>{levels_down[-lvlID]}; }
        }
    };

    // PREP
    std::vector emptyBugMap(input.size(), std::string(input.front().size(), '.'));
    Maze        startMaze{OneLevel{input}, {OneLevel(), OneLevel{emptyBugMap}}, {OneLevel(), OneLevel{emptyBugMap}}};
    incc::doubleBuffer<Maze> buf(startMaze);

    int const midRow = input.size() / 2;
    int const midCol = input[midRow].size() / 2;

    // ONE ITERATION ALGO
    auto oneIteration = [&]() {
        auto &cur = buf.getCurrent();
        auto &nxt = buf.getNext();

        int bottomLvlID = cur.levels_down.size() - 1;

        // Check if we need to 'enlarge' maze 'down' and enlarge if needed
        if (std::ranges::any_of(cur.levels_down[bottomLvlID].bugMap.front(), [](auto &&a) { return a == '#'; }) ||
            std::ranges::any_of(cur.levels_down[bottomLvlID].bugMap.back(), [](auto &&a) { return a == '#'; }) ||
            std::ranges::any_of(cur.levels_down[bottomLvlID].bugMap,
                                [](auto &&a) { return a.front() == '#' || a.back() == '#'; })) {
            cur.levels_down.push_back(OneLevel{emptyBugMap});
            nxt.levels_down.push_back(OneLevel{emptyBugMap});
        }

        // Check if we need to 'enlarge' maze 'up' and enlarge if needed
        if (std::ranges::any_of(dirs, [&](auto &a) {
                return cur.levels_up[cur.levels_up.size() - 1].bugMap[midRow + a.first][midCol + a.second] == '#';
            })) {
            cur.levels_up.push_back(OneLevel{emptyBugMap});
            nxt.levels_up.push_back(OneLevel{emptyBugMap});
        }

        for (int lvlID = (-nxt.levels_down.size() + 1); lvlID < static_cast<int>(nxt.levels_up.size()); ++lvlID) {
            auto &curLvl = cur.get_oneLevel(lvlID)->get();
            auto &nxtLvl = nxt.get_oneLevel(lvlID)->get();

            for (int r = 0; r < emptyBugMap.size(); ++r) {
                for (int c = 0; c < emptyBugMap.front().size(); ++c) {
                    if (r == midRow && c == midCol) { continue; }

                    size_t count_bugAround = 0;
                    for (auto const &dir : dirs) {
                        int rowSrc = r + dir.first;
                        int colSrc = c + dir.second;

                        // Going down
                        if (rowSrc < 0 || rowSrc == emptyBugMap.size() || colSrc < 0 ||
                            colSrc == emptyBugMap.front().size()) {

                            if (auto minusLvl = cur.get_oneLevel(lvlID - 1); minusLvl.has_value()) {
                                count_bugAround += minusLvl->get().get_reqFromTopLvl(dir);
                            }
                            else {
                                // Do nothing
                                // That level is not initialized and that only happens if no bug can be
                                // there now or created this iteration
                            }
                        }

                        // Going up
                        else if (rowSrc == midRow && colSrc == midCol) {
                            if (auto plusLvl = cur.get_oneLevel(lvlID + 1); plusLvl.has_value()) {
                                count_bugAround += (plusLvl->get().get_reqFromBottomLvl(dir));
                            }
                            else {
                                // Do nothing
                                // That level is not initialized and that only happens if no bug can be
                                // there now or created this iteration
                            }
                        }

                        // Not going up or down
                        else { count_bugAround += (curLvl.bugMap[rowSrc][colSrc] == '#'); }
                    }

                    if (curLvl.bugMap[r][c] == '#' && not (count_bugAround == 1)) { nxtLvl.bugMap[r][c] = '.'; }
                    else if (curLvl.bugMap[r][c] == '.' && (count_bugAround == 1 or count_bugAround == 2)) {
                        nxtLvl.bugMap[r][c] = '#';
                    }
                    else { nxtLvl.bugMap[r][c] = curLvl.bugMap[r][c]; }
                }
            }
        }

        buf.swapBuffers();
    };

    // MAIN LOOP
    for (int iter_id = 0; iter_id < iterations; ++iter_id) { oneIteration(); }

    // CALCULATE RESULT
    auto bugMaps = buf.getCurrent().get_vecOfBugMaps();
    size_t bugsCount = std::ranges::fold_left(bugMaps, 0uz, [](auto &&accu, auto &&oneMap) {
        return accu + std::ranges::fold_left(oneMap, 0uz, [](auto &&accu2, auto &&oneLine) {
                   return accu2 + std::ranges::count_if(oneLine, [](auto &&chr) { return chr == '#'; });
               });
    });

    return bugsCount;
}
} // namespace AOC2019