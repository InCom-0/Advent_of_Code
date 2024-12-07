#include <ankerl/unordered_dense.h>
#include <bits/ranges_algo.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
long long day1_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre, d_ctre);

    std::vector<long long> listA;
    std::vector<long long> listB;

    for (int i = 0; i < input.front().size(); ++i) {
        listA.push_back(std::stoll(input[0][i]));
        listB.push_back(std::stoll(input[1][i]));
    }

    std::ranges::sort(listA, std::less());
    std::ranges::sort(listB, std::less());

    auto zt  = std::views::zip_transform([](auto &a, auto &b) { return std::abs(a - b); }, listA, listB);
    auto res = std::ranges::fold_left_first(zt, std::plus());


    return res.value_or(LLONG_MIN);
}

long long day1_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre, d_ctre);

    std::vector<long long> listA;
    std::vector<long long> listB;

    for (int i = 0; i < input.front().size(); ++i) {
        listA.push_back(std::stoll(input[0][i]));
        listB.push_back(std::stoll(input[1][i]));
    }

    ankerl::unordered_dense::map<long long, std::pair<long long, long long>, incom::commons::XXH3Hasher> mp;
    for (auto const &numA : listA) {
        if (mp.contains(numA)) { mp.at(numA).first++; }
        else { mp.insert({numA, {1, 0}}); }
    }
    for (auto const &numB : listB) {
        if (mp.contains(numB)) { mp.at(numB).second++; }
    }

    auto res = std::ranges::fold_left(
        mp, 0ll, [](auto &&ini, auto &&item) { return ini + (item.first * item.second.first * item.second.second); });

    return res;
}
} // namespace AOC2024