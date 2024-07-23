#include "flux/op/ref.hpp"
#include <AOC_commons.h>
#include <algorithm>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>


namespace AOC2018 {

std::string day13_1(std::string dataFile) {
    // CUSTOM TYPES
    struct Cart {
        unsigned int yCoord;
        unsigned int xCoord;
        unsigned int curDirection       : 2;
        unsigned int onNextIntersection : 2 = 0;
        bool         deadCart               = false;

        int getDirChngOnIntersection() { return ((int)onNextIntersection - 1); }
    };
    auto anyCtre = ctre::search<R"(.+)">;
    auto VofV    = AOC_commons::parseInputUsingCTRE::processFile(dataFile, anyCtre).front();

    std::vector<std::pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // DATA PREP
    std::vector<Cart> cartsVec;
    for (unsigned int Ycoo = 0; auto &line : VofV) {
        for (unsigned int Xcoo = 0; auto &chr : line) {
            if (chr == 'v' or chr == '^') {
                if (chr == 'v') { cartsVec.push_back({Ycoo, Xcoo, 2}); }
                else { cartsVec.push_back({Ycoo, Xcoo, 0}); }
                chr = '|'; // change the map to the 'correct' track type underneath the cart
            }
            else if (chr == '<' || chr == '>') {
                if (chr == '<') { cartsVec.push_back({Ycoo, Xcoo, 3}); }
                else { cartsVec.push_back({Ycoo, Xcoo, 1}); }
                chr = '-'; // change the map to the 'correct' track type underneath the cart
            }
            Xcoo++;
        }
        Ycoo++;
    }

    // LAMBDA DEFINITIONS
    auto test_cartCrashed = [&](Cart &oneCart) -> bool {
        return flux::ref(cartsVec).count_if(
                   [&](auto &&a) { return a.yCoord == oneCart.yCoord and a.xCoord == oneCart.xCoord; }) > 1;
    };
    auto moveCartAndTest = [&](Cart &oneCart) -> bool {
        oneCart.yCoord += dirs[oneCart.curDirection].first;
        oneCart.xCoord += dirs[oneCart.curDirection].second;

        if (VofV[oneCart.yCoord][oneCart.xCoord] == '|' or VofV[oneCart.yCoord][oneCart.xCoord] == '-') {}
        else if (VofV[oneCart.yCoord][oneCart.xCoord] == '+') {
            oneCart.curDirection       += oneCart.getDirChngOnIntersection();
            oneCart.onNextIntersection += (((unsigned int)oneCart.onNextIntersection)) / 2 + 1;
        }
        else if (VofV[oneCart.yCoord][oneCart.xCoord] == '/') {
            if (oneCart.curDirection % 2 == 0) { oneCart.curDirection += 1; }
            else { oneCart.curDirection += -1; }
        }
        else if (VofV[oneCart.yCoord][oneCart.xCoord] == '\\') {
            if (oneCart.curDirection % 2 == 0) { oneCart.curDirection += -1; }
            else { oneCart.curDirection += 1; }
        }

        return test_cartCrashed(oneCart);
    };

    // MAIN LOGIC
    while (true) {
        std::ranges::sort(cartsVec, [](auto &&a, auto &&b) {
            if (a.yCoord == b.yCoord) { return a.xCoord < b.xCoord; }
            else { return a.yCoord < b.yCoord; }
        });

        for (auto &oneCart : cartsVec) {
            if (moveCartAndTest(oneCart)) {
                return std::to_string(oneCart.xCoord) + "," + std::to_string(oneCart.yCoord);
            };
        }
    }
    std::unreachable();
}

std::string day13_2(std::string dataFile) {
    // CUSTOM TYPES
    struct Cart {
        unsigned int yCoord;
        unsigned int xCoord;
        unsigned int curDirection       : 2;
        unsigned int onNextIntersection : 2 = 0;
        bool         deadCart               = false;

        int getDirChngOnIntersection() { return ((int)onNextIntersection - 1); }
    };

    auto anyCtre = ctre::search<R"(.+)">;
    auto VofV    = AOC_commons::parseInputUsingCTRE::processFile(dataFile, anyCtre).front();

    std::vector<std::pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // DATA PREP
    std::vector<Cart> cartsVec;
    for (unsigned int Ycoo = 0; auto &line : VofV) {
        for (unsigned int Xcoo = 0; auto &chr : line) {
            if (chr == 'v' or chr == '^') {
                if (chr == 'v') { cartsVec.push_back({Ycoo, Xcoo, 2}); }
                else { cartsVec.push_back({Ycoo, Xcoo, 0}); }
                chr = '|'; // change the map to the 'correct' track type underneath the cart
            }
            else if (chr == '<' || chr == '>') {
                if (chr == '<') { cartsVec.push_back({Ycoo, Xcoo, 3}); }
                else { cartsVec.push_back({Ycoo, Xcoo, 1}); }
                chr = '-'; // change the map to the 'correct' track type underneath the cart
            }
            Xcoo++;
        }
        Ycoo++;
    }

    // LAMBDA DEFINITIONS
    int  removed              = 0;
    auto testCrashedAndRemove = [&](Cart &oneCart) -> void {
        for (auto &oneCart_2 : cartsVec) {
            // NOTE: Yes this is correct, comparing the addresses ie. &oneCart_2 != &oneCart
            // Relatively simple 'hack' to find 'some other one' matching the criteria inside the vector
            // Naturaly wouldn't work in a member function as this physically requires that the lambda gets passed a reference to some element in the cartsVec
            // As opposed to a reference to some randomly created 'Cart' object 
            if (oneCart_2.yCoord == oneCart.yCoord && oneCart_2.xCoord == oneCart.xCoord &&
                oneCart_2.deadCart == false && &oneCart_2 != &oneCart) {
                oneCart_2.deadCart  = true;
                oneCart.deadCart    = true;
                removed            += 2;
            }
        }
    };
    auto moveCartAndTestAndRemove = [&](Cart &oneCart) -> void {
        oneCart.yCoord += dirs[oneCart.curDirection].first;
        oneCart.xCoord += dirs[oneCart.curDirection].second;

        if (VofV[oneCart.yCoord][oneCart.xCoord] == '|' or VofV[oneCart.yCoord][oneCart.xCoord] == '-') {}
        else if (VofV[oneCart.yCoord][oneCart.xCoord] == '+') {
            oneCart.curDirection       += oneCart.getDirChngOnIntersection();
            oneCart.onNextIntersection += (((unsigned int)oneCart.onNextIntersection)) / 2 + 1;
        }
        else if (VofV[oneCart.yCoord][oneCart.xCoord] == '/') {
            if (oneCart.curDirection % 2 == 0) { oneCart.curDirection += 1; }
            else { oneCart.curDirection += -1; }
        }
        else if (VofV[oneCart.yCoord][oneCart.xCoord] == '\\') {
            if (oneCart.curDirection % 2 == 0) { oneCart.curDirection += -1; }
            else { oneCart.curDirection += 1; }
        }
        testCrashedAndRemove(oneCart);
    };

    // MAIN LOGIC
    while (true) {
        std::ranges::sort(cartsVec, [](auto &&a, auto &&b) {
            if (a.yCoord == b.yCoord) { return a.xCoord < b.xCoord; }
            else { return a.yCoord < b.yCoord; }
        });

        for (auto &oneCart : cartsVec) {
            if (oneCart.deadCart) { continue; }
            moveCartAndTestAndRemove(oneCart);
        }
        if (removed == cartsVec.size() - 1) {
            auto toReturn = std::ranges::find_if(cartsVec, [](auto &&a) { return a.deadCart == false; });
            return std::to_string((*toReturn).xCoord) + ',' + std::to_string((*toReturn).yCoord);
        };
        // std::cout << cartsVec.size() << '\n';
    }
    std::unreachable();
}
} // namespace AOC2018