#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"

#include "AOC_2016_day_10.h"


namespace AOC2016 {


int day10_1(std::string &&dataFile, std::pair<int, int> chipToFind) {

    auto goesToBot_ctre = ctre::search<R"(goes to bot)">;
    auto valueX_ctre = ctre::search<R"(\d+)">;

    struct bot {
        std::vector<int> packages = std::vector<int>();
        int lowerDestination = INT_MIN;
        int higherDestination = INT_MIN;
    };

    std::unordered_map<int, bot> botMap;
    std::queue<int> botsToProcess;

    auto checkBotAddIfToQueue = [&] (int& botID) {
        if(botMap[botID].packages.size() == 2 && botMap[botID].lowerDestination != INT_MIN && botMap[botID].higherDestination != INT_MIN) botsToProcess.push(botID);
    };

    auto giveChipToBot = [&] (int& botID, int chipToInsert) {
        botMap[botID];          //Insert if doesn't exist, otherwise do nothing
        botMap[botID].packages.push_back(chipToInsert);
        checkBotAddIfToQueue(botID);
    };

    auto setDestinationsForBot = [&] (int& botID, int lowerDest, int higherDest) {
        botMap[botID];          //Insert if doesn't exist, otherwise do nothing
        botMap[botID].lowerDestination = lowerDest;
        botMap[botID].higherDestination = higherDest;
        checkBotAddIfToQueue(botID);
    };

    auto execSingleBot = [&]  (int botToExec) {
        bot& selectedBot = botMap.at(botToExec);
        auto lowAndHigh = flux::ref(selectedBot.packages).find_minmax(std::less());
        giveChipToBot(selectedBot.lowerDestination, flux::read_at(selectedBot.packages, lowAndHigh.min));
        giveChipToBot(selectedBot.higherDestination, flux::read_at(selectedBot.packages, lowAndHigh.max));

    };


    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto en = oneStr.end();
        int botID;

        if (goesToBot_ctre(bg,en)) {
            botID = valueX_ctre(bg+16, en).to_number();
            giveChipToBot(botID,valueX_ctre(bg, en).to_number());
        }
        else {
            botID = valueX_ctre(bg, en).to_number();
            auto lowTargetRgxRes = valueX_ctre(bg+16, en);
            auto highTargetRgxRes = valueX_ctre(bg+40, en);
            int lowMult = *(lowTargetRgxRes.begin()-4) == 'p' ? 1'000'000 : 0;
            int highMult = *(highTargetRgxRes.begin()-4) == 'p' ? 1'000'000 : 0;

            setDestinationsForBot(botID, lowTargetRgxRes.to_number()+lowMult, highTargetRgxRes.to_number()+highMult);
        }
    }

    while (not botsToProcess.empty()) {
        execSingleBot(botsToProcess.front());
        botsToProcess.pop();
    }

    auto result = flux::from_range(botMap)
                    .filter([&] (auto&& aa) {
                        if (flux::ref(aa.second.packages).fold([] (auto&& init, auto &&bbb) {return init+bbb;},0) ==(chipToFind.first+chipToFind.second)) {
                            return aa.second.packages[0] == chipToFind.first || aa.second.packages[0] == chipToFind.second;
                        }
                        else return false;
                    })
                    .take(1)
                    .to<std::vector<std::pair<int,bot>>>();
    return result[0].first;
}

int day10_2(std::string &&dataFile, std::pair<int, int> chipToFind) {

    auto goesToBot_ctre = ctre::search<R"(goes to bot)">;
    auto valueX_ctre = ctre::search<R"(\d+)">;

    struct bot {
        std::vector<int> packages = std::vector<int>();
        int lowerDestination = INT_MIN;
        int higherDestination = INT_MIN;
    };

    std::unordered_map<int, bot> botMap;
    std::queue<int> botsToProcess;

    auto checkBotAddIfToQueue = [&] (int& botID) {
        if(botMap[botID].packages.size() == 2 && botMap[botID].lowerDestination != INT_MIN && botMap[botID].higherDestination != INT_MIN) botsToProcess.push(botID);
    };

    auto giveChipToBot = [&] (int& botID, int chipToInsert) {
        botMap[botID];          //Insert if doesn't exist, otherwise do nothing
        botMap[botID].packages.push_back(chipToInsert);
        checkBotAddIfToQueue(botID);
    };

    auto setDestinationsForBot = [&] (int& botID, int lowerDest, int higherDest) {
        botMap[botID];          //Insert if doesn't exist, otherwise do nothing
        botMap[botID].lowerDestination = lowerDest;
        botMap[botID].higherDestination = higherDest;
        checkBotAddIfToQueue(botID);
    };

    auto execSingleBot = [&]  (int botToExec) {
        bot& selectedBot = botMap.at(botToExec);
        auto lowAndHigh = flux::ref(selectedBot.packages).find_minmax(std::less());
        giveChipToBot(selectedBot.lowerDestination, flux::read_at(selectedBot.packages, lowAndHigh.min));
        giveChipToBot(selectedBot.higherDestination, flux::read_at(selectedBot.packages, lowAndHigh.max));

    };


    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    while (std::getline(iStream, oneStr)) {
        auto bg = oneStr.begin();
        auto en = oneStr.end();
        int botID;

        if (goesToBot_ctre(bg,en)) {
            botID = valueX_ctre(bg+16, en).to_number();
            giveChipToBot(botID,valueX_ctre(bg, en).to_number());
        }
        else {
            botID = valueX_ctre(bg, en).to_number();
            auto lowTargetRgxRes = valueX_ctre(bg+16, en);
            auto highTargetRgxRes = valueX_ctre(bg+40, en);
            int lowMult = *(lowTargetRgxRes.begin()-4) == 'p' ? 1'000'000 : 0;
            int highMult = *(highTargetRgxRes.begin()-4) == 'p' ? 1'000'000 : 0;

            setDestinationsForBot(botID, lowTargetRgxRes.to_number()+lowMult, highTargetRgxRes.to_number()+highMult);
        }
    }

    while (not botsToProcess.empty()) {
        execSingleBot(botsToProcess.front());
        botsToProcess.pop();
    }

    auto result = flux::from_range(botMap)
                    .filter([&] (auto&& aa) {
                        return aa.first >= 1'000'000 && aa.first < 1'000'003;
                    })
                    .fold([] (auto&& init, auto&& aaa) {return init * aaa.second.packages.front();}, 1);

    return result;
}

}