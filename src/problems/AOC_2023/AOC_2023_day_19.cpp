#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <array>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/hashing.hpp>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>


namespace AOC2023 {
namespace {
enum class Register : size_t {
    x = 0uz,
    m,
    a,
    s,
    _ERR
};

Register
get_registerFrom(char const &c) {
    using enum Register;
    switch (c) {
        case 'x': return x; break;
        case 'm': return m; break;
        case 'a': return a; break;
        case 's': return s; break;
        default:  return _ERR; break;
    }
}

struct OnePart {
    std::array<long long, 4uz> vals;

    long long const &
    get_val(Register const &r) const {
        static long long fakeLL = std::numeric_limits<long long>::min();
        using enum Register;
        switch (r) {
            case x:    return vals[0]; break;
            case m:    return vals[1]; break;
            case a:    return vals[2]; break;
            case s:    return vals[3]; break;
            case _ERR: return fakeLL; break;
            default:   return fakeLL; break;
        }
        std::unreachable();
    }
};

struct InstrBase {
    Register const  r{};
    long long const rightSide{};
};

struct Redirect : InstrBase {
    std::string const target;

    auto &
    getTarget(auto const &rules) const {
        return rules.at(target);
    }
};
struct Accept : InstrBase {};
struct Reject : InstrBase {};

struct InstLess : InstrBase {
    std::variant<Redirect, Reject, Accept> whenTrue;
};
struct InstGreater : InstrBase {
    std::variant<Redirect, Reject, Accept> whenTrue;
};

struct RuleLine {
    std::string                                                                name;
    std::vector<std::variant<InstLess, InstGreater, Redirect, Reject, Accept>> instrs;
};

bool
executeRuleLine(RuleLine const                                                                         &rl,
                ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
                OnePart const                                                                          &part);

std::optional<bool>
execute(InstLess const                                                                         &instr,
        ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
        OnePart const                                                                          &p) {
    auto ol_obj = incstd::variant_utils::Overloads{
        [&](const Redirect &a) -> std::optional<bool> { return executeRuleLine(a.getTarget(rules), rules, p); },

        [&](const Reject &a) -> std::optional<bool> { return false; },
        [&](const Accept &a) -> std::optional<bool> { return true; },
    };

    if (p.get_val(instr.r) < instr.rightSide) { return std::visit(ol_obj, instr.whenTrue); }
    else { return std::nullopt; }
}

std::optional<bool>
execute(InstGreater const                                                                      &instr,
        ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
        OnePart const                                                                          &p) {
    auto ol_obj = incstd::variant_utils::Overloads{
        [&](const Redirect &a) -> std::optional<bool> { return executeRuleLine(a.getTarget(rules), rules, p); },

        [&](const Reject &a) -> std::optional<bool> { return false; },
        [&](const Accept &a) -> std::optional<bool> { return true; },
    };

    if (p.get_val(instr.r) > instr.rightSide) { return std::visit(ol_obj, instr.whenTrue); }
    else { return std::nullopt; }
}

bool
executeRuleLine(RuleLine const                                                                         &rl,
                ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
                OnePart const                                                                          &part) {
    auto ol_obj = incstd::variant_utils::Overloads{
        [&](const InstLess &a) -> std::optional<bool> { return execute(a, rules, part); },
        [&](const InstGreater &a) -> std::optional<bool> { return execute(a, rules, part); },
        [&](const Redirect &a) -> std::optional<bool> { return executeRuleLine(a.getTarget(rules), rules, part); },

        [&](const Reject &a) -> std::optional<bool> { return false; },
        [&](const Accept &a) -> std::optional<bool> { return true; },
    };

    for (auto const &instr : rl.instrs) {
        if (auto ret = std::visit(ol_obj, instr)) { return ret.value(); }
    }
    return false;
}

// ###############################
// ###############################
// ###############################

struct ValidRng {
    long long Min = 1;
    long long sz  = 4000ll;
};

struct Tracker {
    std::array<ValidRng, 4uz> valid_rngs;
    bool                      isValid = true;

    ValidRng &
    get_vr(Register const &r) {
        static ValidRng fakeVR{};
        using enum Register;
        switch (r) {
            case x:    return valid_rngs[0]; break;
            case m:    return valid_rngs[1]; break;
            case a:    return valid_rngs[2]; break;
            case s:    return valid_rngs[3]; break;
            case _ERR: return fakeVR; break;
            default:   return fakeVR; break;
        }
        std::unreachable();
    }


    bool
    update_vr_max(Register const &r, long long const &newMax) {
        ValidRng &vr = get_vr(r);
        if (newMax <= vr.Min) { isValid = false; }
        else if (newMax < (vr.Min + vr.sz)) {
            vr.sz   = newMax - vr.Min;
            isValid = true;
        }
        else { isValid = true; }
        return isValid;
    }
    bool
    update_vr_min(Register const &r, long long const &newMin) {
        ValidRng &vr = get_vr(r);
        if (newMin >= (vr.Min + vr.sz)) { isValid = false; }
        else if (newMin <= vr.Min) { isValid = true; }
        else {
            vr.sz   -= (newMin - vr.Min);
            vr.Min   = newMin;
            isValid  = true;
        }
        return isValid;
    }

    bool
    update_vr_max_rev(Register const &r, long long const &newMaxRev) {
        ValidRng &vr = get_vr(r);
        if (newMaxRev <= vr.Min) { isValid = true; }
        else if (newMaxRev < (vr.Min + vr.sz)) {
            vr.sz   = (vr.Min + vr.sz) - newMaxRev;
            vr.Min  = newMaxRev;
            isValid = true;
        }
        else { isValid = false; }
        return isValid;
    }

    bool
    update_vr_min_rev(Register const &r, long long const &newMinRev) {
        ValidRng &vr = get_vr(r);
        if (newMinRev >= (vr.Min + vr.sz)) { isValid = true; }
        else if (newMinRev <= vr.Min) { isValid = false; }
        else {
            vr.sz   = newMinRev - vr.Min;
            isValid = true;
        }
        return isValid;
    }
};


long long
executeRuleLine_p2(RuleLine const                                                                         &rl,
                   ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
                   Tracker                                                                                &t);

long long
execute_p2(InstLess const                                                                         &instr,
           ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules, Tracker &t) {
    Tracker t2     = t;
    auto    ol_obj = incstd::variant_utils::Overloads{
        [&](const Redirect &a) -> long long { return executeRuleLine_p2(a.getTarget(rules), rules, t2); },

        [&](const Reject &a) -> long long { return 0ll; },
        [&](const Accept &a) -> long long {
            return std::ranges::fold_left(t2.valid_rngs, 1ll, [](auto init, auto val) { return init * val.sz; });
        },
    };
    long long accu{};
    if (t2.update_vr_max(instr.r, instr.rightSide)) { accu += std::visit(ol_obj, instr.whenTrue); }
    t.update_vr_max_rev(instr.r, instr.rightSide);
    return accu;
}

long long
execute_p2(InstGreater const                                                                      &instr,
           ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules, Tracker &t) {
    Tracker t2     = t;
    auto    ol_obj = incstd::variant_utils::Overloads{
        [&](const Redirect &a) -> long long { return executeRuleLine_p2(a.getTarget(rules), rules, t2); },

        [&](const Reject &a) -> long long { return 0ll; },
        [&](const Accept &a) -> long long {
            return std::ranges::fold_left(t2.valid_rngs, 1ll, [](auto init, auto val) { return init * val.sz; });
        },
    };
    long long accu{};
    if (t2.update_vr_min(instr.r, instr.rightSide + 1)) { accu += std::visit(ol_obj, instr.whenTrue); }
    t.update_vr_min_rev(instr.r, instr.rightSide + 1);

    return accu;
}

long long
executeRuleLine_p2(RuleLine const                                                                         &rl,
                   ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
                   Tracker                                                                                &t) {
    auto ol_obj = incstd::variant_utils::Overloads{
        [&](const InstLess &a) -> long long { return execute_p2(a, rules, t); },
        [&](const InstGreater &a) -> long long { return execute_p2(a, rules, t); },
        [&](const Redirect &a) -> long long {
            Tracker t2 = t;
            t.isValid  = false;
            return executeRuleLine_p2(a.getTarget(rules), rules, t2);
        },

        [&](const Reject &a) -> long long {
            t.isValid = false;
            return 0ll;
        },
        [&](const Accept &a) -> long long {
            t.isValid = false;
            return std::ranges::fold_left(t.valid_rngs, 1ll, [](auto init, auto val) { return init * val.sz; });
        },
    };

    long long accu{};
    for (auto const &instr : rl.instrs) {
        accu += std::visit(ol_obj, instr);
        if (not t.isValid) { break; }
    }
    return accu;
}
} // namespace

auto
day19_0(std::string dataFile) {
    auto w_ctre      = ctre::search<R"(\w+)">;
    auto anyOne_ctre = ctre::search<R"(<|>)">;
    auto d_ctre      = ctre::search<R"(\d+)">;


    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }

    std::vector<std::pair<std::string, std::vector<std::string>>> tmpRes;

    std::pair<std::vector<OnePart>, ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher>>
        pr_res;

    auto &[parts, res] = pr_res;

    std::string oneStr;
    while (std::getline(iStream, oneStr)) {
        if (oneStr.empty()) { continue; }
        else if (oneStr.front() == '{') {
            auto tmpVals = incom::aoc::parseInputUsingCTRE::processOneLineRPToneVect(oneStr, d_ctre);
            if (tmpVals.size() != 4) { assert(false); }
            parts.push_back(OnePart{
                .vals{std::stoll(tmpVals[0]), std::stoll(tmpVals[1]), std::stoll(tmpVals[2]), std::stoll(tmpVals[3])}});
        }
        else {
            if (oneStr == "") {
                tmpRes.emplace_back();
                continue;
            }
            tmpRes.emplace_back();
            auto r              = w_ctre(oneStr.begin(), oneStr.end());
            tmpRes.back().first = r.to_string();

            std::string rest = std::string(r.get_end_position(), oneStr.end());
            tmpRes.back().second =
                incom::aoc::parseInputUsingCTRE::processOneLineRPToneVect(rest, w_ctre, anyOne_ctre, d_ctre, w_ctre);
        }
    };

    for (auto const &[name, partitioned] : tmpRes) {
        auto it = res.insert({name, RuleLine{}});
        if (not it.second) { assert(false); }
        RuleLine &rl = it.first->second;
        rl.name      = name;

        for (auto quad : partitioned | std::views::chunk(4)) {
            using enum Register;

            if (quad.size() == 4) {
                if (quad[1].front() == '<') {
                    if (quad[3] == "A") {
                        rl.instrs.push_back(InstLess{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Accept{}});
                    }
                    else if (quad[3] == "R") {
                        rl.instrs.push_back(InstLess{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Reject{}});
                    }
                    else {
                        rl.instrs.push_back(
                            InstLess{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Redirect{{}, quad[3]}});
                    }
                }
                else if (quad[1].front() == '>') {
                    if (quad[3] == "A") {
                        rl.instrs.push_back(
                            InstGreater{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Accept{}});
                    }
                    else if (quad[3] == "R") {
                        rl.instrs.push_back(
                            InstGreater{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Reject{}});
                    }
                    else {
                        rl.instrs.push_back(
                            InstGreater{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Redirect{{}, quad[3]}});
                    }
                }
                else { assert(false); }
            }
            else if (quad.size() == 1) {
                if (quad[0] == "A") { rl.instrs.push_back(Accept{}); }
                else if (quad[0] == "R") { rl.instrs.push_back(Reject{}); }
                else { rl.instrs.push_back(Redirect{{}, quad[0]}); }
            }
            else { assert(false); }
        }
    }


    return pr_res;
}
size_t
day19_1(std::string dataFile) {

    auto const [parts, rules] = day19_0(dataFile);
    auto const &beg           = rules.at("in");

    size_t res{};
    for (auto const &onePart : parts) {
        if (executeRuleLine(beg, rules, onePart)) { res += std::ranges::fold_left(onePart.vals, 0ll, std::plus{}); }
    }

    return res;
}
size_t
day19_2(std::string dataFile) {
    auto const [_, rules] = day19_0(dataFile);
    auto const &beg       = rules.at("in");

    Tracker t{};

    auto res = executeRuleLine_p2(beg, rules, t);


    return static_cast<size_t>(res);
}

} // namespace AOC2023