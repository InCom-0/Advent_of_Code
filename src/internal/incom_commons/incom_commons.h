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

namespace incom {
    namespace concepts {
        
namespace detail {
template <typename T, template <typename...> typename Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> typename Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};
} // namespace detail

template <typename T>
concept pair_t = requires(std::remove_cvref_t<T> pair) {
    { pair.first } -> std::same_as<typename T::first_type &>;
    { pair.second } -> std::same_as<typename T::second_type &>;
};
template <size_t N>
concept isPowerOf2 = ((N != 0) && ! (N & (N - 1)));

// Note: SpecializationOf does not support non-type template parameteres (at all) ... beware
template <typename T, template <typename...> typename Template>
concept SpecializationOf = detail::is_specialization_of<T, Template>::value;

// Generating a tuple with of N times the supplied type
// Usage: pass the N parameter and the T type only, leave the ...Ts pack empty
// Note: If you don't adhere to the above the output tuple will have more 'member types' than you want. Beware.
// Note: Uses recursive template instantiation.
// Note: T and Ts all must be the same types.
template <size_t N, typename T, typename... Ts>
requires(std::same_as<T, Ts> && ...)
struct _c_generateTuple {
    using type = typename _c_generateTuple<N - 1, T, T, Ts...>::type;
};
template <typename T, typename... Ts>
requires(std::same_as<T, Ts> && ...)
struct _c_generateTuple<0, T, Ts...> {
    using type = std::tuple<Ts...>;
};

// Generating an 'integer_sequence' of N identical size_t integers of value S.
// Usage: pass the N parameter and the S integer only, leave the ...Sx pack empty
// Note: If you don't adhere to the above the output integer_sequence will be longer than you (probably) want. Beware.
// Note: Uses recursive template instantiation.
// Note: S and Sx must all be the same value.
template <size_t N, long long S, long long... Sx>
requires((S == Sx) && ...)
struct c_gen_X_repeat_sequence {
    using type = typename c_gen_X_repeat_sequence<N - 1, S, S, Sx...>::type;
};
template <long long S, long long... Sx>
requires((S == Sx) && ...)
struct c_gen_X_repeat_sequence<0, S, Sx...> {
    using type = std::integer_sequence<long long, Sx...>;
};


    }
    namespace commons {

// This tiny type is used to simplify dealing with parameter packs.
// Usually used to invoke a closure (lambda) over each parameter in the pack.
// The closure is successivelly passed an in-place constructed instance of each type in the pack (the constructors are
// passed the 'ttptc' as an initializer)
template <typename... T>
struct argPackHLPR {
    template <typename... TypesToPassToConstructors>
    static void invokeOverConstructedPack(auto &&func, TypesToPassToConstructors &...ttptc) {
        (func(T{ttptc...}), ...);
    };
};

/*
Simple 'double buffer' class that can be useful when employing iterative algorithms that simply 'mutate' the input in
each iteration. Particularly useful for non-trivial data structures ... for instance containers of containers.
*/
template <typename T>
class doubleBuffer {
private:
    T __dataA;
    T __dataB;

    T *current = &__dataA;
    T *next    = &__dataB;

public:
    doubleBuffer() {};
    doubleBuffer(T initial_data) {
        __dataA = initial_data;
        __dataB = initial_data;
    };

    T &getCurrent() const { return (*current); };
    T &getNext() const { return (*next); }

    void swapBuffers() { std::swap(current, next); }
};

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
    requires more_concepts::random_access_container<std::remove_cvref_t<T>> &&
             std::is_arithmetic_v<typename T::value_type>
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

    template <incom::concepts::pair_t T>
    constexpr void _hashTypeX(T &input, XXH3_state_t *state) const {
        this->_hashTypeX(input.first, state);
        this->_hashTypeX(input.second, state);
    }

    template <typename T>
    requires more_concepts::random_access_container<std::remove_cvref_t<T>> &&
             std::is_arithmetic_v<typename T::value_type>
    constexpr void _hashTypeX(T &input, XXH3_state_t *state) const {
        XXH3_64bits_update(state, input.data(),
                           sizeof(typename std::remove_cvref_t<decltype(input)>::value_type) * input.size());
    }

    template <typename T>
    requires more_concepts::random_access_container<std::remove_cvref_t<T>> &&
             more_concepts::random_access_container<std::remove_cvref_t<typename T::value_type>>
    constexpr void _hashTypeX(T &input, XXH3_state_t *state) const {
        for (auto &VinV : input) { this->_hashTypeX(VinV, state); }
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
struct _instrBase_2018 {
    std::reference_wrapper<long long> A;
    std::reference_wrapper<long long> B;
    std::reference_wrapper<long long> C;

    _instrBase_2018(long long &aRef, long long &bRef, long long &cRef) : A{aRef}, B{bRef}, C{cRef} {};

    std::reference_wrapper<std::reference_wrapper<long long>> getMappedRef(const int &id) {
        if (id == 1) { return std::reference_wrapper<std::reference_wrapper<long long>>(A); }
        else if (id == 2) { return std::reference_wrapper<std::reference_wrapper<long long>>(B); }
        else if (id == 3) { return std::reference_wrapper<std::reference_wrapper<long long>>(C); }
        else { assert((void("ERROR"), false)); };
        std::unreachable();
    }
    virtual const std::vector<int> getRS() = 0;
};

template <typename... instrT>
requires(std::derived_from<instrT, _instrBase> && ...)
struct ProgramQuasiAssembly {
    std::unordered_map<char, std::reference_wrapper<long long>, incom::commons::XXH3Hasher> mapping;
    unsigned long long                                                                   instructionID = 0;
    long long                                                                            fakeRegister  = LLONG_MIN;
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
        std::unordered_map<std::string, std::variant<instrT...>, incom::commons::XXH3Hasher> instrTypeMap;
        (instrTypeMap.emplace(TypeToString<instrT>(), std::variant<instrT...>{instrT{fakeRegister, fakeRegister}}),
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

template <typename... instrT>
requires(std::derived_from<instrT, _instrBase_2018> && ...)
struct ProgramQuasiAssembly_2018 {
    // std::unordered_map<char, std::reference_wrapper<long long>, incom::commons::XXH3Hasher> mapping;
    unsigned long long                   instructionID = 0;
    std::vector<long long>               registers;
    std::vector<std::variant<instrT...>> instrVect;

    // Other convenience member functions
    inline bool                     test_isInstructionIDvalid() { return instructionID < instrVect.size(); }
    inline std::variant<instrT...> &getCurrentAndIncrement() { return instrVect[instructionID++]; }

    // In order to dynamically use the 'right' type for each instruction, one has to generate a map that maps the string
    // name representation (as found in the instructions) to the instantiation of std::variant<instrT...> with the
    // correct type
    static std::unordered_map<std::string, std::variant<instrT...>, incom::commons::XXH3Hasher> instrTypeMapCreator(
        const std::vector<std::vector<std::string>> &rawExampleInput, auto &overloadSet, const int registersCount = 4,
        const std::vector<long long> regStartVal = {0, 0, 0, 0}) {

        std::vector<long long> registers_local(registersCount, 0); // Registers
        std::vector<long long> fakeRegisters(4, LLONG_MIN);        // Fake registers
        std::vector<long long> values(4, LLONG_MIN);               // Values
        long long              fakeLong = LLONG_MIN;

        // Parses 3 lines from the example (that is one sample: 1) start regState, 2) OpCode, 3) end regState) into
        // simple VofV of 'long long'. Skips the 4th line which is always empty in input data.
        std::vector<std::vector<long long>> parsed(4, std::vector<long long>());
        auto                                parseOneSet = [&, line = 0]() mutable -> bool {
            for (int i = 0; i < 4; ++i) { parsed[i].clear(); }
            for (int i = 0; i < 3; ++i) {
                if (line >= rawExampleInput.size()) {
                    assert((void("Exit of parsing one example set when the 'parsed' vector isn't empty"),
                            parsed[0].empty() && parsed[1].empty() && parsed[2].empty() && parsed[3].empty()));
                    return false;
                }

                for (int k = 0; k < 4; ++k) { parsed[i].push_back(std::stoll(rawExampleInput[line][k])); }
                line++;
            }
            line++;
            return true;
        };

        // Matcher attempts to execute a sample as if it were each OpCode.
        // Outputs a vector of valid instruction IDs (the IDs match the position in the parameter pack).
        auto matcher = [&]() -> std::vector<int> {
            std::vector<std::variant<instrT...>> variantVector;
            (variantVector.push_back(instrT{fakeLong, fakeLong, fakeLong}), ...);

            for (auto &var : variantVector) {
                auto regValInstructions = std::visit([](auto &&a) { return a.getRS(); }, var);
                for (int k = 1; auto &regOrVal : regValInstructions) {
                    auto refToRef = std::visit([&](auto &&a) { return a.getMappedRef(k); }, var);

                    if (regOrVal == 1) { refToRef.get() = values[k]; }
                    else if (parsed[1][k] >= registersCount) { refToRef.get() = fakeRegisters[k]; }
                    else { refToRef.get() = registers_local[parsed[1][k]]; }
                    k++;
                }
            }
            values[1] = parsed[1][1];
            values[2] = parsed[1][2];
            values[3] = parsed[1][3];

            std::vector<int> res;
            for (int j = 0; j < sizeof...(instrT); ++j) {
                for (int i = 0; i < registersCount; ++i) { registers_local[i] = parsed[0][i]; }
                std::visit(overloadSet, variantVector[j]);

                int innerCounter = 0;
                for (int k = 0; k < registersCount; ++k) {
                    if (registers_local[k] == parsed[2][k]) { innerCounter++; }
                }
                if (innerCounter == registersCount) { res.push_back(j); }
            }
            return res;
        };


        // Push_backs a vector of valid instrIDs to the right rawID position in the top-level vector of
        // rawIdToVofV_InstrIDs
        std::vector rawIdToVofV_InstrIDs(sizeof...(instrT), std::vector<std::vector<int>>());
        std::vector counter(sizeof...(instrT), std::vector<int>(sizeof...(instrT), 0));

        while (parseOneSet()) { rawIdToVofV_InstrIDs[parsed[1][0]].push_back(matcher()); }

        for (int i = 0; auto &rawIDgroup : rawIdToVofV_InstrIDs) {
            for (auto &validIDsGroup : rawIDgroup) {
                for (auto &validIDinGroup : validIDsGroup) { counter[i][validIDinGroup]++; }
            }
            i++;
        }


        // There can only be one 'real' matching 'instrID' per rawID and must match for all (ie. the size of
        // the vector). Depending on input might be necessary to gradually eliminate multiple matches.
        std::unordered_map<int, std::string, incom::commons::XXH3Hasher> IDsMap;

        while (IDsMap.size() < counter.size()) {
            for (int j = 0; auto &counterRangePerID : counter) {
                int idMatched = INT_MIN;
                for (int i = 0; auto &cnt : counterRangePerID) {
                    if (cnt == rawIdToVofV_InstrIDs[j].size()) {
                        if (idMatched != INT_MIN) {
                            idMatched = INT_MIN;
                            break;
                        }
                        idMatched = i;
                    }
                    i++;
                }
                if (idMatched != INT_MIN) {
                    IDsMap.emplace(idMatched, std::to_string(j));
                    for (auto &cntrRng : counter) { cntrRng[idMatched] = 0; }
                }
                j++;
            }
        }

        std::unordered_map<std::string, std::variant<instrT...>, incom::commons::XXH3Hasher> instrTypeMap;
        long long                                                                         cntr = 0;

        // Horrifying hack creating a dangling reference with the 'counter' being passed into Instr_T constructor
        (instrTypeMap.emplace(IDsMap.at(cntr++), std::variant<instrT...>{instrT{cntr, cntr, cntr}}), ...);
        return instrTypeMap;
    };

    // The one and only constructor of the 'prog' type
    ProgramQuasiAssembly_2018(const std::vector<std::vector<std::string>> &rawInstrInput,
                              std::unordered_map<std::string, std::variant<instrT...>, incom::commons::XXH3Hasher> &mapped,
                              const std::vector<long long> regStartVal = {0, 0, 0, 0}) {

        assert((void("Prog type instantiated with an empty input"), rawInstrInput.size() > 0));
        for (auto &line : rawInstrInput) {
            assert((void("Prog type instantiated with an input that has more than 4 items on some instruction line"),
                    line.size() < 5));
            assert(
                (void("Prog type instantiated with an input that has some instruction line empty"), line.size() > 0));
        }

        registers.resize(rawInstrInput.size() * 3); // Must NEVER reallocate.

        // This loop finds out how many registers there actually are.
        int lastRegOccupied = INT_MIN;
        for (auto &line : rawInstrInput) {
            assert((void("Some instructions name in input doesn't match any type in template parameter pack"),
                    mapped.contains(line.front())));

            auto refVal_instructions =
                std::visit([&](auto &&a) -> std::vector<int> { return a.getRS(); }, mapped.at(line.front()));

            std::visit(
                [&](auto &&a) -> void {
                    if (refVal_instructions[0] == 0) {
                        lastRegOccupied = std::max(lastRegOccupied, std::stoi(line[1]));
                    }
                    if (refVal_instructions[1] == 0) {
                        lastRegOccupied = std::max(lastRegOccupied, std::stoi(line[1]));
                    }
                    if (refVal_instructions[2] == 0) {
                        lastRegOccupied = std::max(lastRegOccupied, std::stoi(line[1]));
                    }
                },
                mapped.at(line.front()));
        }


        assert((void("Inferred number of registers do not match the number of registers presumed by the programmer"),
                regStartVal.size() == (lastRegOccupied + 1)));

        // This loop set the right reference and copies the right variant into instrVect
        for (auto &line : rawInstrInput) {
            auto &variantInMap = mapped.at(line.front());
            auto  refVal_instructions_2 =
                std::visit([&](auto &&a) -> std::vector<int> { return a.getRS(); }, variantInMap);

            std::visit(
                [&](auto &&a) -> void {
                    if (refVal_instructions_2[0] == 0) { a.A = registers[std::stoi(line[1])]; }
                    else {
                        registers[++lastRegOccupied] = std::stoll(line[1]);
                        a.A                          = registers[lastRegOccupied];
                    }
                    if (refVal_instructions_2[1] == 0) { a.B = registers[std::stoi(line[2])]; }
                    else {
                        registers[++lastRegOccupied] = std::stoll(line[2]);
                        a.B                          = registers[lastRegOccupied];
                    }
                    if (refVal_instructions_2[2] == 0) { a.C = registers[std::stoi(line[3])]; }
                    else {
                        registers[++lastRegOccupied] = std::stoll(line[3]);
                        a.C                          = registers[lastRegOccupied];
                    }
                },
                variantInMap);
            // Push the right variant inside instrVect
            instrVect.push_back(std::variant<instrT...>(variantInMap));
        }
        // Set registers to their initial values (defaulted 4 registers to 0).
        for (int i = 0; auto &oneVal : regStartVal) { registers[i++] = oneVal; }
    }
};
} // namespace PQA
    }
} // namespace AOC_commons
