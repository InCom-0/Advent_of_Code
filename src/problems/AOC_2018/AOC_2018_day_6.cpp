#include <incom_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <unordered_set>
#include <utility>


namespace AOC2018 {
int day6_1(std::string dataFile) {
    // CUSTOM DATA TYPES
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofV   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // DATA AND LAMBDA PREP
    std::vector<std::pair<int, int>> input;
    for (auto &line : VofV) { input.push_back(std::make_pair(std::stoi(line.front()), std::stoi(line.back()))); }
    int leftBor   = std::ranges::min_element(input, [](auto &&a, auto &&b) { return a.first < b.first; })->first;
    int rightBor  = std::ranges::max_element(input, [](auto &&a, auto &&b) { return a.first < b.first; })->first;
    int topBor    = std::ranges::min_element(input, [](auto &&a, auto &&b) { return a.second < b.second; })->second;
    int bottomBor = std::ranges::max_element(input, [](auto &&a, auto &&b) { return a.second < b.second; })->second;


    std::unordered_set<int> infiniteIDs;
    std::vector<int>        counter(input.size(), 0);

    auto closestID = [&](int &row, int &col) -> int {
        std::pair<int, int> IDdist{INT_MIN, INT_MAX};
        for (int r = 0; auto &point : input) {
            int dist = std::abs(point.second - row) + std::abs(point.first - col);

            if (dist < IDdist.second) {
                IDdist.first  = r;
                IDdist.second = dist;
            }
            else if (dist == IDdist.second) { IDdist.first = INT_MIN; }
            r++;
        }
        if (row == topBor || row == bottomBor || col == leftBor || col == rightBor) {
            infiniteIDs.insert(IDdist.first);
        }
        if (IDdist.first != INT_MIN) { counter[IDdist.first]++; }
        return IDdist.first;
    };

    // MAIN LOGIC
    for (int i = topBor; i <= bottomBor; ++i) {
        for (int j = leftBor; j <= rightBor; ++j) { closestID(i, j); }
    }

    auto res = flux::ref(counter)
                   .filter([&, i = 0](auto &&a) mutable { return not infiniteIDs.contains(i++); })
                   .max(std::less())
                   .value_or(INT_MIN);
    return res;
}

int day6_2(std::string dataFile, int distanceLessThan) {
    // CUSTOM DATA TYPES
    auto d_ctre = ctre::search<R"(\d+)">;
    auto VofV   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // DATA AND LAMBDA PREP
    std::vector<std::pair<int, int>> input;
    for (auto &line : VofV) { input.push_back(std::make_pair(std::stoi(line.front()), std::stoi(line.back()))); }
    int leftBor   = std::ranges::min_element(input, [](auto &&a, auto &&b) { return a.first < b.first; })->first;
    int rightBor  = std::ranges::max_element(input, [](auto &&a, auto &&b) { return a.first < b.first; })->first;
    int topBor    = std::ranges::min_element(input, [](auto &&a, auto &&b) { return a.second < b.second; })->second;
    int bottomBor = std::ranges::max_element(input, [](auto &&a, auto &&b) { return a.second < b.second; })->second;


    std::unordered_set<int> infiniteIDs;
    std::vector<int>        counter(input.size(), 0);
    int                     distanceAboveCounter = 0;

    auto closestID = [&](int &row, int &col) -> void {
        int accuDist = 0;
        for (auto &point : input) { accuDist += std::abs(point.second - row) + std::abs(point.first - col); }
        if (accuDist < distanceLessThan) { distanceAboveCounter++; }
    };

    // MAIN LOGIC
    for (int i = topBor; i <= bottomBor; ++i) {
        for (int j = leftBor; j <= rightBor; ++j) { closestID(i, j); }
    }

    return distanceAboveCounter;
}
} // namespace AOC2018