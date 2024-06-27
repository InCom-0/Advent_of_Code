#include <fstream>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <unordered_set>

#include <flux.hpp>

namespace AOC2017 {
std::string day16_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    // CUSTOM LOCAL TYPES DEFINITIONS
    struct _instrBase {
        unsigned short num_1 = 0;
        unsigned short num_2 = 0;
        char           chr_1 = '_';
        char           chr_2 = '_';
    };
    struct spn : _instrBase {};
    struct xch : _instrBase {};
    struct prt : _instrBase {};

    auto d_ctre = ctre::search<R"(\d+)">;

    // DATA PREP
    std::vector<std::variant<spn, xch, prt>> instructions;
    auto                                     instr_creater = [&](std::string &&str) -> bool {
        if (str.front() == 's') {
            instructions.push_back(spn{(unsigned short)std::stoi(d_ctre(str.begin(), str.end()).to_string())});
        }
        else if (str.front() == 'x') {
            auto ctre_res = d_ctre(str.begin(), str.end());
            instructions.push_back(
                xch{(unsigned short)std::stoi(ctre_res.to_string()),
                    (unsigned short)std::stoi(d_ctre(ctre_res.get_end_position(), str.end()).to_string())});
        }
        else if (str.front() == 'p') { instructions.push_back(prt{0, 0, str[1], str[3]}); }
        else { return false; }
        return true;
    };

    flux::ref(oneStr).chunk_by([](auto &&a, auto &&b) { return a != ','; }).for_each([&](auto &&chnk) {
        instr_creater(chnk.template to<std::string>()); // Template disambiguition ... first time seeing this.
    });

    auto        charInit = std::views::iota(97, 113); // a => ASCII 97, p => ASCII 112
    std::string progLine(charInit.begin(), charInit.end());

    // MAIN LOGIC
    for (auto &oneInstr : instructions) {
        std::visit(
            overloaded{
                [&](spn const &a) { std::rotate(progLine.rbegin(), progLine.rbegin() + a.num_1, progLine.rend()); },
                [&](xch const &a) { std::swap(progLine[a.num_1], progLine[a.num_2]); },
                [&](prt const &a) {
                    std::swap(*std::ranges::find(progLine, a.chr_1), *std::ranges::find(progLine, a.chr_2));
                },

            },
            oneInstr);
    }
    return progLine;
}

/*
This is very very inefficient solution.
Took me hours to figure out this poor solution and I am 100% convinced that this is NOT the smart way to do this :-).

It should be possible to perform the 'instructions' 'en bloc' somehow, but I don't see how.
Right now doing 1000 iterations of the instructions and then repeating the outcome that gives 1'000'000 times ... takes
like 15 secs to compute.

Smarter/faster way would probably be to do it in 'exponential' steps (ie. in 1, 2, 4, 8, etc.).
However, that seems complicated and unwieldy.

For now admiting defeat in coming up with a nice solution ... will check & possibly plagiarize other approaches online
:-(.
*/
std::string day16_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    // CUSTOM LOCAL TYPES DEFINITIONS
    struct _instrBase {
        unsigned short num_1 = 0;
        unsigned short num_2 = 0;
        char           chr_1 = '_';
        char           chr_2 = '_';
    };
    struct spn : _instrBase {};
    struct xch : _instrBase {};
    struct prt : _instrBase {};

    auto d_ctre = ctre::search<R"(\d+)">;

    // DATA PREP
    std::vector<std::variant<spn, xch, prt>> instructions;
    auto                                     instr_creater = [&](std::string &&str) -> bool {
        if (str.front() == 's') {
            instructions.push_back(spn{(unsigned short)std::stoi(d_ctre(str.begin(), str.end()).to_string())});
        }
        else if (str.front() == 'x') {
            auto ctre_res = d_ctre(str.begin(), str.end());
            instructions.push_back(
                xch{(unsigned short)std::stoi(ctre_res.to_string()),
                    (unsigned short)std::stoi(d_ctre(ctre_res.get_end_position(), str.end()).to_string())});
        }
        else if (str.front() == 'p') { instructions.push_back(prt{0, 0, str[1], str[3]}); }
        else { return false; }
        return true;
    };

    flux::ref(oneStr).chunk_by([](auto &&a, auto &&b) { return a != ','; }).for_each([&](auto &&chnk) {
        instr_creater(chnk.template to<std::string>());
    });

    auto        charInit = std::views::iota(97, 113); // a => ASCII 97, p => ASCII 112
    std::string theOne(charInit.begin(), charInit.end());

    std::vector<std::variant<prt>>      nameBasedInstr;
    std::vector<std::variant<spn, xch>> posBasedInstr;

    for (auto &oneInstr : instructions) {
        if (oneInstr.index() == 2) { nameBasedInstr.push_back(std::get<2>(oneInstr)); }
        else if (oneInstr.index() == 1) { posBasedInstr.push_back(std::get<1>(oneInstr)); }
        else if (oneInstr.index() == 0) { posBasedInstr.push_back(std::get<0>(oneInstr)); }
    }

    std::string theOtherOne = theOne;

    // MAIN LOGIC
    for (int i = 0; i < 1000; ++i) {
        for (auto &oneInstr : posBasedInstr) {
            std::visit(overloaded{
                           [&](spn const &a) {
                               std::rotate(theOtherOne.rbegin(), theOtherOne.rbegin() + a.num_1, theOtherOne.rend());
                           },
                           [&](xch const &a) { std::swap(theOtherOne[a.num_1], theOtherOne[a.num_2]); },

                       },
                       oneInstr);
        }
    }
    std::vector<unsigned short> posBasedMap;
    for (auto &oneChar : theOne) {
        posBasedMap.push_back(std::ranges::find(theOtherOne, oneChar) - theOtherOne.begin());
    }

    theOtherOne = theOne;
    for (int i = 0; i < 1000; ++i) {
        for (auto &oneInstr : nameBasedInstr) {
            std::visit(
                overloaded{
                    [&](prt const &a) {
                        std::swap(*std::ranges::find(theOtherOne, a.chr_1), *std::ranges::find(theOtherOne, a.chr_2));
                    },

                },
                oneInstr);
        }
    }
    std::unordered_map<char, char, AOC_commons::XXH3Hasher> nameBasedMap;
    for (auto &oneChar : theOne) {
        nameBasedMap.emplace(oneChar, theOne[std::ranges::find(theOtherOne, oneChar) - theOtherOne.begin()]);
    }

    theOtherOne = theOne;
    for (int i = 0; i < 1'000'000; ++i) {
        for (int j = 0; j < theOne.size(); ++j) { theOtherOne[posBasedMap[j]] = theOne[j]; }
        std::swap(theOne, theOtherOne);
    }

    for (int i = 0; i < 1'000'000; ++i) {
        std::swap(theOne, theOtherOne);
        for (int j = 0; j < theOtherOne.size(); ++j) {
            theOne[std::ranges::find(theOtherOne, nameBasedMap[theOtherOne[j]]) - theOtherOne.begin()] = theOtherOne[j];
        }
    }
    return theOne;
}

/*
Much better solution as compared to day16_2.

Basically finding a cycle first and then computing just the remainder of necessary steps.
Wouldn't work all that well if there were significantly more than 16 'programs' 'dancing' ... but oh well.
*/
std::string day16_3(std::string dataFile, unsigned int numOfDances) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;
    std::getline(iStream, oneStr);

    // CUSTOM LOCAL TYPES DEFINITIONS
    struct _instrBase {
        unsigned short num_1 = 0;
        unsigned short num_2 = 0;
        char           chr_1 = '_';
        char           chr_2 = '_';
    };
    struct spn : _instrBase {};
    struct xch : _instrBase {};
    struct prt : _instrBase {};

    auto d_ctre = ctre::search<R"(\d+)">;

    // DATA PREP
    std::vector<std::variant<spn, xch, prt>> instructions;
    auto                                     instr_creater = [&](std::string &&str) -> bool {
        if (str.front() == 's') {
            instructions.push_back(spn{(unsigned short)std::stoi(d_ctre(str.begin(), str.end()).to_string())});
        }
        else if (str.front() == 'x') {
            auto ctre_res = d_ctre(str.begin(), str.end());
            instructions.push_back(
                xch{(unsigned short)std::stoi(ctre_res.to_string()),
                    (unsigned short)std::stoi(d_ctre(ctre_res.get_end_position(), str.end()).to_string())});
        }
        else if (str.front() == 'p') { instructions.push_back(prt{0, 0, str[1], str[3]}); }
        else { return false; }
        return true;
    };

    flux::ref(oneStr).chunk_by([](auto &&a, auto &&b) { return a != ','; }).for_each([&](auto &&chnk) {
        instr_creater(chnk.template to<std::string>());
    });

    auto        charInit = std::views::iota(97, 113); // a => ASCII 97, p => ASCII 112
    std::string theOne(charInit.begin(), charInit.end());

    auto overload2Visit = overloaded{
        [&](spn const &a) { std::rotate(theOne.rbegin(), theOne.rbegin() + a.num_1, theOne.rend()); },
        [&](xch const &a) { std::swap(theOne[a.num_1], theOne[a.num_2]); },
        [&](prt const &a) { std::swap(*std::ranges::find(theOne, a.chr_1), *std::ranges::find(theOne, a.chr_2)); },
    };

    std::unordered_set<std::string, AOC_commons::XXH3Hasher> pastResults;

    // MAIN LOGIC
    // Find a cycle (that is after X repeats end up where we started)
    while (not pastResults.contains(theOne)) {
        pastResults.emplace(theOne);
        for (auto &oneInstr : instructions) { std::visit(overload2Visit, oneInstr); }
    }

    // Computed the 'remainder' number of dances.
    for (int i = 0; i < (numOfDances % pastResults.size()); ++i) {
        for (auto &oneInstr : instructions) { std::visit(overload2Visit, oneInstr); }
    }
    return theOne;
}

} // namespace AOC2017