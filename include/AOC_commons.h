#pragma once

#include <concepts>
#include <fstream>
#include <vector>

#include <ctre.hpp>
#include <flux.hpp>
#include <more_concepts/more_concepts.hpp>
#include <xxhash.h>


template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

namespace AOC_concepts {
template <typename T>
concept pair_t = requires(std::remove_cvref_t<T> pair) {
    { pair.first } -> std::same_as<typename T::first_type &>;
    { pair.second } -> std::same_as<typename T::second_type &>;
};
template <typename CONTAINER>
concept contigCofArithm =
    (std::__is_specialization_of<std::remove_cvref_t<CONTAINER>, std::vector> ||
     std::same_as<std::array<typename std::remove_cvref_t<CONTAINER>::value_type, std::tuple_size<CONTAINER>{}>,
                  std::remove_cvref_t<CONTAINER>> ||
     std::convertible_to<CONTAINER, std::string_view>) &&
    std::is_arithmetic_v<typename std::remove_cvref_t<CONTAINER>::value_type> == true;

} // namespace AOC_concepts

namespace AOC_commons {

/*
Matrix rotation of 'indexed' random access containers.
Uses 'swapping in circles' method ... should be pretty fast
*/
template <typename T>
requires more_concepts::random_access_container<T> && more_concepts::random_access_container<typename T::value_type> &&
         std::swappable<typename T::value_type::value_type>
void matrixRotateLeft(T &VofVlike) {
    int sideLength = VofVlike.size() - 1;
    if (sideLength < 1) { return; }
    if (flux::ref(VofVlike).any([&](auto &&line) { return line.size() != VofVlike.size(); })) { return; }

    int circles = (sideLength + 2) / 2;
    for (int cir = 0; cir < circles; cir++) {
        for (int i = 0; i < sideLength - (2 * cir); ++i) {
            std::swap(VofVlike[cir][cir + i], VofVlike[cir + i][sideLength - cir]);
            std::swap(VofVlike[cir + i][sideLength - cir], VofVlike[sideLength - cir][sideLength - cir - i]);
            std::swap(VofVlike[sideLength - cir][sideLength - cir - i], VofVlike[sideLength - cir - i][cir]);
        }
    }
    return;
}
template <typename T>
requires more_concepts::random_access_container<T> && more_concepts::random_access_container<typename T::value_type> &&
         std::swappable<typename T::value_type::value_type>
void matrixRotateRight(T &VofVlike) {
    int sideLength = VofVlike.size() - 1;
    if (sideLength < 1) { return; }
    if (sideLength + 1 != VofVlike.front().size()) { return; }
    if (flux::ref(VofVlike).any([&](auto &&line) { return line.size() != VofVlike.size(); })) { return; }

    int circles = (sideLength + 2) / 2;
    for (int cir = 0; cir < circles; cir++) {
        for (int i = 0; i < sideLength - (2 * cir); ++i) {
            std::swap(VofVlike[cir][cir + i], VofVlike[sideLength - cir - i][cir]);
            std::swap(VofVlike[sideLength - cir - i][cir], VofVlike[sideLength - cir][sideLength - cir - i]);
            std::swap(VofVlike[sideLength - cir][sideLength - cir - i], VofVlike[cir + i][sideLength - cir]);
        }
    }
    return;
}

/*
Hashes using XXH3_64bit 'state of the art' non-cryptographic hasher.
Can/should be used for hasing keys in maps, sets and the like.

Hashes arithmetic types + std::vector, std::array and std::pair of the former.
The std::pair can nest all of the above containers + recursive nesting is possible.

TODO: Extend this so that it can hash mostly everything.
TODO: Make it so that when used with a constant size array the size is used as compile time constant.
_
*/
struct XXH3Hasher {
    // DIRECTLY HASHABLE BECAUSE OF CONTIGUOUS DATA
    template <typename T>
    requires std::is_arithmetic_v<std::decay_t<T>>
    constexpr size_t operator()(T &&input) const {
        return XXH3_64bits(&input, sizeof(T));
    }
    template <AOC_concepts::contigCofArithm T>
    constexpr size_t operator()(T &&input) const {
        return XXH3_64bits(input.data(),
                           sizeof(typename std::remove_cvref_t<decltype(input)>::value_type) * input.size());
    }

    // REQUIRES GRADUAL BUILDUP OF XXH3_STATE OUT OF DIS-CONTIGUOUS DATA INSIDE THE INPUT TYPE
    template <typename T>
    constexpr size_t operator()(T &&input) const {
        XXH3_state_t *state = XXH3_createState();
        XXH3_64bits_reset(state);
        this->_hashTypeX(input, state);

        XXH64_hash_t result = XXH3_64bits_digest(state);
        XXH3_freeState(state);
        return result;
    }

    template <typename T>
    requires std::is_arithmetic_v<std::decay_t<T>>
    constexpr void _hashTypeX(T &input, XXH3_state_t *state) const {
        XXH3_64bits_update(state, &input, sizeof(T));
    }

    template <AOC_concepts::pair_t T>
    constexpr void _hashTypeX(T &input, XXH3_state_t *state) const {
        this->_hashTypeX(input.first, state);
        this->_hashTypeX(input.second, state);
    }

    template <AOC_concepts::contigCofArithm T>
    constexpr void _hashTypeX(T &input, XXH3_state_t *state) const {
        XXH3_64bits_update(state, input.data(),
                           sizeof(typename std::remove_cvref_t<decltype(input)>::value_type) * input.size());
    }
};

class parseInputUsingCTRE {
private:
    struct oneLineProcessedHolder {
        std::vector<std::string> insideVofS;
        size_t                   cursor              = 0;
        int                      somethingNotFoundAt = -1;

        oneLineProcessedHolder &operator<<(auto &&toInsert) {
            if (toInsert) { insideVofS.push_back(toInsert.to_string()); }
            else { somethingNotFoundAt = cursor; }
            cursor++;
            return *this;
        }
    };

    /*
    Questionable necessity to use a template for fileProcessedHolder.
    Only did it to learn how to deal with parameter pack 'sizeof...' and constexpr context.

    Thought came to mind ... would it be a good idea to implement this using some sort of flux sequence?
    That might be nicer && might avoid using that crude 'cursor' and actually learn to use a well made cursors :-).
    ... we shall see some other day ...
    */
    template <size_t sz>
    struct fileProcessedHolder {
        std::vector<std::vector<std::string>> data =
            std::vector<std::vector<std::string>>(sz, std::vector<std::string>());
        size_t cursor              = 0;
        int    somethingNotFoundAt = -1;

        fileProcessedHolder &operator<<(auto &&toInsert) {
            if (toInsert) { data[cursor++].push_back(toInsert.to_string()); }
            else { somethingNotFoundAt = cursor++; }
            return *this;
        }

        void resetCursor() { cursor = 0; }
    };

    struct fileProcessedHolder_2 {
        std::vector<std::vector<std::string>> data                = std::vector<std::vector<std::string>>();
        int                                   somethingNotFoundAt = -1;

        fileProcessedHolder_2 &operator<<(auto &&toInsert) {
            if (toInsert) { data.back().push_back(toInsert.to_string()); }
            else { somethingNotFoundAt = data.size(); }
            return *this;
        }
    };

    static auto findNextWithinLine(auto &ctreSrchObj, std::string::iterator &begin, const std::string::iterator &end) {
        auto result = ctreSrchObj(begin, end);
        if (result) { begin = result.get_end_position(); }
        return result;
    }

public:
    /*
    It might be a good idea to implement some sort of concept restrictions on typename ctreSrch.
    Might get back to it at some point ... but for my usage this is not really necessary ... will learn Concepts some
    other day

    Also ... should probably learn how to use exceptions the right way :-)
    */

    template <typename... ctreSrch>
    static std::vector<std::string> processOneLine(std::string &line, ctreSrch &&...perItemInLine) {
        auto                   bg  = line.begin();
        auto                   end = line.end();
        oneLineProcessedHolder sink;
        (sink << ... << findNextWithinLine(perItemInLine, bg, end));
        return std::move(sink.insideVofS);
    }

    template <typename... ctreSrch>
    static std::vector<std::string> processOneLineRPToneVect(std::string &line, ctreSrch &&...perItemInLine) {
        oneLineProcessedHolder sink;

        auto bg  = line.begin();
        auto end = line.end();
        do {
            for (int i = 0; i < sizeof...(perItemInLine); ++i) {
                (sink << ... << findNextWithinLine(perItemInLine, bg, end));
            }
        } while (sink.somethingNotFoundAt == -1);
        return std::move(sink.insideVofS);
    }

    template <typename... ctreSrch>
    static std::vector<std::vector<std::string>> processOneLineRPT(std::string &line, ctreSrch &&...perItemInLine) {
        constexpr size_t                              searchForNumOfItems = sizeof...(perItemInLine);
        fileProcessedHolder<sizeof...(perItemInLine)> sink;

        auto bg  = line.begin();
        auto end = line.end();

        while (true) {
            for (int i = 0; i < searchForNumOfItems; ++i) {
                (sink << ... << findNextWithinLine(perItemInLine, bg, end));
                sink.resetCursor();
            }
            if (sink.somethingNotFoundAt != -1) { break; }
        }
        return sink.data;
    }

    template <typename... ctreSrch>
    static std::vector<std::vector<std::string>> processOneLineRPTinFile(std::string &dataFile,
                                                                         ctreSrch &&...perItemInLine) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) {
            return std::vector<std::vector<std::string>>(1, std::vector<std::string>(1, "STREAM NOT OPEN"));
        }

        std::string oneStr;
        std::getline(iStream, oneStr);

        return processOneLineRPT(oneStr, perItemInLine...);
    }

    template <typename... ctreSrch>
    static std::vector<std::vector<std::string>> processFile(std::string &dataFile, ctreSrch &&...perItemInLine) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) {
            return std::vector<std::vector<std::string>>(1, std::vector<std::string>(1, "STREAM NOT OPEN"));
        }

        fileProcessedHolder<sizeof...(perItemInLine)> sink;
        std::string                                   oneStr;

        while (std::getline(iStream, oneStr)) {
            auto bg  = oneStr.begin();
            auto end = oneStr.end();

            (sink << ... << findNextWithinLine(perItemInLine, bg, end));
            sink.resetCursor();
        }
        return sink.data;
    }
    template <typename... ctreSrch>
    static std::vector<std::vector<std::string>> processFileRPT(std::string &dataFile, ctreSrch &&...perItemInLine) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) {
            return std::vector<std::vector<std::string>>(1, std::vector<std::string>(1, "STREAM NOT OPEN"));
        }

        fileProcessedHolder_2 sink;
        std::string           oneStr;
        constexpr size_t      searchForNumOfItems = sizeof...(perItemInLine);

        while (std::getline(iStream, oneStr)) {
            sink.data.push_back(std::vector<std::string>());
            auto bg  = oneStr.begin();
            auto end = oneStr.end();
            while (true) {
                for (int i = 0; i < searchForNumOfItems; ++i) {
                    (sink << ... << findNextWithinLine(perItemInLine, bg, end));
                }
                if (sink.somethingNotFoundAt != -1) { break; }
            }
            sink.somethingNotFoundAt = -1;
        }
        return std::move(sink.data);
    }
};
} // namespace AOC_commons
