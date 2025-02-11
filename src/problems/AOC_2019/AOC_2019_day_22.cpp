#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/core.h>
#include <incom_commons.h>
#include <ranges>
#include <variant>


namespace AOC2019 {
namespace incc = incom::commons;

struct DIS {};
struct Cut {
    long long m_offset;
};
struct DWI {
    long long m_offset;
};
typedef __int128 int128_t;


using Vari = std::variant<DIS, Cut, DWI>;
long long day22_0(std::string dataFile, int128_t numOfCards, size_t cardToTrack, size_t shuffleReps = 1,
                  bool reverseInstr = false) {

    auto d_ctre = ctre::search<R"(-?\w+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<Vari> seq_techniques;

    for (auto const &line : input) {
        if (line[1] == "into") { seq_techniques.push_back(Vari(DIS())); }

        else if (line[0] == "cut") { seq_techniques.push_back(Vari(Cut(std::stoll(line[1])))); }
        else if (line[1] == "with") { seq_techniques.push_back(Vari(DWI(std::stoll(line[3])))); }
    }

    int128_t cardCurPos = cardToTrack;


    if (reverseInstr) {
        auto fv = std::views::filter(seq_techniques, [](auto &&a) { return a.index() == 1; });
        for (auto &cut_item : fv) { std::get<1>(cut_item).m_offset *= -1; }

        std::ranges::reverse(seq_techniques);

        auto exe_technique_rev = [&](size_t const &cursor) {
            auto const OL_set = overloaded{[&](DIS const &a) { cardCurPos = (numOfCards - cardCurPos - 1); },
                                           [&](Cut const &a) {
                                               size_t loc_offset =
                                                   a.m_offset >= 0 ? a.m_offset : (numOfCards + a.m_offset);
                                               if (loc_offset <= cardCurPos) { cardCurPos -= loc_offset; }
                                               else { cardCurPos = numOfCards - (loc_offset - cardCurPos); }
                                           },
                                           [&](DWI const &a) {
                                               size_t cycleCount = 0;
                                               while ((cardCurPos + (numOfCards * cycleCount)) % a.m_offset != 0) {
                                                   cycleCount++;
                                               }
                                               cardCurPos = (cardCurPos + (numOfCards * cycleCount)) / a.m_offset;
                                           }};
            std::visit(OL_set, seq_techniques.at(cursor));
        };

        ankerl::unordered_dense::set<int128_t, incc::XXH3Hasher> st;
        st.insert(cardCurPos);
        for (size_t rep = 0; rep < shuffleReps; ++rep) {
            for (size_t curs = 0; curs < seq_techniques.size(); ++curs) { exe_technique_rev(curs); }
            if (not st.insert(cardCurPos).second) {
                 return -888; }
        }
    }

    else {
        auto exe_technique = [&](size_t const &cursor) {
            auto const OL_set = overloaded{[&](DIS const &a) { cardCurPos = (numOfCards - cardCurPos - 1); },
                                           [&](Cut const &a) {
                                               size_t loc_offset =
                                                   a.m_offset >= 0 ? a.m_offset : (numOfCards + a.m_offset);
                                               if (loc_offset <= cardCurPos) { cardCurPos -= loc_offset; }
                                               else { cardCurPos = numOfCards - (loc_offset - cardCurPos); }
                                           },
                                           [&](DWI const &a) { cardCurPos = (a.m_offset * cardCurPos) % numOfCards; }};
            std::visit(OL_set, seq_techniques.at(cursor));
        };

        for (size_t rep = 0; rep < shuffleReps; ++rep) {
            for (size_t curs = 0; curs < seq_techniques.size(); ++curs) { exe_technique(curs); }
        }
    }

    return cardCurPos;
}
long long day22_1(std::string dataFile) {

    return day22_0(dataFile, 10'007, 2019, 1, false);
}


long long day22_2(std::string dataFile) {
    return day22_0(dataFile, 119315717514047, 2020, 101741582076661, true);
    // return day22_0(dataFile, 10'007, 6129, 1, true);
}
} // namespace AOC2019