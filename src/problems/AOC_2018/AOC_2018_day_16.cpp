#include <incom_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <variant>


namespace AOC2018 {


template <typename OverloadSet, typename... _instrT>
requires(std::derived_from<_instrT, incom::commons::PQA::_instrBase_2018> && ...)
int day16_1_matcher(const std::vector<std::vector<long long>> &parsed, const OverloadSet &olSet,
                    std::vector<long long> &registers) {

    std::vector<std::variant<_instrT...>> variantVector;
    (variantVector.push_back(_instrT{registers[parsed[1][1]], registers[parsed[1][2]], registers[parsed[1][3]]}), ...);

    for (int i = 1; i < 3; ++i) { registers[15 + i] = parsed[1][i]; }

    int validForXInstructions = 0;
    for (int j = 0; j < sizeof...(_instrT); ++j) {
        for (int i = 0; i < 4; ++i) { registers[i] = parsed[0][i]; }
        std::visit(olSet, variantVector[j]);

        int innerCounter = 0;
        for (int k = 0; k < 4; ++k) {
            if (registers[k] == parsed[2][k]) { innerCounter++; }
        }
        if (innerCounter == 4) { validForXInstructions++; }
    }
    return validForXInstructions;
}


int day16_1(std::string dataFile, int behaveLikeXorMoreOpCodes) {
    struct addr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct addi : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct mulr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct muli : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct banr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct bani : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct borr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct bori : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct setr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };
    struct seti : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {1, 1, 0}; }
    };

    struct gtir : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {1, 0, 0}; }
    };
    struct gtri : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };
    struct gtrr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };

    struct eqir : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {1, 0, 0}; }
    };
    struct eqri : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };
    struct eqrr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };

    auto d_ctre = ctre::search<R"(\d+)">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);

    std::vector<std::vector<long long>> parsed(4, std::vector<long long>());

    std::string tempStr;

    auto parseOneSet = [&]() -> bool {
        int counter = 0;
        for (int i = 0; i < 4; ++i) { parsed[i].clear(); }
        for (int i = 0; i < 4; ++i) {
            std::getline(iStream, tempStr);
            auto split = incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(tempStr, d_ctre);
            if (split.empty()) { continue; }
            else {
                counter++;
                for (auto &oneStr : split) { parsed[i].push_back(std::stoll(oneStr)); }
            }
        }
        if (counter != 3) { return false; }
        else { return true; }
    };

    std::vector<long long>            registers(32, INT_MIN); // 0 to 3 are real registers, 4 to 7 are fake registers
    std::reference_wrapper<long long> firstVal  = registers[16];
    std::reference_wrapper<long long> secondVal = registers[17];

    auto overloadSet = overloaded{
        [&](addr &a) { a.C.get() = a.A.get() + a.B.get(); },
        [&](addi &a) { a.C.get() = a.A.get() + secondVal.get(); },

        [&](mulr &a) { a.C.get() = a.A.get() * a.B.get(); },
        [&](muli &a) { a.C.get() = a.A.get() * secondVal.get(); },

        [&](banr &a) { a.C.get() = a.A.get() & a.B.get(); },
        [&](bani &a) { a.C.get() = a.A.get() & secondVal.get(); },

        [&](borr &a) { a.C.get() = a.A.get() | a.B.get(); },
        [&](bori &a) { a.C.get() = a.A.get() | secondVal.get(); },

        [&](setr &a) { a.C.get() = a.A.get(); },
        [&](seti &a) { a.C.get() = firstVal.get(); },

        [&](gtir &a) { a.C.get() = firstVal.get() > a.B.get(); },
        [&](gtri &a) { a.C.get() = a.A.get() > secondVal.get(); },
        [&](gtrr &a) { a.C.get() = a.A.get() > a.B.get(); },

        [&](eqir &a) { a.C.get() = firstVal.get() == a.B.get(); },
        [&](eqri &a) { a.C.get() = a.A.get() == secondVal.get(); },
        [&](eqrr &a) { a.C.get() = a.A.get() == a.B.get(); },
    };

    int trueCounter = 0;
    while (parseOneSet()) {
        if (day16_1_matcher<decltype(overloadSet), addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir,
                            gtri, gtrr, eqir, eqri, eqrr>(parsed, overloadSet, registers) >= behaveLikeXorMoreOpCodes) {
            trueCounter++;
        }
    }
    return trueCounter;
}

long long day16_2(std::string dataFile_1, std::string dataFile_2) {
    struct addr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct addi : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct mulr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct muli : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct banr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct bani : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct borr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };
    struct bori : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };

    struct setr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };
    struct seti : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {1, 1, 0}; }
    };

    struct gtir : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {1, 0, 0}; }
    };
    struct gtri : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };
    struct gtrr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };

    struct eqir : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {1, 0, 0}; }
    };
    struct eqri : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 1, 0}; }
    };
    struct eqrr : incom::commons::PQA::_instrBase_2018 {
        using incom::commons::PQA::_instrBase_2018::_instrBase_2018;
        const std::vector<int> getRS() override { return {0, 0, 0}; }
    };

    auto d_ctre = ctre::search<R"(\d+)">;

    auto rawExampleInput = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile_1, d_ctre);
    auto rawInstrInput   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile_2, d_ctre);

    auto overloadSet = overloaded{
        [&](addr &a) { a.C.get() = a.A.get() + a.B.get(); },
        [&](addi &a) { a.C.get() = a.A.get() + a.B.get(); },

        [&](mulr &a) { a.C.get() = a.A.get() * a.B.get(); },
        [&](muli &a) { a.C.get() = a.A.get() * a.B.get(); },

        [&](banr &a) { a.C.get() = a.A.get() & a.B.get(); },
        [&](bani &a) { a.C.get() = a.A.get() & a.B.get(); },

        [&](borr &a) { a.C.get() = a.A.get() | a.B.get(); },
        [&](bori &a) { a.C.get() = a.A.get() | a.B.get(); },

        [&](setr &a) { a.C.get() = a.A.get(); },
        [&](seti &a) { a.C.get() = a.A.get(); },

        [&](gtir &a) { a.C.get() = a.A.get() > a.B.get(); },
        [&](gtri &a) { a.C.get() = a.A.get() > a.B.get(); },
        [&](gtrr &a) { a.C.get() = a.A.get() > a.B.get(); },

        [&](eqir &a) { a.C.get() = a.A.get() == a.B.get(); },
        [&](eqri &a) { a.C.get() = a.A.get() == a.B.get(); },
        [&](eqrr &a) { a.C.get() = a.A.get() == a.B.get(); },
    };

    auto instrTypeMap =
        incom::commons::PQA::ProgramQuasiAssembly_2018<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir,
                                                    gtri, gtrr, eqir, eqri, eqrr>::instrTypeMapCreator(rawExampleInput,
                                                                                                       overloadSet);

    incom::commons::PQA::ProgramQuasiAssembly_2018<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri,
                                                gtrr, eqir, eqri, eqrr>
        pqa_2018(rawInstrInput, instrTypeMap);


    while (pqa_2018.test_isInstructionIDvalid()) { std::visit(overloadSet, pqa_2018.getCurrentAndIncrement()); }

    return pqa_2018.registers[0];
}
} // namespace AOC2018