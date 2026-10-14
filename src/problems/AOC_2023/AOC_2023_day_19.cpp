#include <algorithm>
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
            [&](const Redirect &a) -> std::optional<bool> { return a.getTarget(rules).executeRuleLine(rules, p); },

            [&](const Reject &a) -> std::optional<bool> { return false; },
            [&](const Accept &a) -> std::optional<bool> { return true; },
        };

        if (p.get_val(r) < rightSide) { return std::visit(ol_obj, this->whenTrue); }
        else { return std::nullopt; }
    }
};

struct InstGreater : InstrBase {
    std::optional<bool>
    execute(auto const &rules, OnePart const &p) const {
        auto ol_obj = incstd::variant_utils::Overloads{
            [&](const Redirect &a) -> std::optional<bool> { return a.getTarget(rules).executeRuleLine(rules, p); },

            [&](const Reject &a) -> std::optional<bool> { return false; },
            [&](const Accept &a) -> std::optional<bool> { return true; },
        };

        if (p.get_val(r) > rightSide) { return std::visit(ol_obj, this->whenTrue); }
        else { return std::nullopt; }
    }
};


struct RuleLine {
    std::string                                                                name;
    std::vector<std::variant<InstLess, InstGreater, Redirect, Reject, Accept>> instrs;

    bool
    executeRuleLine(ankerl::unordered_dense::map<std::string, RuleLine, incstd::hashing::XXH3Hasher> const &rules,
                    OnePart const &part) const {
        auto ol_obj = incstd::variant_utils::Overloads{
            [&](const InstLess &a) -> std::optional<bool> { return a.execute(rules, part); },
            [&](const InstGreater &a) -> std::optional<bool> { return a.execute(rules, part); },
            [&](const Redirect &a) -> std::optional<bool> { return a.getTarget(rules).executeRuleLine(rules, part); },

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
                            InstLess{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Redirect{quad[3]}});
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
                            InstGreater{get_registerFrom(quad[0].front()), std::stoll(quad[2]), Redirect{quad[3]}});
                    }
                }
                else { assert(false); }
            }
            else if (quad.size() == 1) {
                if (quad[0] == "A") { rl.instrs.push_back(Accept{}); }
                else if (quad[0] == "R") { rl.instrs.push_back(Reject{}); }
                else { rl.instrs.push_back(Redirect{quad[0]}); }
            }
            else { assert(false); }
        }
    }


    return pr_res;
}
size_t
day19_1(std::string dataFile) {

    auto const [parts, rules] = day19_0(dataFile);


    auto const &beg = rules.at("in");

    size_t res{};
    for (auto const &onePart : parts) {
        if (beg.executeRuleLine(rules, onePart)) { res += std::ranges::fold_left(onePart.vals, 0ll, std::plus{}); }
    }

    return res;
}
size_t
day19_2(std::string dataFile) {
    return 0uz;
}

} // namespace AOC2023