#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cmath>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.h>
#include <incstd/core/hashing.hpp>
#include <limits>
#include <ranges>
#include <string>

#include <incstd/incstd_core.hpp>
#include <utility>


namespace AOC2025 {
size_t
day8_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::pair<size_t, std::vector<long long>>> data{};
    for (size_t id = 0; auto const &line : input) {
        data.push_back(
            {id++, std::vector<long long>{std::stoll(line.at(0)), std::stoll(line.at(1)), std::stoll(line.at(2))}});
    }

    auto compute3dDistance = [](std::span<long long, 3> pointA, std::span<long long, 3> pointB) {
        return std::sqrt(std::pow(pointA[0] - pointB[0], 2) + std::pow(pointA[1] - pointB[1], 2) +
                         (std::pow(pointA[2] - pointB[2], 2)));
    };

    struct Dist {
        size_t pointA_id;
        size_t pointB_id;
        double dist;
    };

    std::vector<Dist> pairDists;

    for (auto const &[pA, pB] : incom::standard::views::combinations_k<2>(data)) {
        pairDists.push_back(
            Dist{pA.first, pB.first,
                 compute3dDistance(std::span<long long, 3>(pA.second), std::span<long long, 3>(pB.second))});
    }
    std::ranges::sort(pairDists, [](auto const &a, auto const &b) { return a.dist < b.dist; });


    std::vector<std::vector<size_t>> circuits{};
    for (auto const &oneDist : std::views::take(pairDists, 1000)) {
        std::optional<size_t> foundA_at;
        for (size_t id = 0; auto &oneCir : circuits) {
            auto iteA = std::ranges::find(oneCir, oneDist.pointA_id);
            if (iteA != oneCir.end()) {
                foundA_at = id;
                break;
            }
            id++;
        }
        std::optional<size_t> foundB_at;
        for (size_t id = 0; auto &oneCir : circuits) {
            auto iteB = std::ranges::find(oneCir, oneDist.pointB_id);
            if (iteB != oneCir.end()) {
                foundB_at = id;
                break;
            }
            id++;
        }

        if (not foundA_at.has_value()) {
            if (not foundB_at.has_value()) { circuits.push_back({oneDist.pointA_id, oneDist.pointB_id}); }
            else { circuits.at(foundB_at.value()).push_back(oneDist.pointA_id); }
        }
        else if (not foundB_at.has_value()) { circuits.at(foundA_at.value()).push_back(oneDist.pointB_id); }
        // Both are found
        else {
            if (foundA_at.value() != foundB_at.value()) {
                for (auto const &oneBnum : circuits.at(foundB_at.value())) {
                    circuits.at(foundA_at.value()).push_back(oneBnum);
                }
                circuits.erase(circuits.begin() + foundB_at.value());
            }
        }
    }
    std::ranges::sort(circuits, [](auto const &cirA, auto const &cirB) { return cirA.size() > cirB.size(); });


    auto res = std::views::take(circuits, 3) | std::views::transform([](auto const &cir) { return cir.size(); });

    return std::ranges::fold_left_first(res, std::multiplies{}).value_or(0);
}
size_t
day8_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<std::pair<size_t, std::vector<long long>>> data{};
    for (size_t id = 0; auto const &line : input) {
        data.push_back(
            {id++, std::vector<long long>{std::stoll(line.at(0)), std::stoll(line.at(1)), std::stoll(line.at(2))}});
    }

    auto compute3dDistance = [](std::span<long long, 3> pointA, std::span<long long, 3> pointB) {
        return std::sqrt(std::pow(pointA[0] - pointB[0], 2) + std::pow(pointA[1] - pointB[1], 2) +
                         (std::pow(pointA[2] - pointB[2], 2)));
    };

    struct Dist {
        size_t pointA_id;
        size_t pointB_id;
        double dist;
    };

    std::vector<Dist> pairDists;

    for (auto const &[pA, pB] : incom::standard::views::combinations_k<2>(data)) {
        pairDists.push_back(
            Dist{pA.first, pB.first,
                 compute3dDistance(std::span<long long, 3>(pA.second), std::span<long long, 3>(pB.second))});
    }
    std::ranges::sort(pairDists, [](auto const &a, auto const &b) { return a.dist < b.dist; });


    std::vector<std::vector<size_t>> circuits{};
    for (auto const &oneDist : pairDists) {
        std::optional<size_t> foundA_at;
        for (size_t id = 0; auto &oneCir : circuits) {
            auto iteA = std::ranges::find(oneCir, oneDist.pointA_id);
            if (iteA != oneCir.end()) {
                foundA_at = id;
                break;
            }
            id++;
        }
        std::optional<size_t> foundB_at;
        for (size_t id = 0; auto &oneCir : circuits) {
            auto iteB = std::ranges::find(oneCir, oneDist.pointB_id);
            if (iteB != oneCir.end()) {
                foundB_at = id;
                break;
            }
            id++;
        }

        // Putting the boxes in the right 'circuits'
        // A not found
        if (not foundA_at.has_value()) {
            // B not found
            if (not foundB_at.has_value()) { circuits.push_back({oneDist.pointA_id, oneDist.pointB_id}); }
            // B found
            else { circuits.at(foundB_at.value()).push_back(oneDist.pointA_id); }
        }
        // A found B not found
        else if (not foundB_at.has_value()) { circuits.at(foundA_at.value()).push_back(oneDist.pointB_id); }
        // Both are found
        else {
            if (foundA_at.value() != foundB_at.value()) {
                for (auto const &oneBnum : circuits.at(foundB_at.value())) {
                    circuits.at(foundA_at.value()).push_back(oneBnum);
                }
                circuits.erase(circuits.begin() + foundB_at.value());
            }
        }
        // If all boxes are in the first circuit we have the result
        if (circuits.front().size() == data.size()) {
            return data.at(oneDist.pointA_id).second.front() * data.at(oneDist.pointB_id).second.front();
        }
    }
    return 0uz;
}
} // namespace AOC2025