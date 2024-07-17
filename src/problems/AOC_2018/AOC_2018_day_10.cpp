#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>


namespace AOC2018 {


int day10_1(std::string dataFile) {
    // CUSTOM TYPES
    struct SimplePair {
        long first;
        long second;
    };

    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto VofV   = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // DATA PREP
    std::vector<SimplePair> positions;
    std::vector<SimplePair> velocities;
    for (auto &line : VofV) {
        positions.push_back(SimplePair{std::stol(line[0]), std::stol(line[1])});
        velocities.push_back(SimplePair{std::stol(line[2]), std::stol(line[3])});
    }

    long          maxVertical = LONG_MAX, minVertical = 0, maxHorizontal, minHorizontal, lastHeight;
    unsigned long counter = 0;

    // MAIN LOGIC
    do {
        lastHeight    = maxVertical - minVertical;
        maxVertical   = LONG_MIN;
        minVertical   = LONG_MAX;
        maxHorizontal = LONG_MIN;
        minHorizontal = LONG_MAX;

        for (unsigned int i = 0; i < positions.size(); ++i) {
            positions[i].first  += velocities[i].first;
            positions[i].second += velocities[i].second;
            maxVertical          = std::max(maxVertical, positions[i].second);
            minVertical          = std::min(minVertical, positions[i].second);
            maxHorizontal        = std::max(maxHorizontal, positions[i].first);
            minHorizontal        = std::min(minHorizontal, positions[i].first);
        }
        counter++;
    } while ((maxVertical - minVertical) < lastHeight);

    maxVertical   = LONG_MIN;
    minVertical   = LONG_MAX;
    maxHorizontal = LONG_MIN;
    minHorizontal = LONG_MAX;

    for (unsigned int i = 0; i < positions.size(); ++i) {
        positions[i].first  -= velocities[i].first;
        positions[i].second -= velocities[i].second;
        maxVertical          = std::max(maxVertical, positions[i].second);
        minVertical          = std::min(minVertical, positions[i].second);
        maxHorizontal        = std::max(maxHorizontal, positions[i].first);
        minHorizontal        = std::min(minHorizontal, positions[i].first);
    }
    
    // PRESENT OUTPUT
    std::vector<std::string> out(maxVertical - minVertical + 1, std::string(maxHorizontal - minHorizontal + 1, ' '));
    for (auto &onePos : positions) { out[onePos.second - minVertical][onePos.first - minHorizontal] = '#'; }
    for (auto &line : out) { std::cout << line << '\n'; }
    return counter;
}
} // namespace AOC2018