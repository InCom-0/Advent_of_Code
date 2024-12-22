#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;
long long day18_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // Get a more reasonable input structure for the 'falling bytes'
    std::vector<std::pair<int, int>> fallingBytes;
    for (auto const &line : input) {
        fallingBytes.push_back({std::stoi(line.back()) + 1, std::stoi(line.front()) + 1});
    }

    // Create suitable map structure including 'padding'
    std::vector<std::vector<char>> map;
    std::vector<char>              middleInsert(71, '.');
    middleInsert.push_back('#');
    middleInsert.push_back('#');
    std::ranges::rotate(middleInsert, middleInsert.end() - 1);
    map.push_back(std::vector(73, '#'));
    for (int i = 0; i < 71; ++i) { map.push_back(middleInsert); }
    map.push_back(std::vector(73, '#'));

    std::pair<int, int> target{71, 71};

    // Let the first 1024 bytes 'fall' on the map
    for (int i = 0; i < 1024; i++) { map[fallingBytes[i].first][fallingBytes[i].second] = '#'; }

    incc::doubleBuffer<std::vector<std::pair<int, int>>> nodesToExplore;
    nodesToExplore.getCurrent().push_back({1, 1});

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::vector stepsFromStartMap(map.size(), std::vector(map.front().size(), LLONG_MIN));
    stepsFromStartMap[1][1] = 0;

    while (not nodesToExplore.getCurrent().empty()) {
        for (auto const &node : nodesToExplore.getCurrent()) {
            for (auto const &dr : dirs) {
                if (map[node.first + dr.front()][node.second + dr.back()] == '.') {
                    map[node.first + dr.front()][node.second + dr.back()] = 'E';
                    stepsFromStartMap[node.first + dr.front()][node.second + dr.back()] =
                        stepsFromStartMap[node.first][node.second] + 1;
                    nodesToExplore.getNext().push_back({node.first + dr.front(), node.second + dr.back()});
                }
            }
        }
        nodesToExplore.getCurrent().clear();
        nodesToExplore.swapBuffers();
    }
    return stepsFromStartMap[71][71];
}

std::string day18_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // Get a more reasonable input structure for the 'falling bytes'
    std::vector<std::pair<int, int>> fallingBytes;
    for (auto const &line : input) {
        fallingBytes.push_back({std::stoi(line.back()) + 1, std::stoi(line.front()) + 1});
    }

    // Create suitable map structure including 'padding'
    std::vector<std::vector<char>> map;
    std::vector<char>              middleInsert(71, '.');
    middleInsert.push_back('#');
    middleInsert.push_back('#');
    std::ranges::rotate(middleInsert, middleInsert.end() - 1);
    map.push_back(std::vector(73, '#'));
    for (int i = 0; i < 71; ++i) { map.push_back(middleInsert); }
    map.push_back(std::vector(73, '#'));

    auto map_BU = map;

    std::pair<int, int> target{71, 71};

    // Let the first 1024 bytes 'fall' on the map
    for (int i = 0; i < 1024; i++) { map[fallingBytes[i].first][fallingBytes[i].second] = '#'; }

    incc::doubleBuffer<std::vector<std::pair<int, int>>> nodesToExplore;
    constexpr std::array<std::array<int, 2>, 4>          dirs{-1, 0, 0, 1, 1, 0, 0, -1};
    std::vector stepsFromStartMap(map.size(), std::vector(map.front().size(), LLONG_MIN));

    auto reset_data = [&]() {
        map = map_BU;
        nodesToExplore.getCurrent().clear();
        nodesToExplore.getNext().clear();
        nodesToExplore.getCurrent().push_back({1, 1});
        stepsFromStartMap       = std::vector(map.size(), std::vector(map.front().size(), LLONG_MIN));
        stepsFromStartMap[1][1] = 0;
    };

    auto simulate_until = [&](size_t const fallingByte) -> long long {
        for (int i = 0; i < fallingByte; i++) { map[fallingBytes[i].first][fallingBytes[i].second] = '#'; }
        while (not nodesToExplore.getCurrent().empty()) {
            for (auto const &node : nodesToExplore.getCurrent()) {
                for (auto const &dr : dirs) {
                    if (map[node.first + dr.front()][node.second + dr.back()] == '.') {
                        map[node.first + dr.front()][node.second + dr.back()] = 'E';
                        stepsFromStartMap[node.first + dr.front()][node.second + dr.back()] =
                            stepsFromStartMap[node.first][node.second] + 1;
                        if (node.first + dr.front() == target.first && node.second + dr.back() == target.second) {
                            nodesToExplore.getCurrent().clear();
                            nodesToExplore.getNext().clear();
                            break;
                        }
                        nodesToExplore.getNext().push_back({node.first + dr.front(), node.second + dr.back()});
                    }
                }
            }
            nodesToExplore.getCurrent().clear();
            nodesToExplore.swapBuffers();
        }
        return stepsFromStartMap[target.first][target.second];
    };

    long long min = 1024;
    long long max = fallingBytes.size();
    long long mid = (min + max) / 2;

    while (min + 1 != max) {
        reset_data();
        long long resOfIter = simulate_until(mid);
        if (resOfIter == LLONG_MIN) { max = mid; }
        else { min = mid; }
        mid = (min + max) / 2;
    }

    std::string resStr = std::to_string(fallingBytes[max - 1].second - 1);
    resStr.push_back(',');
    resStr.append(std::to_string(fallingBytes[max - 1].first - 1));

    return resStr;
}
} // namespace AOC2024