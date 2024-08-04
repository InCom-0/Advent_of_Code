#pragma once

#include <cassert>
#include <concepts>
#include <fstream>
#include <source_location>
#include <vector>


#include <ctre.hpp>
#include <flux.hpp>
#include <more_concepts/more_concepts.hpp>
#include <xxhash.h>


// A lambda operator() overload 'trick' type.
// Mostly used for 'inline' creation of logic for std::visit of std::variant
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
    template <typename T>
    requires more_concepts::random_access_container<std::remove_cvref_t<T>> && std::is_arithmetic_v<typename T::value_type>
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

    template <typename T>
    requires more_concepts::random_access_container<std::remove_cvref_t<T>> && std::is_arithmetic_v<typename T::value_type>
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

// ENTIRELY POSSIBLE THAT THIS HACKY NAMESPACE IS NOT REALLY PORTABLE ... BEWARE.
namespace PQA {
/*
Quasi compile time reflection for typenames
*/
template <typename T>
consteval auto TypeToString() {
    auto EmbeddingSignature = std::string_view{std::source_location::current().function_name()};
    auto firstPos           = EmbeddingSignature.rfind("::") + 2;
    return EmbeddingSignature.substr(firstPos, EmbeddingSignature.size() - firstPos - 1);
}
struct _instrBase {
    std::reference_wrapper<long long> source;
    std::reference_wrapper<long long> target;
};

template <typename... instrT>
requires(std::derived_from<instrT, _instrBase> && ...)
struct ProgramQuasiAssembly {
    std::unordered_map<char, std::reference_wrapper<long long>, AOC_commons::XXH3Hasher> mapping;
    unsigned long long                                                                   instructionID = 0;
    long long                                                                            fakeRegister = LLONG_MIN;
    std::vector<long long>                                                               registers;
    std::vector<std::variant<instrT...>>                                                 instrVect;

    // The one and only constructor of the 'prog' type
    ProgramQuasiAssembly(const std::vector<std::vector<std::string>> &input, const long long registersStartValue = 0) {
        assert((void("Prog type instantiated with an empty input"), input.size() > 0));
        for (auto &line : input) {
            assert((void("Prog type instantiated with an input that has more than 3 items on some instruction line"),
                    line.size() < 4));
            assert(
                (void("Prog type instantiated with an input that has some instruction line empty"), line.size() > 0));
        }

        // Mapping a type 'by string' to the same type inside a std::variant instance;
        // TypeToString uses a very crude form of 'reflection'.
        // ENTIRELY POSSIBLE THAT THIS HACK IS NOT REALLY PORTABLE ... BEWARE.
        std::unordered_map<std::string, std::variant<instrT...>, AOC_commons::XXH3Hasher> instrTypeMap;
        (instrTypeMap.emplace(TypeToString<instrT>(),
                              std::variant<instrT...>{instrT{fakeRegister, fakeRegister}}),
         ...);

        registers.reserve(input.size() * 2); // Must NEVER reallocate.

        for (auto &line : input) {
            assert((void("Some instructions name in input doesn't match any type in template parameter pack"),
                    instrTypeMap.contains(line.front())));

            for (long long i = 1; i < line.size(); ++i) {
                if (line[i].front() >= 'a' && line[i].front() <= 'z') {
                    // 'Named' registers are inserted into a vector named registers + UOmap is created to
                    // obtain the right reference later
                    if (not mapping.contains(line[i].front())) {
                        registers.push_back(registersStartValue);
                        mapping.emplace(line[i].front(), registers.back());
                    }
                }
            }
        }
        for (auto &line : input) {
            std::reference_wrapper<long long> firstR  = fakeRegister;
            std::reference_wrapper<long long> secondR = fakeRegister;
            if (line.size() >= 2) {
                if (line[1].front() >= 'a' && line[1].front() <= 'z') { firstR = mapping.at(line[1][0]); }
                else {
                    // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register
                    // used only in that instruction
                    registers.push_back(std::stoi(line[1]));
                    firstR = registers.back();
                }
            }
            if (line.size() >= 3) {
                if (line[2].front() >= 'a' && line[2].front() <= 'z') { secondR = mapping.at(line[2][0]); }
                else {
                    // Integer inputs in the instructions are treated 'as-if' it were another but 'unnamed' register
                    // used only in that instruction
                    registers.push_back(std::stoi(line[2]));
                    secondR = registers.back();
                }
            }

            // Update just the right variant item in the instrTypeMap.
            std::visit(
                [&](auto &&a) {
                    a.source = firstR;
                    a.target = secondR;
                },
                instrTypeMap.at(line.front()));

            // Push the right variant inside instrVect
            instrVect.push_back(std::variant<instrT...>(instrTypeMap.at(line.front())));
        }
        return;
    }

    // Other convenience member functions
    inline bool                     test_isInstructionIDvalid() { return instructionID < instrVect.size(); }
    inline std::variant<instrT...> &getCurrentAndIncrement() { return instrVect[instructionID++]; }
};
} // namespace PQA
} // namespace AOC_commons
