#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>
#include <utility>
#include <vector>


namespace AOC2024 {
namespace incc = incom::commons;
long long day21_1(std::string dataFile) {
    auto any_ctre       = ctre::search<R"(.+)">;
    auto d_nonzero_ctre = ctre::search<R"([1-9]\d*)">;
    auto input          = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
    auto onlyDigits     = incc::parseInputUsingCTRE::processFile(dataFile, d_nonzero_ctre).front();

    std::vector<long long> numericParts;
    for (auto const &od_line : onlyDigits) { numericParts.push_back(std::stoll(od_line)); }

    std::vector<std::vector<char>> keyP{{'7', '8', '9'}, {'4', '5', '6'}, {'1', '2', '3'}, {'~', '0', 'A'}};
    std::vector<std::vector<char>> dirP{{'~', '^', 'A'}, {'<', 'v', '>'}};

    ankerl::unordered_dense::map<char, std::pair<int, int>> keyP_map{
        {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}},
        {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}}, {'~', {3, 0}}, {'0', {3, 1}}, {'A', {3, 2}}};

    ankerl::unordered_dense::map<char, std::pair<int, int>> dirP_map{{'~', {0, 0}}, {'^', {0, 1}}, {'A', {0, 2}},
                                                                     {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}};

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    struct KeyPad {
        ankerl::unordered_dense::map<char, std::pair<int, int>> keyMap;
        std::vector<std::vector<char>>                          kp;

        KeyPad(ankerl::unordered_dense::map<char, std::pair<int, int>> const &key_map,
               std::vector<std::vector<char>> const &pad, int const startRow, int const startCol)
            : keyMap(key_map), kp(pad) {}

        std::vector<std::vector<char>> planRoutesTo(char from, char to) {
            std::vector<char>              tmp;
            std::vector<std::vector<char>> res;
            std::pair<int, int>            tar = keyMap.at(to);
            std::pair<int, int>            src = keyMap.at(from);

            int const horDir = tar.first - src.first < 0 ? -1 : 1;
            int const verDir = tar.second - src.second < 0 ? -1 : 1;

            // Start with horizontal movement
            while (src.first != tar.first && kp[src.first][src.second] != '~') {
                src.first += horDir;
                if (horDir == -1) { tmp.push_back('^'); }
                else { tmp.push_back('v'); }
            }
            while (src.second != tar.second && kp[src.first][src.second] != '~') {
                src.second += verDir;
                if (verDir == -1) { tmp.push_back('<'); }
                else { tmp.push_back('>'); }
            }
            if (kp[src.first][src.second] != '~') { res.push_back(tmp); }
            tmp.clear();

            // Start with vertical movement
            src = keyMap.at(from);
            while (src.second != tar.second && kp[src.first][src.second] != '~') {
                src.second += verDir;
                if (verDir == -1) { tmp.push_back('<'); }
                else { tmp.push_back('>'); }
            }
            while (src.first != tar.first && kp[src.first][src.second] != '~') {
                src.first += horDir;
                if (horDir == -1) { tmp.push_back('^'); }
                else { tmp.push_back('v'); }
            }
            if (kp[src.first][src.second] != '~') { res.push_back(tmp); }
            assert((void("Impossible to find no routes from one key to any other key"), not res.empty()));

            for (auto &resItem : res) { resItem.push_back('A'); }
            return res;
        }
    };

    std::vector<KeyPad> keyPads;
    keyPads.push_back(KeyPad(keyP_map, keyP, 3, 2));
    keyPads.push_back(KeyPad(dirP_map, dirP, 0, 2));
    keyPads.push_back(KeyPad(dirP_map, dirP, 0, 2));
    keyPads.push_back(KeyPad(dirP_map, dirP, 0, 2));

    std::vector<ankerl::unordered_dense::map<std::pair<char, char>, long long>> keyPressesPerRoute(keyPads.size());


    auto computeNumOfButtonPresses = [&](std::vector<char> const &sequenceOfButtons,
                                         size_t const kp_to_use) -> long long {
        char      src = 'A';
        long long res = 0;

        for (auto const &buttonToPress : sequenceOfButtons) {
            res += keyPressesPerRoute.at(kp_to_use).at({src, buttonToPress});
            src  = buttonToPress;
        }
        return res;
    };


    for (int keyPadID = keyPads.size() - 1; keyPadID >= 0; keyPadID--) {
        for (auto const &keyLoc : keyPads[keyPadID].keyMap) {
            if (keyLoc.first == '~') { continue; }
            else {
                for (auto const &keyLoc_other : keyPads[keyPadID].keyMap) {
                    if (keyLoc_other.first == '~') { continue; }
                    else if (keyPadID == keyPads.size() - 1) {
                        keyPressesPerRoute.at(keyPadID).insert({{keyLoc.first, keyLoc_other.first}, 1});
                    }
                    else {
                        auto      possibleRoutes = keyPads[keyPadID].planRoutesTo(keyLoc.first, keyLoc_other.first);
                        long long toInput        = LLONG_MAX;
                        for (auto const &onePossiRoute : possibleRoutes) {
                            toInput = std::min(toInput, computeNumOfButtonPresses(onePossiRoute, keyPadID + 1));
                        }
                        keyPressesPerRoute.at(keyPadID).insert(
                            {{keyLoc.first, keyLoc_other.first}, std::move(toInput)});
                    }
                }
            }
        }
    }
    char      srcChar = 'A';
    long long ans     = 0;
    for (size_t lineID = 0; auto const &codeToType : input) {
        long long numOfHumanPresses = 0;
        for (auto const &buttonRequested : codeToType) {
            numOfHumanPresses += keyPressesPerRoute.front().at({srcChar, buttonRequested});
            srcChar            = buttonRequested;
        }
        ans += numOfHumanPresses * numericParts.at(lineID++);
    }
    return ans;
}

long long day21_2(std::string dataFile) {
    auto any_ctre       = ctre::search<R"(.+)">;
    auto d_nonzero_ctre = ctre::search<R"([1-9]\d*)">;
    auto input          = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
    auto onlyDigits     = incc::parseInputUsingCTRE::processFile(dataFile, d_nonzero_ctre).front();

    std::vector<long long> numericParts;
    for (auto const &od_line : onlyDigits) { numericParts.push_back(std::stoll(od_line)); }

    std::vector<std::vector<char>> keyP{{'7', '8', '9'}, {'4', '5', '6'}, {'1', '2', '3'}, {'~', '0', 'A'}};
    std::vector<std::vector<char>> dirP{{'~', '^', 'A'}, {'<', 'v', '>'}};

    ankerl::unordered_dense::map<char, std::pair<int, int>> keyP_map{
        {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}},
        {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}}, {'~', {3, 0}}, {'0', {3, 1}}, {'A', {3, 2}}};

    ankerl::unordered_dense::map<char, std::pair<int, int>> dirP_map{{'~', {0, 0}}, {'^', {0, 1}}, {'A', {0, 2}},
                                                                     {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}};

    constexpr std::array<std::array<int, 2>, 4> dirs{-1, 0, 0, 1, 1, 0, 0, -1};

    struct KeyPad {
        ankerl::unordered_dense::map<char, std::pair<int, int>> keyMap;
        std::vector<std::vector<char>>                          kp;

        KeyPad(ankerl::unordered_dense::map<char, std::pair<int, int>> const &key_map,
               std::vector<std::vector<char>> const &pad, int const startRow, int const startCol)
            : keyMap(key_map), kp(pad) {}

        std::vector<std::vector<char>> planRoutesTo(char from, char to) {
            std::vector<char>              tmp;
            std::vector<std::vector<char>> res;
            std::pair<int, int>            tar = keyMap.at(to);
            std::pair<int, int>            src = keyMap.at(from);

            int const horDir = tar.first - src.first < 0 ? -1 : 1;
            int const verDir = tar.second - src.second < 0 ? -1 : 1;

            // Start with horizontal movement
            while (src.first != tar.first && kp[src.first][src.second] != '~') {
                src.first += horDir;
                if (horDir == -1) { tmp.push_back('^'); }
                else { tmp.push_back('v'); }
            }
            while (src.second != tar.second && kp[src.first][src.second] != '~') {
                src.second += verDir;
                if (verDir == -1) { tmp.push_back('<'); }
                else { tmp.push_back('>'); }
            }
            if (kp[src.first][src.second] != '~') { res.push_back(tmp); }
            tmp.clear();

            // Start with vertical movement
            src = keyMap.at(from);
            while (src.second != tar.second && kp[src.first][src.second] != '~') {
                src.second += verDir;
                if (verDir == -1) { tmp.push_back('<'); }
                else { tmp.push_back('>'); }
            }
            while (src.first != tar.first && kp[src.first][src.second] != '~') {
                src.first += horDir;
                if (horDir == -1) { tmp.push_back('^'); }
                else { tmp.push_back('v'); }
            }
            if (kp[src.first][src.second] != '~') { res.push_back(tmp); }
            assert((void("Impossible to find no routes from one key to any other key"), not res.empty()));

            for (auto &resItem : res) { resItem.push_back('A'); }
            return res;
        }
    };

    std::vector<KeyPad> keyPads;
    keyPads.push_back(KeyPad(keyP_map, keyP, 3, 2));
    for (size_t i = 0; i < 26; ++i) { keyPads.push_back(KeyPad(dirP_map, dirP, 0, 2)); }


    std::vector<ankerl::unordered_dense::map<std::pair<char, char>, long long>> keyPressesPerRoute(keyPads.size());

    auto computeNumOfButtonPresses = [&](std::vector<char> const &sequenceOfButtons,
                                         size_t const kp_to_use) -> long long {
        char      src = 'A';
        long long res = 0;

        for (auto const &buttonToPress : sequenceOfButtons) {
            res += keyPressesPerRoute.at(kp_to_use).at({src, buttonToPress});
            src  = buttonToPress;
        }
        return res;
    };


    for (int keyPadID = keyPads.size() - 1; keyPadID >= 0; keyPadID--) {
        for (auto const &keyLoc : keyPads[keyPadID].keyMap) {
            if (keyLoc.first == '~') { continue; }
            else {
                for (auto const &keyLoc_other : keyPads[keyPadID].keyMap) {
                    if (keyLoc_other.first == '~') { continue; }
                    else if (keyPadID == keyPads.size() - 1) {
                        keyPressesPerRoute.at(keyPadID).insert({{keyLoc.first, keyLoc_other.first}, 1});
                    }
                    else {
                        auto      possibleRoutes = keyPads[keyPadID].planRoutesTo(keyLoc.first, keyLoc_other.first);
                        long long toInput        = LLONG_MAX;
                        for (auto const &onePossiRoute : possibleRoutes) {
                            toInput = std::min(toInput, computeNumOfButtonPresses(onePossiRoute, keyPadID + 1));
                        }
                        keyPressesPerRoute.at(keyPadID).insert(
                            {{keyLoc.first, keyLoc_other.first}, std::move(toInput)});
                    }
                }
            }
        }
    }
    char      srcChar = 'A';
    long long ans     = 0;
    for (size_t lineID = 0; auto const &codeToType : input) {
        long long numOfHumanPresses = 0;
        for (auto const &buttonRequested : codeToType) {
            numOfHumanPresses += keyPressesPerRoute.front().at({srcChar, buttonRequested});
            srcChar            = buttonRequested;
        }
        ans += numOfHumanPresses * numericParts.at(lineID++);
    }
    return ans;
}
} // namespace AOC2024