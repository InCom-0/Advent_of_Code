#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <print>
#include <stack>
#include <string>


namespace AOC2018 {

struct OnePos {
    unsigned long posY;
    unsigned long posX;
    int           junction = 0; // 0 at start, +1 for left is blocked, +1 for right is blocked.
};

std::pair<size_t, size_t> day17_0(std::string dataFile) {

    // DEFINITIONS
    auto xCtre = ctre::search<R"((?<=x=)[^,]+)">;
    auto yCtre = ctre::search<R"((?<=y=)[^,]+)">;
    auto dCtre = ctre::search<R"(\d+)">;

    // DATA PREP
    auto xCoords_string = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, xCtre);
    auto yCoords_string = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, yCtre);

    std::vector<OnePos> xCoords;
    std::vector<OnePos> yCoords;
    unsigned long       xMax = 0;
    unsigned long       yMax = 0;
    unsigned long       yMin = ULONG_MAX;

    for (auto &line : xCoords_string) {
        if (line.front().contains("..")) {
            std::vector<std::string> split =
                AOC_commons::parseInputUsingCTRE::processOneLineRPToneVect(line.front(), dCtre);
            xCoords.push_back(OnePos{std::stoul(split.front()), std::stoul(split.back())});
        }
        else { xCoords.push_back(OnePos{std::stoul(line.front()), std::stoul(line.front())}); }
        xMax = std::max(xMax, xCoords.back().posY);
    }
    for (auto &line : yCoords_string) {
        if (line.front().contains("..")) {
            std::vector<std::string> split =
                AOC_commons::parseInputUsingCTRE::processOneLineRPToneVect(line.front(), dCtre);
            yCoords.push_back(OnePos{std::stoul(split.front()), std::stoul(split.back())});
        }
        else { yCoords.push_back(OnePos{std::stoul(line.front()), std::stoul(line.front())}); }
        yMax = std::max(yMax, yCoords.back().posY);
        yMin = std::min(yMin, yCoords.back().posY);
    }

    xMax++;
    // Create a map out of the data
    std::vector<std::vector<char>> map(yMax + 1, std::vector<char>(xMax + 1, '.'));
    for (int coordID = 0; coordID < xCoords.size(); ++coordID) {
        for (int xID = xCoords[coordID].posY; xID <= xCoords[coordID].posX; ++xID) {
            for (int yID = yCoords[coordID].posY; yID <= yCoords[coordID].posX; ++yID) { map[yID][xID] = '#'; }
        }
    }
    std::stack<OnePos> stackOfJunctions;

    // LAMBDA DEFINITIONS
    int xPosAdder         = 0;
    int stillWaterCounter = 0;

    auto fillStillWater = [&]() -> void {
        int xAdder = 0;
        while (map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + (xAdder)] != '#') {
            map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + (xAdder--)] = 'W';
            stillWaterCounter++;
        }
        xAdder = 0;
        while (map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + (++xAdder)] != '#') {
            map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + (xAdder)] = 'W';
            stillWaterCounter++;
        }
    };

    auto explore = [&](this auto &self, auto &executor, int leftOrRight) -> bool {
        int addDepth = 0;
        if (map[stackOfJunctions.top().posY + 1][stackOfJunctions.top().posX + xPosAdder] == 'F') {
            map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + xPosAdder] = 'F';
            return false;
        }

        // Logic for exploring 'down'
        if (map[stackOfJunctions.top().posY + 1][stackOfJunctions.top().posX + xPosAdder] == '.') {
            if (xPosAdder != 0) {
                // Special case where for the very begining
                stackOfJunctions.push(OnePos{stackOfJunctions.top().posY, stackOfJunctions.top().posX + xPosAdder});
            }

            while (map[stackOfJunctions.top().posY + 1][stackOfJunctions.top().posX] == '.') {
                map[stackOfJunctions.top().posY + 1][stackOfJunctions.top().posX] = 'F';
                stackOfJunctions.push(OnePos{stackOfJunctions.top().posY + 1, stackOfJunctions.top().posX});

                xPosAdder = 0;
                addDepth++;
                if ((stackOfJunctions.top().posY + 1) >= (yMax + 1)) { break; }
            }

            bool tempResult = true;
            if ((stackOfJunctions.top().posY + 1) >= (yMax + 1)) {
                stackOfJunctions.pop();
                addDepth--;
            }

            while (addDepth-- > 0) { tempResult = executor() and tempResult; }

            if (tempResult) {
                xPosAdder = 0;
                if (self(executor, leftOrRight) == true) {
                    stackOfJunctions.pop();
                    return true;
                }
                else {
                    stackOfJunctions.pop();
                    return false;
                }
            }
            else {
                stackOfJunctions.pop();
                return false;
            }
        }

        // Logic for exploring the sides
        xPosAdder += leftOrRight;
        if (stackOfJunctions.top().posX + xPosAdder == 0 || stackOfJunctions.top().posX + xPosAdder > xMax) {
            return false;
        }
        else if (map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + xPosAdder] == '.') {
            map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + xPosAdder] = 'F';
            return self(executor, leftOrRight);
        }
        else if (map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + xPosAdder] == '#') { return true; }
        else if (map[stackOfJunctions.top().posY][stackOfJunctions.top().posX + xPosAdder] == 'F') { return false; }
        else { assert((void("ERROR"), false)); }
        std::unreachable();
    };

    int  lOrR     = 0;
    auto executor = [&](this auto &self) -> bool {
        xPosAdder                        = 0;
        lOrR                             = -1;
        stackOfJunctions.top().junction += explore(self, lOrR);

        xPosAdder                        = 0;
        lOrR                             = 1;
        stackOfJunctions.top().junction += explore(self, lOrR);

        if (stackOfJunctions.top().junction == 2) {
            fillStillWater();
            stackOfJunctions.pop();
            return true;
        }
        else {
            stackOfJunctions.pop();
            return false;
        }
    };

    // MAIN LOGIC
    OnePos springPos{0, 500};
    stackOfJunctions.push(springPos);
    stackOfJunctions.push(springPos);
    executor();

    // CALCULATE RESULT
    auto stillWaterCount = flux::ref(map).drop(yMin).fold(
        [](size_t &&acc, auto &&line) {
            return acc + flux::ref(line).count_if([](auto &chr) { return (chr == 'W'); });
        },
        0);

    auto flowWaterCount = flux::ref(map).drop(yMin).fold(
        [](size_t &&acc, auto &&line) {
            return acc + flux::ref(line).count_if([](auto &chr) { return (chr == 'F'); });
        },
        0);
    /* const auto    tmpPth = (std::filesystem::path("../data") / "test.txt");
    std::ofstream oStream; */

    /*     oStream.open(tmpPth.string());
        if (not oStream.is_open()) { std::cout << "ERROR OPENING FILE"; }
        else {
            for (auto &line : map) {
                std::string ara(line.begin(), line.end());
                oStream << ara << '\n';
            }
        }
        oStream.close(); */

    return std::make_pair(stillWaterCount, flowWaterCount);
}

size_t day17_1(std::string dataFile) {
    auto ansPair = day17_0(dataFile);
    return ansPair.first + ansPair.second;
}
size_t day17_2(std::string dataFile) {
    return day17_0(dataFile).first;
}
} // namespace AOC2018