#include <ankerl/unordered_dense.h>
#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional/functional.hpp>
#include <incom_commons.h>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>


namespace AOC2019 {
long long day14_1(std::string dataFile) {


    auto d_ctre = ctre::search<R"(\d+)">;
    auto w_ctre = ctre::search<R"(\w+)">;

    auto input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, w_ctre);

    ankerl::unordered_dense::map<std::string, long long, incom::commons::XXH3Hasher> storedInExcess;
    ankerl::unordered_dense::map<std::string, std::pair<long long, std::vector<std::pair<int, std::string>>>,
                                 incom::commons::XXH3Hasher>
        recipes;
    for (auto &inpLine : input) { storedInExcess.insert({inpLine.back(), 0ll}); };


    for (auto &inpLine : input) {
        std::vector<std::pair<int, std::string>> toInsert;
        for (auto iter = inpLine.begin(); iter < (inpLine.end() - 2); iter += 2) {
            toInsert.push_back({std::stoi(*(iter)), *(iter + 1)});
        }
        recipes.insert({inpLine.back(), {std::stoi(*(inpLine.end() - 2)), std::move(toInsert)}});
    }

    std::queue<std::pair<std::string, long long>> que;

    que.push(std::make_pair("FUEL", 1));
    long long oreRequired = 0;

    auto solve = [&]() -> void {
        auto const &recipeForQueFront = recipes.at(que.front().first);
        auto       &inStorage         = storedInExcess.at(que.front().first);

        int recipeMult = 0;

        if (que.front().second > inStorage) {
            recipeMult  = (que.front().second - inStorage) / recipeForQueFront.first;
            recipeMult += ((recipeMult * recipeForQueFront.first) != (que.front().second - inStorage));

            for (auto const &ingredient : recipeForQueFront.second) {
                if (ingredient.second == "ORE") { oreRequired += (ingredient.first * recipeMult); }
                else { que.push(std::make_pair(ingredient.second, ingredient.first * recipeMult)); }
            }
        }

        inStorage += (recipeMult * recipeForQueFront.first) - que.front().second;
        que.pop();
    };

    while (not que.empty()) { solve(); }

    return oreRequired;
}

long long day14_2(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    auto w_ctre = ctre::search<R"(\w+)">;

    auto input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, w_ctre);

    // DATA PREP
    ankerl::unordered_dense::set<std::string, incom::commons::XXH3Hasher> hlpr_namesToIDs;
    for (auto &line : input) {
        for (auto iter = line.begin(); iter < line.end(); iter += 2) { hlpr_namesToIDs.insert(*(iter + 1)); }
    }

    ankerl::unordered_dense::map<std::string, long long, incom::commons::XXH3Hasher> namesToIDs;
    for (long long id = 0; auto &item : hlpr_namesToIDs) { namesToIDs.insert(std::make_pair(item, id++)); }

    std::vector<long long> storedInExcess(namesToIDs.size(), 0);
    std::vector<std::pair<long long, std::vector<std::pair<long long, long long>>>> recipes(namesToIDs.size());


    for (auto &inpLine : input) {
        std::vector<std::pair<long long, long long>> toInsert;
        long long                                    resultID = namesToIDs.at(inpLine.back());

        for (auto iter = inpLine.begin(); iter < (inpLine.end() - 2); iter += 2) {
            toInsert.push_back({std::stoll(*(iter)), namesToIDs.at(*(iter + 1))});
        }
        recipes[resultID] = {std::stoll(*(inpLine.end() - 2)), std::move(toInsert)};
    }

    long long       oreRequired = 0;
    long long const oreID       = namesToIDs.at("ORE");
    long long const fuelID      = namesToIDs.at("FUEL");

    std::queue<std::pair<long long, long long>> que;

    // LAMBDA DEFINITIONS
    // Solve does 'one step' in finding the volume of 'ore' one needs to make everything in the 'que'
    // Successivelly calling solve until 'que' is empty will give the final answer
    auto solve = [&]() -> void {
        auto const &recipeForQueFront = recipes.at(que.front().first);
        auto       &inStorage         = storedInExcess.at(que.front().first);

        long long recipeMult = 0;

        if (que.front().second > inStorage) {
            recipeMult = (que.front().second - inStorage) / recipeForQueFront.first;
            if (recipeMult < 0) { std::cout << "WTF"; }
            recipeMult += ((recipeMult * recipeForQueFront.first) != (que.front().second - inStorage));
            if (recipeMult < 0) { std::cout << "WTF"; }

            for (auto const &ingredient : recipeForQueFront.second) {
                if (ingredient.second == oreID) { oreRequired += (ingredient.first * recipeMult); }
                else { que.push(std::make_pair(ingredient.second, ingredient.first * recipeMult)); }
            }
        }

        inStorage += (recipeMult * recipeForQueFront.first) - que.front().second;
        que.pop();
    };


    que.push(std::make_pair(fuelID, 1));
    while (not que.empty()) { solve(); }

    // Binary search setup
    long long const startOreAmount = 1'000'000'000'000;
    long long       bottom         = 1;
    long long       mid            = LLONG_MAX;
    long long       top            = startOreAmount / oreRequired;


    // First find an interval withing which the solution is for certain (ie. find the outtermost 'top')
    while (oreRequired < startOreAmount) {
        que.push(std::make_pair(fuelID, top));
        oreRequired    = 0;
        storedInExcess = std::vector<long long>(namesToIDs.size(), 0);
        while (not que.empty()) { solve(); }
        top *= 2;
    }

    // MAIN ALGO
    // Actual binary search
    mid = (bottom + top) / 2;
    while (mid != bottom) {
        que.push(std::make_pair(fuelID, mid));
        oreRequired    = 0;
        storedInExcess = std::vector<long long>(namesToIDs.size(), 0);
        while (not que.empty()) { solve(); }
        if (oreRequired > startOreAmount) { top = mid; }
        else { bottom = mid; }
        mid = (bottom + top) / 2;
    }

    return mid;
}
} // namespace AOC2019