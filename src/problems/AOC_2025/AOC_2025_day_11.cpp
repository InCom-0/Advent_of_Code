#include <ankerl/unordered_dense.h>
#include <array>
#include <cassert>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <incstd/core/hashing.hpp>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <string>


namespace AOC2025 {

size_t
day11_1(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::map<std::string, size_t, incom::standard::hashing::XXH3Hasher> nameMap;
    size_t youID = std::numeric_limits<size_t>::max();
    size_t outID = std::numeric_limits<size_t>::max();
    {
        size_t i = 0;
        for (auto const &inpLine : input) {
            if (inpLine.front() == "you") { youID = i; }
            nameMap.insert({inpLine.front(), i++});
        }
        outID = i;
        nameMap.insert({"out", i});
    }

    std::vector<std::vector<size_t>> links;
    for (auto const &inpLine : input) {
        links.push_back({});
        for (auto const &oneLinkTo : std::span(inpLine.begin() + 1, inpLine.end())) {
            links.back().push_back(nameMap.at(oneLinkTo));
        }
    }
    std::vector<size_t> bfsStack{youID};

    size_t resAccu = 0uz;
    auto   recSolv = [&](this auto const &self) -> void {
        for (auto link : links.at(bfsStack.back())) {
            // if (link == youID) { continue; }
            if (link == outID) {
                resAccu++;
                continue;
            }
            else {
                bfsStack.push_back(link);
                self();
                bfsStack.pop_back();
            }
        }
    };

    recSolv();
    return resAccu;
}

size_t
day11_2(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::map<std::string, size_t, incom::standard::hashing::XXH3Hasher> nameMap;
    {
        size_t i = 0;
        for (auto const &inpLine : input) { nameMap.insert({inpLine.front(), i++}); }
        nameMap.insert({"out", i});
    }

    std::vector links(input.size() + 1, std::vector<size_t>{});
    for (size_t i = 0; auto const &inpLine : input) {
        for (auto const &oneLinkTo : std::span(inpLine.begin() + 1, inpLine.end())) {
            links[i].push_back(nameMap.at(oneLinkTo));
        }
        i++;
    }
    size_t const outID = nameMap.size() - 1;
    size_t const svrID = nameMap.at("svr");
    size_t const dacID = nameMap.at("dac");
    size_t const fftID = nameMap.at("fft");


    std::vector<std::optional<size_t>> solvedBP(links.size());


    std::vector<std::vector<size_t>> const solverPlan{{{svrID, dacID, fftID, outID}, {svrID, fftID, dacID, outID}}};

    size_t resAccu{};
    for (auto pthCombo : solverPlan) {
        auto   solved  = solvedBP;
        solved.back() = 1;
        size_t iterRes = 0uz;
        for (auto const [srcID, destID] : std::views::reverse(std::views::pairwise(pthCombo))) {

            std::vector<size_t> dfsStack{srcID};
            auto                recSolv = [&](this auto const &self) -> size_t {
                size_t res{};
                for (auto link : links.at(dfsStack.back())) {
                    if (auto &solvedNum = solved.at(link); solvedNum != std::nullopt) {
                         res += solvedNum.value(); }
                    else {
                        dfsStack.push_back(link);
                        res += self();
                        dfsStack.pop_back();
                    }
                }

                solved.at(dfsStack.back()) = res;
                return res;
            };

            iterRes                     = recSolv();
            solved                      = solvedBP;
            solved.at(dfsStack.front()) = iterRes;
        }
        resAccu += iterRes;
    }

    return resAccu;
}

} // namespace AOC2025