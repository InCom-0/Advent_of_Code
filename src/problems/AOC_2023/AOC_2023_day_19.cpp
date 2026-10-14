#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/hashing.hpp>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <variant>


namespace AOC2023 {

enum class Register : size_t {
    x = 0uz,
    m,
    a,
    s,
};

struct OnePart {
    std::array<long long, 4uz> vals;

    long long const &
    get_val(Register const &r) const {
        static long long fakeLL = std::numeric_limits<long long>::min();
        using enum Register;
        switch (r) {
            case x:  return vals[0]; break;
            case m:  return vals[1]; break;
            case a:  return vals[2]; break;
            case s:  return vals[3]; break;
            default: return fakeLL; break;
        }
        std::unreachable();
    }
};

struct Redirect {
    std::string const target;

    auto &
    getTarget(auto const &rules) const {
        return rules.at(target);
    }
};

struct Accept {};
struct Reject {};

struct InstrBase {
    Register const  r;
    long long const rightSide;

    std::variant<Redirect, Reject, Accept> whenTrue;

    bool
    execute(OnePart const &p) const = delete;
};


struct InstLess : InstrBase {
    std::optional<bool>
    execute(auto const &rules, OnePart const &p) const {
        auto ol_obj = incstd::variant_utils::Overloads{
            [&](const Redirect &a) -> std::optional<bool> { return a.getTarget(rules).executeRule(rules, p); },

            [&](const Reject &a) -> std::optional<bool> { return false; },
            [&](const Accept &a) -> std::optional<bool> { return true; },
        };

        if (p.get_val(r) < rightSide) { return std::visit(ol_obj, this->whenTrue); }
        else { return std::nullopt; }
    }
};

struct InstGreater : InstrBase {
    bool
    execute(auto &rules, OnePart const &p) const {
        return p.get_val(r) > rightSide;
    }
};


struct RuleLine {
    std::string                                                                name;
    std::vector<std::variant<InstLess, InstGreater, Redirect, Reject, Accept>> instrs;

    bool
    executeRule(ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
                OnePart const                                                                          &part) const {
        auto ol_obj = incstd::variant_utils::Overloads{
            [&](const InstLess &a) -> std::optional<bool> { return a.execute(rules, part); },
            [&](const InstGreater &a) -> std::optional<bool> { return a.execute(rules, part); },
            [&](const Redirect &a) -> std::optional<bool> { return false; },

            [&](const Reject &a) -> std::optional<bool> { return false; },
            [&](const Accept &a) -> std::optional<bool> { return true; },
        };

        for (auto const &instr : instrs) {
            if (auto ret = std::visit(ol_obj, instr)) { return ret.value(); }
        }
        return false;
    }
};


auto
day19_0(std::string dataFile) {

    ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> rls;

    rls.at("in").executeRule(rls, OnePart{});
    return;
}
size_t
day19_1(std::string dataFile) {
    return 0uz;
}
size_t
day19_2(std::string dataFile) {
    return 0uz;
}

} // namespace AOC2023