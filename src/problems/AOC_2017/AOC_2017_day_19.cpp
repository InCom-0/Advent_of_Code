#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <intrin.h>

#include <flux.hpp>


namespace AOC2017 {
    std::string day19_1(std::string dataFile) {
        // GETTING DATA
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string oneStr;

        std::vector<std::string> input;
        while (std::getline(iStream, oneStr)) input.push_back(oneStr);

        // DATA PREP
        size_t maxLineLength = flux::ref(input).max([] (auto &&a, auto &&b) {return a.size() < b.size();})->size();

        size_t rowID = 0;
        size_t colID = input.front().size()-1;

        for (auto &line: input) {
            auto tempSize = line.size();
            for (int i = 0; i < maxLineLength - tempSize; ++i) line.push_back(' ');
            line.push_back(' ');
        }
        input.push_back(std::string(201,' '));

        std::vector<std::pair<size_t,size_t>> dirs {{-1,0}, {0,1}, {1,0}, {0,-1}};
        std::string collectionOfFoundLetters;

        size_t dirSize = dirs.size();

        // LAMBDA DEFINITION ... note that rowID and colID and also captured
        auto solver = [&] (size_t &likelyDirID, size_t &impossibleDirID) -> bool {
            if (input[rowID][colID] >= 'A' && input[rowID][colID] <= 'Z') collectionOfFoundLetters.push_back(input[rowID][colID]);

            bool rtrn = false;
            if (input[rowID + dirs[likelyDirID].first][colID + dirs[likelyDirID].second] != ' ') {
                rtrn = true;
                rowID += dirs[likelyDirID].first;
                colID += dirs[likelyDirID].second;
            }
            else {
                for (int i = 0; i < dirSize; ++i) {
                    if (i == impossibleDirID || i == likelyDirID) continue;
                    else if (input[rowID + dirs[i].first][colID + dirs[i].second] != ' ') {
                        rtrn = true;
                        rowID += dirs[i].first;
                        colID += dirs[i].second;

                        likelyDirID = i;
                        if (i > 1) impossibleDirID = i-2;
                        else impossibleDirID = i + 2;
                    }
                }
            }
            return rtrn;
        };

        // MAIN LOGIC
        size_t likelyD_ID = 2;
        size_t impossibleD_ID = 0;
        while (solver(likelyD_ID,impossibleD_ID)) {};

        return collectionOfFoundLetters;
    }

    size_t day19_2(std::string dataFile) {
        // GETTING DATA
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        std::string oneStr;

        std::vector<std::string> input;
        while (std::getline(iStream, oneStr)) input.push_back(oneStr);

        // DATA PREP
        size_t maxLineLength = flux::ref(input).max([] (auto &&a, auto &&b) {return a.size() < b.size();})->size();

        size_t rowID = 0;
        size_t colID = input.front().size()-1;

        for (auto &line: input) {
            auto tempSize = line.size();
            for (int i = 0; i < maxLineLength - tempSize; ++i) line.push_back(' ');
            line.push_back(' ');
        }
        input.push_back(std::string(201,' '));

        std::vector<std::pair<size_t,size_t>> dirs {{-1,0}, {0,1}, {1,0}, {0,-1}};
        std::string collectionOfFoundLetters;
        size_t stepsCounter = 0;

        size_t dirSize = dirs.size();

        // LAMBDA DEFINITION ... note that rowID and colID and also captured
        auto solver = [&] (size_t &likelyDirID, size_t &impossibleDirID) -> bool {
            if (input[rowID][colID] >= 'A' && input[rowID][colID] <= 'Z') collectionOfFoundLetters.push_back(input[rowID][colID]);
            stepsCounter++;

            bool rtrn = false;
            if (input[rowID + dirs[likelyDirID].first][colID + dirs[likelyDirID].second] != ' ') {
                rtrn = true;
                rowID += dirs[likelyDirID].first;
                colID += dirs[likelyDirID].second;
            }
            else {
                for (int i = 0; i < dirSize; ++i) {
                    if (i == impossibleDirID || i == likelyDirID) continue;
                    else if (input[rowID + dirs[i].first][colID + dirs[i].second] != ' ') {
                        rtrn = true;
                        rowID += dirs[i].first;
                        colID += dirs[i].second;

                        likelyDirID = i;
                        if (i > 1) impossibleDirID = i-2;
                        else impossibleDirID = i+2;
                    }
                }
            }
            return rtrn;
        };

        // MAIN LOGIC
        size_t likelyD_ID = 2;
        size_t impossibleD_ID = 0;
        while (solver(likelyD_ID,impossibleD_ID)) {};

        return stepsCounter;
    }

}