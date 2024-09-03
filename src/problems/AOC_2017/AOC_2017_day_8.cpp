#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2017 {

int day8_1(std::string dataFile) {
    auto w_ctre     = ctre::search<R"([^ ]+(?=[[:blank:]]))">;
    auto w_ctre_end = ctre::search<R"((?<= )-?.*)">;

    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre, w_ctre, w_ctre, w_ctre, w_ctre,
                                                                 w_ctre, w_ctre_end);

    // DATA PREP
    std::vector<int>                     registers;
    std::unordered_map<std::string, int> regMap;

    for (auto &line : VofV) {
        regMap[line[0]] = INT_MIN;
        regMap[line[4]] = INT_MIN;
    }
    registers.reserve(regMap.size());

    for (int idx = 0; auto &mpItem : regMap) {
        registers.push_back(0);
        mpItem.second = idx++;
    }

    // CUSTOM LOCAL TYPES
    struct _instrBase {
        int  valInput_1 = INT_MIN;
        int  valInput_2 = INT_MIN;
        int *source     = nullptr;
        int *target     = nullptr;
    };

    struct inc : _instrBase {};
    struct dec : _instrBase {};

    struct ifEQ : _instrBase {};
    struct ifNEQ : _instrBase {};
    struct ifGT : _instrBase {};
    struct ifGTE : _instrBase {};
    struct ifLT : _instrBase {};
    struct ifLTE : _instrBase {};

    using actInstrType = std::variant<inc, dec>;
    using ifInstrType  = std::variant<ifEQ, ifNEQ, ifGT, ifGTE, ifLT, ifLTE>;

    std::pair<actInstrType, ifInstrType>              oneInstr;
    std::vector<std::pair<actInstrType, ifInstrType>> instructions;
    for (auto &line : VofV) {
        if (line[1] == "inc") {
            oneInstr.first = inc{INT_MIN, std::stoi(line[2]), nullptr, &(registers[regMap[line[0]]])};
        }
        else if (line[1] == "dec") {
            oneInstr.first = dec{INT_MIN, std::stoi(line[2]), nullptr, &(registers[regMap[line[0]]])};
        }
        else { return INT_MIN; }

        if (line[5] == "==") {
            oneInstr.second = ifEQ{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == "!=") {
            oneInstr.second = ifNEQ{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == ">") {
            oneInstr.second = ifGT{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == ">=") {
            oneInstr.second = ifGTE{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == "<") {
            oneInstr.second = ifLT{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == "<=") {
            oneInstr.second = ifLTE{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else { return INT_MIN; }

        instructions.push_back(oneInstr);
    }

    // MAIN LOGIC
    int  curInstrID      = 0;
    auto actInstrVisitor = overloaded{
        [&](const inc &a) { (*a.target) += a.valInput_2; },
        [&](const dec &a) { (*a.target) -= a.valInput_2; },
    };

    while (curInstrID < instructions.size()) {
        std::visit(overloaded{
                       [&](const ifEQ &a) {
                           if ((*a.source) == a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifNEQ &a) {
                           if ((*a.source) != a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifGT &a) {
                           if ((*a.source) > a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifGTE &a) {
                           if ((*a.source) >= a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifLT &a) {
                           if ((*a.source) < a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifLTE &a) {
                           if ((*a.source) <= a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                   },
                   instructions[curInstrID].second);
    }

    return flux::ref(registers).max([](auto &&a, auto &&b) { return a < b; }).value_or(INT_MIN);
}
int day8_2(std::string dataFile) {
    auto w_ctre     = ctre::search<R"([^ ]+(?=[[:blank:]]))">;
    auto w_ctre_end = ctre::search<R"((?<= )-?.*)">;

    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre, w_ctre, w_ctre, w_ctre, w_ctre,
                                                                 w_ctre, w_ctre_end);

    // DATA PREP
    std::vector<int>                     registers;
    std::unordered_map<std::string, int> regMap;

    for (auto &line : VofV) {
        regMap[line[0]] = INT_MIN;
        regMap[line[4]] = INT_MIN;
    }
    registers.reserve(regMap.size());

    for (int idx = 0; auto &mpItem : regMap) {
        registers.push_back(0);
        mpItem.second = idx++;
    }

    // CUSTOM LOCAL TYPES
    struct _instrBase {
        int  valInput_1 = INT_MIN;
        int  valInput_2 = INT_MIN;
        int *source     = nullptr;
        int *target     = nullptr;
    };

    struct inc : _instrBase {};
    struct dec : _instrBase {};

    struct ifEQ : _instrBase {};
    struct ifNEQ : _instrBase {};
    struct ifGT : _instrBase {};
    struct ifGTE : _instrBase {};
    struct ifLT : _instrBase {};
    struct ifLTE : _instrBase {};

    using actInstrType = std::variant<inc, dec>;
    using ifInstrType  = std::variant<ifEQ, ifNEQ, ifGT, ifGTE, ifLT, ifLTE>;

    std::pair<actInstrType, ifInstrType>              oneInstr;
    std::vector<std::pair<actInstrType, ifInstrType>> instructions;
    for (auto &line : VofV) {
        if (line[1] == "inc") {
            oneInstr.first = inc{INT_MIN, std::stoi(line[2]), nullptr, &(registers[regMap[line[0]]])};
        }
        else if (line[1] == "dec") {
            oneInstr.first = dec{INT_MIN, std::stoi(line[2]), nullptr, &(registers[regMap[line[0]]])};
        }
        else { return INT_MIN; }

        if (line[5] == "==") {
            oneInstr.second = ifEQ{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == "!=") {
            oneInstr.second = ifNEQ{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == ">") {
            oneInstr.second = ifGT{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == ">=") {
            oneInstr.second = ifGTE{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == "<") {
            oneInstr.second = ifLT{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else if (line[5] == "<=") {
            oneInstr.second = ifLTE{INT_MIN, std::stoi(line[6]), &(registers[regMap[line[4]]]), nullptr};
        }
        else { return INT_MIN; }

        instructions.push_back(oneInstr);
    }

    // MAIN LOGIC
    int  maxSoFar     = 0;
    auto updateCurMax = [&](const int &changedReg) { maxSoFar = std::max(maxSoFar, changedReg); };

    int  curInstrID      = 0;
    auto actInstrVisitor = overloaded{
        [&](const inc &a) {
            (*a.target) += a.valInput_2;
            updateCurMax(*a.target);
        },
        [&](const dec &a) {
            (*a.target) -= a.valInput_2;
            updateCurMax(*a.target);
        },
    };

    while (curInstrID < instructions.size()) {
        std::visit(overloaded{
                       [&](const ifEQ &a) {
                           if ((*a.source) == a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifNEQ &a) {
                           if ((*a.source) != a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifGT &a) {
                           if ((*a.source) > a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifGTE &a) {
                           if ((*a.source) >= a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifLT &a) {
                           if ((*a.source) < a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                       [&](const ifLTE &a) {
                           if ((*a.source) <= a.valInput_2) {
                               std::visit(actInstrVisitor, instructions[curInstrID].first);
                           }
                           curInstrID++;
                       },
                   },
                   instructions[curInstrID].second);
    }

    return maxSoFar;
}

} // namespace AOC2017