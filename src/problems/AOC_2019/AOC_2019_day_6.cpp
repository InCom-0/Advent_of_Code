
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <optional>
#include <queue>


namespace AOC2019 {
long long day6_1(std::string dataFile) {

    struct SpaceObj {
        std::string                                   name;
        std::vector<std::reference_wrapper<SpaceObj>> previous;
        std::optional<long long>                      indirOrbits = std::nullopt;

        long long get_myOrbits() {
            indirOrbits = indirOrbits.or_else([&]() -> std::optional<long long> {
                return std::ranges::fold_left(previous, 0,
                                              [](auto &&init, auto &&a) { return init + a.get().get_myOrbits(); });
            });
            return previous.size() + indirOrbits.value();
        }
    };

    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::segmented_map<std::string, SpaceObj, incom::commons::XXH3Hasher> so_map;

    for (auto &line : input) {
        for (auto &strInLine : line) { so_map.emplace(strInLine, SpaceObj{strInLine}); }
    }

    for (auto &line : input) { so_map.at(line.back()).previous.push_back(so_map.at(line.front())); }

    long long accu = 0;
    for (auto &[_, so] : so_map) { accu += so.get_myOrbits(); }

    return accu;
}

long long day6_2(std::string dataFile) {
    struct SpaceObj {
        std::vector<std::reference_wrapper<SpaceObj>> previous;
        std::vector<std::reference_wrapper<SpaceObj>> next;
    };

    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::segmented_map<std::string, SpaceObj, incom::commons::XXH3Hasher> so_map;

    for (auto &line : input) {
        for (auto &strInLine : line) { so_map.emplace(strInLine, SpaceObj{}); }
    }

    for (auto &line : input) {
        so_map.at(line.back()).previous.push_back(so_map.at(line.front()));
        so_map.at(line.front()).next.push_back(so_map.at(line.back()));
    }

    SpaceObj &you = so_map.at("YOU");
    SpaceObj &san = so_map.at("SAN");

    std::queue<std::tuple<std::reference_wrapper<SpaceObj>, int, std::reference_wrapper<SpaceObj>>> qu;
    qu.push({you, 0, you});

    int  res = 0;
    auto exe = [&]() {
        while (not qu.empty()) {
            if (&(std::get<0>(qu.front()).get()) == (&san)) { return std::get<1>(qu.front()) - 2; }

            for (auto &obPrev : std::get<0>(qu.front()).get().previous) {
                if (&(std::get<2>(qu.front()).get()) == &(obPrev.get())) { continue; }
                qu.push({obPrev.get(), std::get<1>(qu.front()) + 1, std::get<0>(qu.front()).get()});
            }
            for (auto &nextPrev : std::get<0>(qu.front()).get().next) {
                if (&(std::get<2>(qu.front()).get()) == &(nextPrev.get())) { continue; }
                qu.push({nextPrev.get(), std::get<1>(qu.front()) + 1, std::get<0>(qu.front()).get()});
            }
            qu.pop();
        }
        std::unreachable();
    };

    return exe();
}
} // namespace AOC2019