#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>


namespace AOC2016 {
int day4_1(std::string dataFile) {
    auto inp = incom::commons::parseInputUsingCTRE::processFile(
        dataFile, ctre::search<R"(.+(?=-\d))">, ctre::search<R"(\d+(?=\[))">, ctre::search<R"((?<=\[)\w+)">);

    std::vector<std::vector<std::pair<char, int>>> vectOfRoomMaps;
    for (auto roomStr : inp[0]) {
        std::unordered_map<char, int> tempMap;
        for (auto &charInRoomStr : roomStr) {
            if (charInRoomStr == '-') { continue; }
            if (tempMap.contains(charInRoomStr)) { tempMap.at(charInRoomStr)++; }
            else { tempMap.emplace(charInRoomStr, 1); }
        }
        vectOfRoomMaps.push_back(std::vector<std::pair<char, int>>(tempMap.begin(), tempMap.end()));
    }

    for (auto &rmMap : vectOfRoomMaps) {
        std::ranges::sort(rmMap, [](auto &&a, auto &&b) {
            if (a.second > b.second) { return true; }
            if (a.second == b.second) { return a.first < b.first; }
            return false;
        });
    }
    auto res = flux::zip(flux::ref(vectOfRoomMaps), flux::ref(inp[2]),
                         flux::ref(inp[1]).map([](auto &&g) { return std::stoi(g); }))
                   .filter([](auto &&a) {
                       return flux::zip(flux::ref(std::get<0>(a)), flux::ref(std::get<1>(a))).all([](auto &&aaa) {
                           return aaa.first.first == aaa.second;
                       });
                   })
                   .fold([](auto &&accu, auto &&b) { return accu + std::get<2>(b); }, 0);

    return res;
}

int day4_2(std::string dataFile) {
    auto inp = incom::commons::parseInputUsingCTRE::processFile(
        dataFile, ctre::search<R"(.+(?=-\d))">, ctre::search<R"(\d+(?=\[))">, ctre::search<R"((?<=\[)\w+)">);

    std::vector<std::vector<std::pair<char, int>>> vectOfRoomMaps;
    for (auto roomStr : inp[0]) {
        std::unordered_map<char, int> tempMap; // might be better to use a find on vector<pair<char,int>> ... max
                                               // element size 32 finding single char
        for (auto &charInRoomStr : roomStr) {
            if (charInRoomStr == '-') { continue; }
            if (tempMap.contains(charInRoomStr)) { tempMap.at(charInRoomStr)++; }
            else { tempMap.emplace(charInRoomStr, 1); }
        }
        vectOfRoomMaps.push_back(std::vector<std::pair<char, int>>(tempMap.begin(), tempMap.end()));
    }

    for (auto &rmMap : vectOfRoomMaps) {
        std::ranges::sort(rmMap, [](auto &&a, auto &&b) {
            if (a.second > b.second) { return true; }
            if (a.second == b.second) { return a.first < b.first; }
            return false;
        });
    }

    auto res = flux::zip(flux::ref(inp[0]), flux::ref(inp[1]).map([](auto &&g) { return std::stoi(g); }),
                         flux::ref(inp[2]), flux::ref(vectOfRoomMaps))
                   .filter([](auto &&a) {
                       return flux::zip(flux::ref(std::get<3>(a)), flux::ref(std::get<2>(a))).all([](auto &&aaa) {
                           return aaa.first.first == aaa.second;
                       });
                   })
                   .map([alfabetSize = 'z' - 'a' + 1](auto &&a) {
                       int moveBy = std::get<1>(a) % alfabetSize;

                       std::string decryptedStr;
                       for (auto &roomNameChar : std::get<0>(a)) {
                           if (roomNameChar == '-') { decryptedStr.push_back(' '); }
                           else {
                               if ('z' - roomNameChar >= moveBy) { decryptedStr.push_back(roomNameChar + moveBy); }
                               else { decryptedStr.push_back('a' + (moveBy - ('z' - roomNameChar)) - 1); }
                           }
                       }
                       return std::pair<std::string, int>(decryptedStr, std::get<1>(a));
                   })
                   .filter([](auto &&a) { return a.first.contains("northpole"); })
                   .to<std::vector<std::pair<std::string, int>>>();

    return res.front().second;
}

} // namespace AOC2016