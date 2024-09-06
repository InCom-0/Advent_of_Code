#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2018 {

size_t day18_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;

    auto VofV = AOC_commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
    for (auto &line : VofV) {
        line.insert(0, 1, 'B');
        line.push_back('B');
    }
    VofV.insert(VofV.begin(), std::string(VofV.front().size(), 'B'));
    VofV.push_back(std::string(VofV.front().size(), 'B'));

    AOC_commons::doubleBuffer buf(VofV);

    std::vector<std::pair<int, int>> dirs{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};


    auto openChange = [&](auto const &dataCur, int const &rowPos, int const &colPos, char &toChng) -> void {
        if (dataCur[rowPos][colPos] == '.') {
            size_t treeCounter = 0;
            for (auto &pr : dirs) { treeCounter += (dataCur[rowPos + pr.first][colPos + pr.second] == '|'); }
            if (treeCounter > 2) { toChng = '|'; }
            else { toChng = '.'; }
        }
    };
    auto treeChange = [&](auto const &dataCur, int const &rowPos, int const &colPos, char &toChng) -> void {
        if (dataCur[rowPos][colPos] == '|') {
            size_t lumberCounter = 0;
            for (auto &pr : dirs) { lumberCounter += (dataCur[rowPos + pr.first][colPos + pr.second] == '#'); }
            if (lumberCounter > 2) { toChng = '#'; }
            else { toChng = '|'; }
        }
    };
    auto lumberChange = [&](auto const &dataCur, int const &rowPos, int const &colPos, char &toChng) -> void {
        if (dataCur[rowPos][colPos] == '#') {
            size_t lumberCounter = 0;
            size_t treeCounter   = 0;
            for (auto &pr : dirs) {
                lumberCounter += (dataCur[rowPos + pr.first][colPos + pr.second] == '#');
                treeCounter   += (dataCur[rowPos + pr.first][colPos + pr.second] == '|');
            }
            if (lumberCounter > 0 and treeCounter > 0) { toChng = '#'; }
            else { toChng = '.'; }
        }
    };

    for (int gen = 0; gen < 10; ++gen) {
        auto const &cur = buf.getCurrent();
        auto       &nxt = buf.getNext();

        for (int i = 1; i < (VofV.size() - 1); ++i) {
            for (int j = 1; j < (VofV.front().size() - 1); ++j) { openChange(cur, i, j, nxt[i][j]); }
        }
        for (int i = 1; i < (VofV.size() - 1); ++i) {
            for (int j = 1; j < (VofV.front().size() - 1); ++j) { treeChange(cur, i, j, nxt[i][j]); }
        }
        for (int i = 1; i < (VofV.size() - 1); ++i) {
            for (int j = 1; j < (VofV.front().size() - 1); ++j) { lumberChange(cur, i, j, nxt[i][j]); }
        }

        buf.swapBuffers();

        // for (auto &line : buf.getCurrent()) { std::cout << line << '\n'; }
        // std::cout << '\n' << '\n' << '\n';
    }


    return flux::ref(buf.getCurrent())
               .fold(
                   [](size_t &&init, auto &&line) {
                       return init + flux::ref(line).count_if([](auto &&chr) { return (chr == '#'); });
                   },
                   0) *
           flux::ref(buf.getCurrent())
               .fold(
                   [](size_t &&init, auto &&line) {
                       return init + flux::ref(line).count_if([](auto &&chr) { return (chr == '|'); });
                   },
                   0);
}

size_t day18_2(std::string dataFile) {
    // DATA PREP
    auto any_ctre = ctre::search<R"(.+)">;

    auto VofV = AOC_commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
    for (auto &line : VofV) {
        line.insert(0, 1, 'B');
        line.push_back('B');
    }
    VofV.insert(VofV.begin(), std::string(VofV.front().size(), 'B'));
    VofV.push_back(std::string(VofV.front().size(), 'B'));

    AOC_commons::doubleBuffer buf(VofV);

    std::vector<std::pair<int, int>> const dirs{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    std::unordered_map<size_t, size_t>     theMap;
    AOC_commons::XXH3Hasher                hasher;
    size_t                                 tempHash = hasher(buf.getCurrent());
    size_t                                 counter  = 0;

    // LAMBDA DEFINITIONS

    auto allChange = [&](auto const &dataCur, int const &rowPos, int const &colPos, char &toChng) -> void {
        size_t lumberCounter = 0;
        size_t treeCounter   = 0;
        for (auto &pr : dirs) {
            lumberCounter += (dataCur[rowPos + pr.first][colPos + pr.second] == '#');
            treeCounter   += (dataCur[rowPos + pr.first][colPos + pr.second] == '|');
        }
        if (dataCur[rowPos][colPos] == '#') {
            if (lumberCounter > 0 and treeCounter > 0) { toChng = '#'; }
            else { toChng = '.'; }
            return;
        }
        else if (dataCur[rowPos][colPos] == '|') {
            if (lumberCounter > 2) { toChng = '#'; }
            else { toChng = '|'; }
        }
        else if (dataCur[rowPos][colPos] == '.') {
            if (treeCounter > 2) { toChng = '|'; }
            else { toChng = '.'; }
        }
        return;
    };

    auto const oneGeneration = [&]() {
        theMap.insert({tempHash, counter++});
        auto const &cur = buf.getCurrent();
        auto       &nxt = buf.getNext();

        for (int i = 1; i < (VofV.size() - 1); ++i) {
            for (int j = 1; j < (VofV.front().size() - 1); ++j) { allChange(cur, i, j, nxt[i][j]); }
        }
        buf.swapBuffers();
        tempHash = hasher(buf.getCurrent());

        // for (auto &line : buf.getCurrent()) { std::cout << line << '\n'; }
        // std::cout << '\n' << '\n' << '\n';
    };

    // MAIN LOGIC
    // Find a cycle by hash
    do { oneGeneration(); } while (not theMap.contains(tempHash));

    // Compute the 'position in cycle' corresponding to the 1'000'000'000 iteration
    size_t remnant = (1'000'000'000 - counter) % (counter - theMap[tempHash]);

    // Compute the map state of the 1'000'000'000 iteration
    for (int i = 0; i < (remnant); ++i) { oneGeneration(); }

    // COMPUTE THE RETURN VALUE
    return flux::ref(buf.getCurrent())
               .fold(
                   [](size_t &&init, auto &&line) {
                       return init + flux::ref(line).count_if([](auto &&chr) { return (chr == '#'); });
                   },
                   0) *
           flux::ref(buf.getCurrent())
               .fold(
                   [](size_t &&init, auto &&line) {
                       return init + flux::ref(line).count_if([](auto &&chr) { return (chr == '|'); });
                   },
                   0);
}
} // namespace AOC2018