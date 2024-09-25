#include <incom_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>


namespace AOC2018 {

long long day19_1(std::string dataFile, const std::vector<long long> regStartVal) {
    // TYPE DEFINITIONS
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

    // INPUT PROCESSING
    auto            w_ctre = ctre::search<R"(\w+)">;
    auto            input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);
    decltype(input) trimmedInput(input.begin() + 1, input.end());

    // DATA PREP
    std::unordered_map<
        std::string,
        std::variant<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr>,
        incom::commons::XXH3Hasher>
        instrTypeMap;

    auto mapCreatorIteration = [&](auto &&oneVariant) {
        instrTypeMap.emplace(incom::commons::PQA::TypeToString<std::remove_cvref_t<decltype(oneVariant)>>(),
                             std::variant<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr,
                                          eqir, eqri, eqrr>{oneVariant});
    };

    long long fakeLong;
    incom::commons::argPackHLPR<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri,
                             eqrr>::invokeOverConstructedPack(mapCreatorIteration, fakeLong, fakeLong, fakeLong);

    incom::commons::PQA::ProgramQuasiAssembly_2018<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri,
                                                gtrr, eqir, eqri, eqrr>
        pqa_2018(trimmedInput, instrTypeMap, regStartVal);

    // LAMBDA DEFINITIONS
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

    // MAIN LOGIC
    long long const instrCursor            = std::stoll(input.front().back());
    long long       instructionID_external = 0;

    while (instructionID_external >= 0 and instructionID_external < pqa_2018.instrVect.size()) {
        pqa_2018.registers[instrCursor] = instructionID_external;
        std::visit(overloadSet, pqa_2018.instrVect[instructionID_external]);
        instructionID_external = pqa_2018.registers[instrCursor] + 1;
    }
    return pqa_2018.registers[0];
}

long long day19_2(std::string dataFile, const std::vector<long long> regStartVal) {
    // TYPE DEFINITIONS
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

    // INPUT PROCESSING
    auto            w_ctre = ctre::search<R"(\w+)">;
    auto            input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);
    decltype(input) trimmedInput(input.begin() + 1, input.end());

    // DATA PREP
    std::unordered_map<
        std::string,
        std::variant<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr>,
        incom::commons::XXH3Hasher>
        instrTypeMap;

    auto mapCreatorIteration = [&](auto &&oneVariant) {
        instrTypeMap.emplace(incom::commons::PQA::TypeToString<std::remove_cvref_t<decltype(oneVariant)>>(),
                             std::variant<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr,
                                          eqir, eqri, eqrr>{oneVariant});
    };

    long long fakeLong;
    incom::commons::argPackHLPR<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri,
                             eqrr>::invokeOverConstructedPack(mapCreatorIteration, fakeLong, fakeLong, fakeLong);

    incom::commons::PQA::ProgramQuasiAssembly_2018<addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri,
                                                gtrr, eqir, eqri, eqrr>
        pqa_2018(trimmedInput, instrTypeMap, regStartVal);

    // LAMBDA DEFINITIONS
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

    // MAIN LOGIC
    long long const instrCursor            = std::stoll(input.front().back());
    long long       instructionID_external = 0;

    long long iterationCounter = -1;

    while (instructionID_external >= 0 and instructionID_external < pqa_2018.instrVect.size() and
           iterationCounter < 200) {
        pqa_2018.registers[instrCursor] = instructionID_external;
        std::visit(overloadSet, pqa_2018.instrVect[instructionID_external]);
        instructionID_external = pqa_2018.registers[instrCursor] + 1;
        iterationCounter++;
    }

    long long const sumOfDivisorsFor = pqa_2018.registers[1];
    long long       sumOfDivisors    = 0;

    for (int i = 1; i <= sumOfDivisorsFor; ++i) {
        if (sumOfDivisorsFor % i == 0) { sumOfDivisors += i; }
    }
    return sumOfDivisors;
}
} // namespace AOC2018