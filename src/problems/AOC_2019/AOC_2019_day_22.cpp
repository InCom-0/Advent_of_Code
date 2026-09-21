#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/core.h>
#include <incom_commons.h>
#include <iostream>
#include <ranges>
#include <variant>


namespace AOC2019 {


struct DIS {};
struct Cut {
    long long m_offset;
};
struct DWI {
    long long m_offset;
};
typedef __int128 int128_t;


using Vari = std::variant<DIS, Cut, DWI>;
long long
day22_0(std::string dataFile, int128_t numOfCards, size_t cardToTrack, size_t shuffleReps = 1,
        bool reverseInstr = false) {

    auto d_ctre = ctre::search<R"(-?\w+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<Vari> seq_techniques;

    for (auto const &line : input) {
        if (line[1] == "into") { seq_techniques.push_back(Vari(DIS())); }

        else if (line[0] == "cut") { seq_techniques.push_back(Vari(Cut(std::stoll(line[1])))); }
        else if (line[1] == "with") { seq_techniques.push_back(Vari(DWI(std::stoll(line[3])))); }
    }

    int128_t cardCurPos = cardToTrack;

    auto exe_technique = [&](size_t const &cursor) {
        auto const OL_set = incstd::variant_utils::Overloads{
            [&](DIS const &a) { cardCurPos = (numOfCards - cardCurPos - 1); },
            [&](Cut const &a) {
                size_t loc_offset = a.m_offset >= 0 ? a.m_offset : (numOfCards + a.m_offset);
                if (loc_offset <= cardCurPos) { cardCurPos -= loc_offset; }
                else { cardCurPos = numOfCards - (loc_offset - cardCurPos); }
            },
            [&](DWI const &a) { cardCurPos = (a.m_offset * cardCurPos) % numOfCards; }};
        std::visit(OL_set, seq_techniques.at(cursor));
    };

    for (size_t rep = 0; rep < shuffleReps; ++rep) {
        for (size_t curs = 0; curs < seq_techniques.size(); ++curs) { exe_technique(curs); }
    }


    return cardCurPos;
}


long long
day22_1(std::string dataFile) {

    return day22_0(dataFile, 10'007, 2019, 1, false);
}

class LinearFunction {
public:
    int128_t
    norm(int128_t x) const {
        x %= mod;
        if (x < 0) { x += mod; }
        return x;
    }

    int128_t
    mod_mul(int128_t a, int128_t b) const {
        return norm(norm(a) * norm(b));
    }

    int128_t
    mod_add(int128_t a, int128_t b) const {
        return norm(norm(a) + norm(b));
    }

    LinearFunction
    pow(long long exponent) const {
        if (exponent == 0) { return {1, 0, mod}; }
        else if (exponent == 1) { return *this; }
        else if (exponent % 2 == 0) {
            LinearFunction half = this->pow(exponent / 2);
            return half.compose(half);
        }
        else {
            LinearFunction half = this->pow(exponent / 2);
            return this->compose(half.compose(half));
        }
    }

    int128_t
    mod_inv(int128_t n) const {
        n = norm(n);
        return mod_pow(n, mod - 2);
    }

    int128_t
    mod_pow(int128_t base, int128_t exp) const {
        base        = norm(base);
        int128_t res = 1;
        while (exp > 0) {
            if (exp & 1) { res = mod_mul(res, base); }
            base   = mod_mul(base, base);
            exp  >>= 1;
        }
        return res;
    }

    long long
    reverse_apply(long long y) const {
        // Solve for x in y = k * x + b (mod n)
        // => x = (y - b) * k_inv (mod n), where k_inv is the modular inverse of k mod n
        int128_t k_inv = mod_inv(k);
        int128_t ans   = mod_mul(mod_add(y, -b), k_inv);
        return static_cast<long long>(ans);
    }

    int128_t k;
    int128_t b;
    int128_t mod;
    LinearFunction
    compose(const LinearFunction &g) const {
        int128_t new_k = mod_mul(k, g.k);
        int128_t new_b = mod_add(mod_mul(k, g.b), b);
        return {new_k, new_b, mod};
    }
};

long long
day22_00(std::string dataFile, long long numOfCards, long long reps, long long cardToTrack) {
    auto d_ctre = ctre::search<R"(-?\w+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<Vari> seq_techniques;

    for (auto const &line : input) {
        if (line[1] == "into") { seq_techniques.push_back(Vari(DIS())); }

        else if (line[0] == "cut") { seq_techniques.push_back(Vari(Cut(std::stoll(line[1])))); }
        else if (line[1] == "with") { seq_techniques.push_back(Vari(DWI(std::stoll(line[3])))); }
    }


    LinearFunction total_function = {1, 0, numOfCards};
    auto           overload_obj   = incstd::variant_utils::Overloads{
        [&](const DIS &a) {
            LinearFunction inst = {-1, -1, numOfCards};
            total_function      = inst.compose(total_function);
        },
        [&](const Cut &a) {
            LinearFunction inst = {1, -a.m_offset, numOfCards};
            total_function      = inst.compose(total_function);
        },
        [&](const DWI &a) {
            LinearFunction inst = {a.m_offset, 0, numOfCards};
            total_function      = inst.compose(total_function);
        },
    };
    for (const auto &oneTech : seq_techniques) { std::visit(overload_obj, oneTech); }

    LinearFunction total_function_Ntimes = total_function.pow(reps);

    return total_function_Ntimes.reverse_apply(cardToTrack);
}


long long
day22_2(std::string dataFile) {
    return day22_00(dataFile, 119'315'717'514'047, 101'741'582'076'661, 2020);
}
} // namespace AOC2019