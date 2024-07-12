#include <ctre/wrapper.hpp>
#include <fstream>

#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2018 {
int day8_1(std::string dataFile) {
    // CUSTOM TYPES
    auto d_ctre = ctre::search<R"(\d+)">;
    auto vec    = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front();

    std::vector<int> input;
    for (auto &str : vec) { input.push_back(std::stoi(str)); }

    size_t id            = 0;
    int    sumOfMetaData = 0;

    auto createNode = [&](this const auto &self) -> void {
        int numOfNodes    = input[id++];
        int numOfMetadata = input[id++];

        for (int i = 0; i < numOfNodes; ++i) { self(); }
        for (int i = 0; i < numOfMetadata; ++i) { sumOfMetaData += input[id++]; }
    };
    createNode();
    return sumOfMetaData;
}

int day8_2(std::string dataFile) {
    // CUSTOM TYPES
    auto d_ctre = ctre::search<R"(\d+)">;
    auto vec    = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front();

    std::vector<int> input;
    for (auto &str : vec) { input.push_back(std::stoi(str)); }

    size_t id         = 0;
    auto   createNode = [&](this const auto &self) -> int {
        int              numOfNodes              = input[id++];
        int              numOfMetadata           = input[id++];
        int              myValue                 = 0;
        std::vector<int> computedChildNodeValues = {};

        for (int i = 0; i < numOfNodes; ++i) { computedChildNodeValues.push_back(self()); }

        if (numOfNodes > 0) {
            for (int i = 0; i < numOfMetadata; ++i) {
                if (input[id] <= numOfNodes) { myValue += computedChildNodeValues[input[id] - 1]; }
                id++;
            }
        }
        else {
            for (int i = 0; i < numOfMetadata; ++i) { myValue += input[id++]; }
        }
        return myValue;
    };

    return createNode();
}
} // namespace AOC2018