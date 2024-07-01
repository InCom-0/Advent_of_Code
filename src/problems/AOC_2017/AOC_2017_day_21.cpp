#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <vector>

#include <AOC_commons.h>


namespace AOC2017 {


int day21_0(std::string dataFile, unsigned int numOfIterations) {
    // CUSTOM LOCAL TYPES
    using Pic = std::vector<std::vector<char>>;

    auto sideL_ctre = ctre::search<R"(.+(?= =>))">;
    auto sideR_ctre = ctre::search<R"((?<==> ).+)">;

    // DATA PREP
    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, sideL_ctre, sideR_ctre);

    ankerl::unordered_dense::map<std::string, std::vector<std::vector<char>>> mp;
    for (auto &VofV_line : VofV) {
        std::string subsTarStr;
        auto        substituteTarget = flux::ref(VofV_line.front())
                                    .chunk_by([](auto &&a, auto &&b) { return not (a == '/' or b == '/'); })
                                    .filter([](auto &&chnk) { return flux::read_at(chnk, flux::first(chnk)) != '/'; })
                                    .to<std::vector<std::vector<char>>>();

        auto substituteWith = flux::ref(VofV_line.back())
                                  .chunk_by([](auto &&a, auto &&b) { return not (a == '/' or b == '/'); })
                                  .filter([](auto &&chnk) { return flux::read_at(chnk, flux::first(chnk)) != '/'; })
                                  .to<std::vector<std::vector<char>>>();

        // Emplace the 'initial setup of substituteTarget'
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        // Flip the target 3 times. 1st horizontally, 2nd vertically, 3rd horizontally again and insert on each.
        for (auto &subsLine : substituteTarget) { std::reverse(subsLine.begin(), subsLine.end()); }
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        std::reverse(substituteTarget.begin(), substituteTarget.end());
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        for (auto &subsLine : substituteTarget) { std::reverse(subsLine.begin(), subsLine.end()); }
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        // Rotate right, insert and do the 3 steps above one more time.
        AOC_commons::matrixRotateRight(substituteTarget);
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        for (auto &subsLine : substituteTarget) { std::reverse(subsLine.begin(), subsLine.end()); }
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        std::reverse(substituteTarget.begin(), substituteTarget.end());
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();

        for (auto &subsLine : substituteTarget) { std::reverse(subsLine.begin(), subsLine.end()); }
        for (auto &subsLine : substituteTarget) { subsTarStr.append(subsLine.begin(), subsLine.end()); }
        mp.emplace(subsTarStr, substituteWith);
        subsTarStr.clear();
    }

    Pic picA;
    picA.push_back(std::vector<char>{'.', '#', '.'});
    picA.push_back(std::vector<char>{'.', '.', '#'});
    picA.push_back(std::vector<char>{'#', '#', '#'});
    Pic picB;

    Pic *picOrig  = &picA;
    Pic *pic2Fill = &picB;

    // MAIN LOGIC
    for (int iteration = 0; iteration < numOfIterations; ++iteration) {
        int picSideSize = (*picOrig).front().size();
        int patternSize = ((picSideSize % 2) == 0) ? 2 : 3;
        pic2Fill->resize(picSideSize + (picSideSize / patternSize));
        for (auto &pic2FillLine : (*pic2Fill)) { pic2FillLine.resize(picSideSize + (picSideSize / patternSize)); }

        for (int lineStart = 0; lineStart < (picSideSize - 1); lineStart += patternSize) {
            for (int colStart = 0; colStart < (picSideSize - 1); colStart += patternSize) {
                std::string lookupStr;
                for (int line = 0; line < patternSize; ++line) {
                    lookupStr.append((*picOrig)[lineStart + line].begin() + colStart,
                                     (*picOrig)[lineStart + line].begin() + colStart + patternSize);
                }
                Pic &substituteWith = mp.at(lookupStr);

                int adjLineStart = lineStart + (lineStart / patternSize);
                int adjColStart  = colStart + (colStart / patternSize);

                for (int line = 0; auto &substLine : substituteWith) {
                    for (int col = 0; auto &substChar : substLine) {
                        (*pic2Fill)[adjLineStart + line][adjColStart + (col)] = substChar;
                        col++;
                    }
                    line++;
                }
            }
        }
        std::swap(picOrig, pic2Fill);
    }

    return flux::ref(*picOrig).fold(
        [](auto &&accu, auto &&line) { return accu + flux::ref(line).count_if([](auto &&aa) { return aa == '#'; }); },
        0);
}

int day21_1(std::string dataFile) {
    return day21_0(dataFile, 5);
}
int day21_2(std::string dataFile) {
    return day21_0(dataFile, 18);
}

} // namespace AOC2017