#include <fstream>

#include <incom_commons.h>
#include <ctre.hpp>

#include <flux.hpp>
#include <md5.h>


namespace AOC2016 {

std::string day21_1(std::string dataFile, std::string &&input) {
    struct ActionBase {
        char letter_1;
        char letter_2;
        int  pos_1 = 0;
        int  pos_2 = 0;
    };
    struct SwapPoss : ActionBase {};
    struct SwapLetter : ActionBase {};
    struct RotR : ActionBase {};
    struct RotL : ActionBase {};
    struct RotBOP : ActionBase {};
    struct Reverse_X_through_Y : ActionBase {};
    struct MovePos_x_To_y : ActionBase {};

    std::vector<std::variant<SwapPoss, SwapLetter, RotR, RotL, RotBOP, Reverse_X_through_Y, MovePos_x_To_y>>
        actionsToTake;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    while (std::getline(iStream, oneStr)) {
        if (oneStr.starts_with("swap position ")) {
            actionsToTake.push_back(SwapPoss{0, 0, oneStr[14] - 48, oneStr.back() - 48});
        }
        else if (oneStr.starts_with("swap letter ")) { actionsToTake.push_back(SwapLetter{oneStr[12], oneStr.back()}); }
        else if (oneStr.starts_with("rotate right ")) { actionsToTake.push_back(RotR{0, 0, oneStr[13] - 48}); }
        else if (oneStr.starts_with("rotate left ")) { actionsToTake.push_back(RotL{0, 0, oneStr[12] - 48}); }
        else if (oneStr.starts_with("rotate based ")) { actionsToTake.push_back(RotBOP{oneStr.back()}); }
        else if (oneStr.starts_with("reverse positions ")) {
            actionsToTake.push_back(Reverse_X_through_Y{0, 0, oneStr[18] - 48, oneStr.back() - 48});
        }
        else if (oneStr.starts_with("move position ")) {
            actionsToTake.push_back(MovePos_x_To_y{0, 0, oneStr[14] - 48, oneStr.back() - 48});
        }

        else { return "ERROR"; }
    }

    for (auto &oneAction : actionsToTake) {
        std::visit(overloaded{
                       [&](const SwapPoss &a) -> void { std::swap(input[a.pos_1], input[a.pos_2]); },
                       [&](const SwapLetter &a) -> void {
                           std::swap(input[input.find(a.letter_1)], input[input.find(a.letter_2)]);
                       },
                       [&](const RotR &a) -> void {
                           std::rotate(input.rbegin(), input.rbegin() + a.pos_1 % input.size(), input.rend());
                       },
                       [&](const RotL &a) -> void {
                           std::rotate(input.begin(), input.begin() + a.pos_1 % input.size(), input.end());
                       },
                       [&](const RotBOP &a) -> void {
                           std::rotate(input.rbegin(),
                                       input.rbegin() + (input.find(a.letter_1) > 3 ? input.find(a.letter_1) + 2
                                                                                    : input.find(a.letter_1) + 1) %
                                                            input.size(),
                                       input.rend());
                       },
                       [&](const Reverse_X_through_Y &a) -> void {
                           std::reverse(input.begin() + a.pos_1, input.begin() + a.pos_2 + 1);
                       },
                       [&](const MovePos_x_To_y &a) -> void {
                           const char tmpChar = input[a.pos_1];
                           input.erase(a.pos_1, 1);
                           input.insert(a.pos_2, 1, tmpChar);
                       },
                   },
                   oneAction);
    }
    return input;
}

std::string day21_2(std::string dataFile, std::string &&input) {
    struct ActionBase {
        char letter_1;
        char letter_2;
        int  pos_1 = 0;
        int  pos_2 = 0;
    };
    struct SwapPoss : ActionBase {};
    struct SwapLetter : ActionBase {};
    struct RotR : ActionBase {};
    struct RotL : ActionBase {};
    struct RotBOP : ActionBase {};
    struct Reverse_X_through_Y : ActionBase {};
    struct MovePos_x_To_y : ActionBase {};

    std::vector<std::variant<SwapPoss, SwapLetter, RotR, RotL, RotBOP, Reverse_X_through_Y, MovePos_x_To_y>>
        actionsToTake;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    while (std::getline(iStream, oneStr)) {
        if (oneStr.starts_with("swap position ")) {
            actionsToTake.push_back(SwapPoss{0, 0, oneStr[14] - 48, oneStr.back() - 48});
        }
        else if (oneStr.starts_with("swap letter ")) { actionsToTake.push_back(SwapLetter{oneStr[12], oneStr.back()}); }
        else if (oneStr.starts_with("rotate right ")) { actionsToTake.push_back(RotR{0, 0, oneStr[13] - 48}); }
        else if (oneStr.starts_with("rotate left ")) { actionsToTake.push_back(RotL{0, 0, oneStr[12] - 48}); }
        else if (oneStr.starts_with("rotate based ")) { actionsToTake.push_back(RotBOP{oneStr.back()}); }
        else if (oneStr.starts_with("reverse positions ")) {
            actionsToTake.push_back(Reverse_X_through_Y{0, 0, oneStr[18] - 48, oneStr.back() - 48});
        }
        else if (oneStr.starts_with("move position ")) {
            actionsToTake.push_back(MovePos_x_To_y{0, 0, oneStr[14] - 48, oneStr.back() - 48});
        }

        else { return "ERROR"; }
    }

    std::vector preComputed_RotBOP_sources(input.size(), std::pair(INT_MIN, INT_MIN));

    for (int i = 0; i < input.size(); ++i) {
        int target                                               = i > 3 ? i + i + 2 : i + i + 1;
        preComputed_RotBOP_sources[target % input.size()].first  = i;
        preComputed_RotBOP_sources[target % input.size()].second = target % input.size();
    }

    /*
    Note that RotR actually rotates Left and RotL actually rotates Right here, because they need to be reversed.
    RoTBOP action was immensly annoying as it has 4 distinct cases based on odd vs. even && >3 vs. not >3.
    */
    for (auto &oneAction : std::views::reverse(actionsToTake)) {
        std::visit(overloaded{
                       [&](const SwapPoss &a) -> void { std::swap(input[a.pos_1], input[a.pos_2]); },
                       [&](const SwapLetter &a) -> void {
                           std::swap(input[input.find(a.letter_1)], input[input.find(a.letter_2)]);
                       },
                       [&](const RotR &a) -> void {
                           std::rotate(input.begin(), input.begin() + a.pos_1 % input.size(), input.end());
                       },
                       [&](const RotL &a) -> void {
                           std::rotate(input.rbegin(), input.rbegin() + a.pos_1 % input.size(), input.rend());
                       },
                       [&](const RotBOP &a) -> void {
                           size_t nowPos  = input.find(a.letter_1);
                           size_t prevPos = preComputed_RotBOP_sources[nowPos].first;

                           if (prevPos > nowPos) { prevPos = input.size() + (nowPos - prevPos); }
                           else { prevPos = nowPos - prevPos; }

                           std::rotate(input.begin(), input.begin() + prevPos, input.end());
                       },
                       [&](const Reverse_X_through_Y &a) -> void {
                           std::reverse(input.begin() + a.pos_1, input.begin() + a.pos_2 + 1);
                       },
                       [&](const MovePos_x_To_y &a) -> void {
                           const char tmpChar = input[a.pos_2];
                           input.erase(a.pos_2, 1);
                           input.insert(a.pos_1, 1, tmpChar);
                       },
                   },
                   oneAction);
    }
    return input;
}

} // namespace AOC2016
