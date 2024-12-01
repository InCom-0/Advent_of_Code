#pragma once

#include <ankerl/unordered_dense.h>
#include <incom_commons.h>
#include <more_concepts/more_concepts.hpp>


namespace incom {
namespace seq {

template <typename T, typename F>
requires more_concepts::container<T> && std::predicate<F, std::vector<typename T::value_type>>
auto build_map_uniqueSubSeq2startPos(T const &inputSequence, size_t const min_subSeqSize, F filter_subSeq) {
    ankerl::unordered_dense::map<std::vector<typename T::value_type>,
                                 ankerl::unordered_dense::set<size_t, incom::commons::XXH3Hasher>,
                                 incom::commons::XXH3Hasher>
        mapToBuild;

    std::vector dp(inputSequence.size() + 1, std::vector(inputSequence.size() + 1, 0));

    for (int i = inputSequence.size() - 1; i >= 0; --i) {
        for (int j = inputSequence.size() - 1; j > i; --j) {
            if (inputSequence[i] == inputSequence[j]) {
                dp[i][j] = 1 + std::min(dp[i + 1][j + 1], j - i - 1);
                for (int add = dp[i][j]; add >= min_subSeqSize; --add) {
                    std::vector<typename T::value_type> tmpSubSeq(inputSequence.begin() + i,
                                                                  inputSequence.begin() + i + add);
                    if (filter_subSeq(tmpSubSeq)) {
                        mapToBuild[tmpSubSeq].emplace(i);
                        mapToBuild[tmpSubSeq].emplace(j);
                    }
                }
            }
        }
    }
    return mapToBuild;
}

template <typename T, typename F>
requires more_concepts::container<T> && std::predicate<F, std::vector<typename T::value_type>>
auto solve_seqFromRepUniqueSubseq(T const &inputSequence, size_t const min_subSeqSize,
                                  size_t const max_ofUniqueSubseqInRes, F const filter_subSeq) {

    ankerl::unordered_dense::map<std::vector<typename T::value_type>,
                                 ankerl::unordered_dense::set<size_t, incom::commons::XXH3Hasher>,
                                 incom::commons::XXH3Hasher> const mp_subseq_2_ids =
        build_map_uniqueSubSeq2startPos(inputSequence, min_subSeqSize, filter_subSeq);

    ankerl::unordered_dense::map<size_t, std::vector<std::vector<typename T::value_type>>, incom::commons::XXH3Hasher>
        mp_pos_2_subseq;

    for (auto const &mpItem : mp_subseq_2_ids) {
        for (auto const &posItem : mpItem.second) {
            mp_pos_2_subseq.insert({posItem, std::vector<std::vector<typename T::value_type>>()});
            mp_pos_2_subseq[posItem].push_back(mpItem.first);
        }
    }
    size_t curHead = 0;
    ankerl::unordered_dense::map<typename std::vector<typename T::value_type>, size_t, incom::commons::XXH3Hasher>
        selTracker;

    // Recursive solver.
    // Explores in a DFS manner all the possible arrangements of subsequences from the beginning
    // Respects how many different subsequences can be used (therefore short circuits on most of the unsuitable
    // parts of the tree)
    auto __inside_solver = [&](this auto &self) -> std::optional<std::vector<std::vector<typename T::value_type>>> {
        for (auto const &selOption : mp_pos_2_subseq[curHead]) {

            if (selTracker.contains(selOption)) { selTracker.at(selOption)++; }
            else if (selTracker.size() < max_ofUniqueSubseqInRes) { selTracker.emplace(selOption, 1); }
            else { continue; }

            curHead += selOption.size();
            if (curHead == inputSequence.size()) {
                return std::vector(1, selOption);
            } // Sucess! Unroll recursion afterwards

            // Recursive call
            std::optional<std::vector<std::vector<typename T::value_type>>> res = self();
            if (res != std::nullopt) {
                res.value().push_back(selOption);
                return res.value();
            }

            curHead -= selOption.size();

            selTracker.at(selOption)--;
            if (selTracker.at(selOption) == 0) { selTracker.erase(selOption); }
        }
        return std::nullopt;
    };

    auto res = __inside_solver();
    return res.and_then([](auto &val) -> std::optional<std::vector<std::vector<typename T::value_type>>> {
        std::ranges::reverse(val);
        return val;
    });
}
} // namespace seq
} // namespace incom