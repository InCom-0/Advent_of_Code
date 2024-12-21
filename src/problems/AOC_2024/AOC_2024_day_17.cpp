#include <climits>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/core.h>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;

struct _instrBase_2024 {
    std::reference_wrapper<long long> source;
    std::reference_wrapper<long long> target;
};

template <typename... instrT>
requires(std::derived_from<instrT, _instrBase_2024> && ...)
struct ProgramQuasiAssembly_LOC {
    std::unordered_map<char, std::reference_wrapper<long long>, incom::commons::XXH3Hasher> mapping;
    size_t                                                                                  instructionID = 0;
    long long                                                                               fakeRegister  = LLONG_MIN;
    std::vector<long long>                                                                  registers;
    std::vector<std::variant<instrT...>>                                                    instrVect;

    // The one and only constructor of the 'prog' type
    ProgramQuasiAssembly_LOC(const std::vector<std::vector<std::string>> &input, const long long regStartVal_A = 0,
                             const long long regStartVal_B = 0, const long long regStartVal_C = 0) {
        assert((void("Prog type instantiated with an empty input"), input.size() > 0));
        for (auto &line : input) {
            assert((void("Prog type instantiated with an input that has more than 3 items on some instruction line"),
                    line.size() < 4));
            assert(
                (void("Prog type instantiated with an input that has some instruction line empty"), line.size() > 0));
        }

        // Mapping a type 'by string' to the same type inside a std::variant instance;
        // TypeToString uses a very crude form of 'reflection'.
        // ENTIRELY POSSIBLE THAT THIS HACK IS NOT REALLY PORTABLE ... BEWARE.
        std::unordered_map<std::string, std::variant<instrT...>, incom::commons::XXH3Hasher> instrTypeMap;
        std::unordered_map<std::string, std::string, incom::commons::XXH3Hasher>             instrTypeRemap_bits2String;
        (instrTypeMap.emplace(incc::PQA::TypeToString<instrT>(),
                              std::variant<instrT...>{instrT{fakeRegister, fakeRegister}}),
         ...);

        int remapID = 0;
        (instrTypeRemap_bits2String.emplace(std::to_string(remapID++), incc::PQA::TypeToString<instrT>()), ...);

        registers.reserve(input.size() * 2); // Must NEVER reallocate.

        for (auto &line : input) {
            assert((void("Some instructions name in input doesn't match any type in template parameter pack"),
                    instrTypeRemap_bits2String.contains(line.front())));
        }
        // Create the registers A, B, C ... but actually looking them up by '4', '5', '6'
        registers.push_back(regStartVal_A);
        mapping.emplace('4', registers.back());
        registers.push_back(regStartVal_B);
        mapping.emplace('5', registers.back());
        registers.push_back(regStartVal_C);
        mapping.emplace('6', registers.back());

        for (auto &line : input) {
            std::reference_wrapper<long long> firstR  = fakeRegister;
            std::reference_wrapper<long long> secondR = fakeRegister;

            if (std::visit([&](auto &&a) -> bool { return a.has_comboOper(); },
                           instrTypeMap.at(instrTypeRemap_bits2String.at(line.front()))) == true) {

                if (line[1].front() >= '4' && line[1].front() <= '6') { firstR = mapping.at(line[1][0]); }
                else {
                    // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register
                    // used only in that instruction
                    registers.push_back(std::stoi(line[1]));
                    firstR = registers.back();
                }
            }
            else {
                registers.push_back(std::stoi(line[1]));
                firstR = registers.back();
            }
            if (line.size() >= 3) {
                if (line[2].front() >= 'a' && line[2].front() <= 'z') { secondR = mapping.at(line[2][0]); }
                else {
                    // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register
                    // used only in that instruction
                    registers.push_back(std::stoi(line[2]));
                    secondR = registers.back();
                }
            }

            // Update just the right variant item in the instrTypeMap.
            std::visit(
                [&](auto &&a) {
                    a.source = firstR;
                    a.target = secondR;
                },
                instrTypeMap.at(instrTypeRemap_bits2String.at(line.front())));

            // Push the right variant inside instrVect
            instrVect.push_back(std::variant<instrT...>(instrTypeMap.at(instrTypeRemap_bits2String.at(line.front()))));
        }
        return;
    }

    // Other convenience member functions
    inline bool                     test_isInstructionIDvalid() { return instructionID < instrVect.size(); }
    inline std::variant<instrT...> &getCurrentAndIncrement() { return instrVect[instructionID++]; }
};

struct adv : _instrBase_2024 {
    constexpr bool has_comboOper() { return true; }
};
struct bxl : _instrBase_2024 {
    constexpr bool has_comboOper() { return false; }
};
struct bst : _instrBase_2024 {
    constexpr bool has_comboOper() { return true; }
};
struct jnz : _instrBase_2024 {
    constexpr bool has_comboOper() { return false; }
};
struct bxc : _instrBase_2024 {
    constexpr bool has_comboOper() { return false; }
};
struct out : _instrBase_2024 {
    constexpr bool has_comboOper() { return true; }
};
struct bdv : _instrBase_2024 {
    constexpr bool has_comboOper() { return true; }
};
struct cdv : _instrBase_2024 {
    constexpr bool has_comboOper() { return true; }
};
std::string day17_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    auto splt = std::views::chunk(input.back(), 2) | std::ranges::to<std::vector<std::vector<std::string>>>();

    ProgramQuasiAssembly_LOC<adv, bxl, bst, jnz, bxc, out, bdv, cdv> pqa(
        splt, std::stoll(input[0].front()), std::stoll(input[1].front()), std::stoll(input[2].front()));

    std::string res          = "";
    auto        overload_obj = overloaded{
        [&](const adv &a) {
            pqa.registers[0] = static_cast<long long>(std::trunc(pqa.registers[0] / std::pow(2, a.source.get())));
        },
        [&](const bxl &a) { pqa.registers[1] ^= a.source.get(); },
        [&](const bst &a) { pqa.registers[1] = a.source.get() % 8; },
        [&](const jnz &a) {
            if (pqa.registers[0] != 0) { pqa.instructionID = a.source.get() / 2; }
        },
        [&](const bxc &a) { pqa.registers[1] ^= pqa.registers[2]; },
        [&](const out &a) {
            // fmt::print("{0},", a.source.get() % 8);
            res.push_back(a.source.get() % 8 + 48);
            res.push_back(',');
        },
        [&](const bdv &a) {
            pqa.registers[1] = static_cast<long long>(std::trunc(pqa.registers[0] / std::pow(2, a.source.get())));
        },
        [&](const cdv &a) {
            pqa.registers[2] = static_cast<long long>(std::trunc(pqa.registers[0] / std::pow(2, a.source.get())));
        },

    };

    while (pqa.test_isInstructionIDvalid()) { std::visit(overload_obj, pqa.getCurrentAndIncrement()); }

    res.pop_back();
    return res;
}

long long day17_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    auto splt = std::views::chunk(input.back(), 2) | std::ranges::to<std::vector<std::vector<std::string>>>();

    std::vector<long long> reversedToExpect = std::views::reverse(input.back()) |
                                              std::views::transform([](auto const &a) { return std::stoll(a); }) |
                                              std::ranges::to<std::vector>();

    long long outSingleVal;

    ProgramQuasiAssembly_LOC<adv, bxl, bst, jnz, bxc, out, bdv, cdv> pqa(
        splt, std::stoll(input[0].front()), std::stoll(input[1].front()), std::stoll(input[2].front()));

    auto overload_obj = overloaded{
        [&](const adv &a) {
            pqa.registers[0] = static_cast<long long>(std::trunc(pqa.registers[0] / std::pow(2, a.source.get())));
        },
        [&](const bxl &a) { pqa.registers[1] ^= a.source.get(); },
        [&](const bst &a) { pqa.registers[1] = a.source.get() % 8; },
        [&](const jnz &a) {
            if (pqa.registers[0] != 0) { pqa.instructionID = a.source.get() / 2; }
        },
        [&](const bxc &a) { pqa.registers[1] ^= pqa.registers[2]; },
        [&](const out &a) {
            outSingleVal      = a.source.get() % 8;
            pqa.instructionID = SIZE_MAX;
        },
        [&](const bdv &a) {
            pqa.registers[1] = static_cast<long long>(std::trunc(pqa.registers[0] / std::pow(2, a.source.get())));
        },
        [&](const cdv &a) {
            pqa.registers[2] = static_cast<long long>(std::trunc(pqa.registers[0] / std::pow(2, a.source.get())));
        },
    };

    int  curLvl     = -1;
    auto rec_solver = [&](this auto const &self, long long afterVal) -> std::optional<long long> {
        curLvl++;
        afterVal *= 8;
        for (int zeroToSeven = 0 + (afterVal == 0); zeroToSeven < 8; ++zeroToSeven) {
            pqa.instructionID = 0;
            pqa.registers[0]  = afterVal + zeroToSeven;
            pqa.registers[1]  = 0;
            pqa.registers[2]  = 0;

            while (pqa.test_isInstructionIDvalid()) { std::visit(overload_obj, pqa.getCurrentAndIncrement()); }
            if (outSingleVal == reversedToExpect[curLvl]) {
                if (curLvl + 1 == reversedToExpect.size()) {
                    curLvl--;
                    return afterVal + zeroToSeven;
                }

                auto resOfThis = self(afterVal + zeroToSeven);
                if (resOfThis.has_value()) {
                    curLvl--;
                    return resOfThis;
                }
            }
        }
        curLvl--;
        return std::nullopt;
    };

    auto res = rec_solver(0);
    return res.value_or(LLONG_MIN);
}
} // namespace AOC2024