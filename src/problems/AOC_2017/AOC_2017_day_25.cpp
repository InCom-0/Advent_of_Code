#include <algorithm>
#include <fstream>

#include <incom_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2017 {
struct state {
    char   stateName;
    char   stateTar_ifFalse;
    char   stateTar_ifTrue;
    size_t stateTarIDX_ifFalse;
    size_t stateTarIDX_ifTrue;
    int move_ifFalse;
    int move_ifTrue;

    bool w_ifFalse;
    bool w_ifTrue;
};

int day25_1(std::string dataFile) {
    // CUSTOM LOCAL TYPES
    auto d_ctre      = ctre::search<R"(\d+)">;
    auto colonW_ctre = ctre::search<R"(\w+(?=:))">;
    auto dptW_ctre   = ctre::search<R"([[:alpha:]]+(?=\.))">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    std::getline(iStream, oneStr);
    auto        tmpStr    = dptW_ctre(oneStr.begin(), oneStr.end());
    std::string startWith = tmpStr.to_string();

    std::getline(iStream, oneStr);
    tmpStr               = d_ctre(oneStr.begin(), oneStr.end());
    long long afterSteps = tmpStr.to_number();

    std::vector<state> vectorOfStates;
    while (std::getline(iStream, oneStr)) {
        state stateToInsert;

        std::getline(iStream, oneStr);
        tmpStr                  = colonW_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.stateName = tmpStr.to_string().front();

        std::getline(iStream, oneStr);
        std::getline(iStream, oneStr);
        tmpStr                  = d_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.w_ifFalse = tmpStr.to_number();

        std::getline(iStream, oneStr);
        tmpStr                     = dptW_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.move_ifFalse = tmpStr.to_string() == "left" ? -1 : 1;

        std::getline(iStream, oneStr);
        tmpStr                         = dptW_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.stateTar_ifFalse = tmpStr.to_string().front();

        std::getline(iStream, oneStr);
        std::getline(iStream, oneStr);
        tmpStr                 = d_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.w_ifTrue = tmpStr.to_number();

        std::getline(iStream, oneStr);
        tmpStr                    = dptW_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.move_ifTrue = tmpStr.to_string() == "left" ? -1 : 1;

        std::getline(iStream, oneStr);
        tmpStr                        = dptW_ctre(oneStr.begin(), oneStr.end());
        stateToInsert.stateTar_ifTrue = tmpStr.to_string().front();

        vectorOfStates.push_back(stateToInsert);
    }

    for (auto &stateItem : vectorOfStates) {
        stateItem.stateTarIDX_ifFalse =
            std::ranges::find_if(vectorOfStates, [&](auto &a) { return a.stateName == stateItem.stateTar_ifFalse; }) -
            vectorOfStates.begin();

        stateItem.stateTarIDX_ifTrue =
            std::ranges::find_if(vectorOfStates, [&](auto &a) { return a.stateName == stateItem.stateTar_ifTrue; }) -
            vectorOfStates.begin();
    }

    bit::bit_vector<unsigned char> tape(1024, bit::bit0);
    size_t                         idxInTape = 0;
    size_t                         thisState =
        std::ranges::find_if(vectorOfStates, [&](auto &&a) { return startWith.front() == a.stateName; }) -
        vectorOfStates.begin();
    size_t bit1Counter = 0;

    for (size_t i = 0; i < afterSteps; i++) {
        if (idxInTape == 0 || idxInTape == (tape.size() - 1)) {
            tape.resize(tape.size() * 2, bit::bit0);
            bit::rotate(tape.begin(), tape.end() - (tape.size() / 4), tape.end());
            idxInTape += (tape.size() / 4);
        }
        if (tape[idxInTape] == bit::bit0) {
            if (vectorOfStates[thisState].w_ifFalse == true) { bit1Counter++; }
            tape[idxInTape]  = bit::bit_value(vectorOfStates[thisState].w_ifFalse);
            idxInTape       += vectorOfStates[thisState].move_ifFalse;
            thisState        = vectorOfStates[thisState].stateTarIDX_ifFalse;
        }
        else {
            if (vectorOfStates[thisState].w_ifTrue == false) { bit1Counter--; }
            tape[idxInTape]  = bit::bit_value(vectorOfStates[thisState].w_ifTrue);
            idxInTape       += vectorOfStates[thisState].move_ifTrue;
            thisState        = vectorOfStates[thisState].stateTarIDX_ifTrue;
        }
    }


    return bit1Counter;
}

int day25_2(std::string dataFile) {


    return -999;
}
} // namespace AOC2017