#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>


namespace AOC2023 {
struct Game {
    size_t ID = 0uz;
    struct Cubes {
        size_t red   = 0uz;
        size_t green = 0uz;
        size_t blue  = 0uz;
    };

    std::vector<Cubes> draws;
};

auto
day2_0(std::string const &dataFile) {
    auto digits_ctre = ctre::search<R"(\d+)">;
    auto color_ctre  = ctre::search<R"(\d+|;|red|green|blue)">;

    struct Game {
        size_t ID = 0uz;
        struct Cubes {
            size_t red   = 0uz;
            size_t green = 0uz;
            size_t blue  = 0uz;
        };

        std::vector<Cubes> draws;
    };


    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }

    std::string oneStr;

    std::vector<Game> games;

    while (std::getline(iStream, oneStr)) {
        std::string::iterator bg  = oneStr.begin();
        std::string::iterator end = oneStr.end();

        auto result = color_ctre(oneStr.begin(), oneStr.end());

        games.emplace_back();
        games.back().ID = std::stoull(result.to_string());
        games.back().draws.emplace_back();

        std::vector<std::string> test;
        bg = result.get_end_position();

        size_t lastNum = 0uz;


        while (result = color_ctre(bg, end), result) {
            std::string nonNumber = result.to_string();
            if (nonNumber == ";") { games.back().draws.emplace_back(); }
            else if (nonNumber == "red") { games.back().draws.back().red = lastNum; }
            else if (nonNumber == "green") { games.back().draws.back().green = lastNum; }
            else if (nonNumber == "blue") { games.back().draws.back().blue = lastNum; }
            else {
                if (auto optNum = result.to_optional_number()) { lastNum = optNum.value(); }
            }
            bg = result.get_end_position();
        }
    }
    return games;
}

size_t
day2_1(std::string dataFile) {
    auto games = day2_0(dataFile);

    Game::Cubes bag{.red = 12, .green = 13, .blue = 14};
    size_t      accuRes = 0uz;

    for (auto const &oneGame : games) {
        if (std::ranges::all_of(oneGame.draws, [&](auto const &oneDraw) {
                return (oneDraw.red <= bag.red && oneDraw.green <= bag.green && oneDraw.blue <= bag.blue);
            })) {
            accuRes += oneGame.ID;
        }
    }
    return accuRes;
}


size_t
day2_2(std::string dataFile) {
    auto games = day2_0(dataFile);

    Game::Cubes bag{.red = 12, .green = 13, .blue = 14};
    size_t      accuRes = 0uz;

    for (auto const &oneGame : games) {

        Game::Cubes thisGroupMin{.red = 0uz, .green = 0uz, .blue = 0uz};
        for (auto const &oneDraw : oneGame.draws) {
            thisGroupMin.red   = std::max(thisGroupMin.red, oneDraw.red);
            thisGroupMin.green = std::max(thisGroupMin.green, oneDraw.green);
            thisGroupMin.blue  = std::max(thisGroupMin.blue, oneDraw.blue);
        }
        accuRes += (thisGroupMin.red * thisGroupMin.green * thisGroupMin.blue);
    }
    return accuRes;
}


} // namespace AOC2023