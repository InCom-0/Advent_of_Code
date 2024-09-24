#pragma once

#include <AOC_commons.h>

#include <ctre.hpp>
#include <flux.hpp>
#include <mdspan/mdspan.hpp>
#include <more_concepts/more_concepts.hpp>
#include <robin_hood.h>


#include <concepts>
#include <type_traits>


namespace AOC2018 {


template <typename T, template <typename...> typename Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> typename Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};
// Note: SpecializationOf does not support non-type template parameteres (at all) ... beware
template <typename T, template <typename...> typename Template>
concept SpecializationOf = is_specialization_of<T, Template>::value;

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
template <size_t N, size_t S, size_t... Sx>
requires((S == Sx) && ...)
struct c_gen_X_repeat_sequence {
    using type = typename c_gen_X_repeat_sequence<N - 1, S, S, Sx...>::type;
};
template <size_t S, size_t... Sx>
requires((S == Sx) && ...)
struct c_gen_X_repeat_sequence<0, S, Sx...> {
    using type = std::integer_sequence<size_t, Sx...>;
};


/* 'Omni Vector' container type

Omni => The vector goes both ways ... that is you can index into 'Omni_Vector' with negative indices.
Omni => Made to nativelly supports N dimensions aka: nesting (as in eg. 'vector<vector<int>>' is 2-dimensional)

By no means 'industrial grade'

-

Above all the most important feature:
There is an internal type (_OV) that the Omni Vector uses and that supports templated nesting within itself.
It knows at compile time when and how many times it is nested within itself (enabled through features of the type system
AND template metaprogramming). This enables programmatic control of the whole structure by calling functions on the top
level only (for commonly used tasks). Thereby abstracting away most of the pesky details of managing such nested data
structures (which is typically left to the programmer to deal with 'by hand')

_

Fast access to all levels using the traditional operator[]. 'Automatic construction' of the contained data structure
below the top level. 'Automatic expansion' of the contained data structure using the 'expand_ifNecessary(IDs... ids)'
member function (usually called before accessing data item with the same 'ids...'). No push_back(), push_front(),
pop_back(), pop_front(). Expands in 'chunks' of ('blockSize' power_to #of dimensions). Can't reduce size once expanded.

_

Usage:
Simply specialize the Omni_Vector with the type you need contained (first template argument).
Optionally provide 3 more non-type template arguments: 2) # of dimensions 3) Default value for for the type contained,
4) Block size

It should be excellent if you data is highly 'clustered' in practice (including multiple clusters)
Horrifyingly unsuitable if your data is 'sparse' in practice with massive empty/unexplored/defaulted areas in between.
_

Note:
It is not intended to be used the same way or for the same purpose as std::vector<T>.
The main intended use case is when one needs to execute some sort of 'explorative' algorithm in space of arbitrary
number of dimensions and the 'exploration' happens from a central point 'one step at a time' in unpredictable directions
(including extreme cases).
_

Mostly created as an excercise into compile time (meta)programming and usage of the various more modern features of the
C++ typesystem.


 */

template <typename Data_T, size_t c_numOfDimensions = 1, Data_T c_defaultValue = Data_T(), size_t c_blockSize = 8>
requires std::copy_constructible<Data_T> && std::is_default_constructible_v<Data_T>
class Omni_Vector {

private:
    template <typename T>
    class _OV {
    private:
        // DATA MEMBERS
        std::vector<T> m_posVect;
        std::vector<T> m_negVect;

    public:
        // CONSTRUCTION AND DESTRUCTION

        // Constructs all 'levels' below automatically. Contained data is default copy initialized to the value of
        // 'c_defaultValue'. Default data is created in each dimension: 'c_blockSize' number of elements in each
        // direction. For 2 dimensions with c_blockSize 16 => (2^2=4)*(16^2) = 1'024. For 3 dimensions with c_blockSize
        // 16 => (2^3=8)*(16^3) = 32'768. Beware of dimensionality !!!
        _OV() {
            if constexpr (SpecializationOf<T, _OV>) {
                m_posVect = std::vector<T>(c_blockSize, T());
                m_negVect = std::vector<T>(c_blockSize, T());
            }
            else if constexpr (std::is_same_v<T, std::remove_cvref_t<Data_T>>) {
                m_posVect = std::vector<T>(c_blockSize, c_defaultValue);
                m_negVect = std::vector<T>(c_blockSize, c_defaultValue);
            }
            else {
                static_assert(
                    false,
                    "Impossible to construct on the last level due to Data_T type not the same as the 'typename T'");
            }
        }

        // ACCESS
        // TODO: Would be fantastic to add iterator and ranges support

        // Acess style modelling vector
        // Can use the
        template <typename First_ID, typename... Other_IDs>
        auto &operator[](First_ID const &&id, Other_IDs const &&...other_ids) {
            if constexpr (sizeof...(other_ids) == 0) {
                if (id < 0) { return m_negVect[(-id) - 1]; }
                else { return m_posVect[id]; }
            }
            else {
                if (id < 0) { return m_negVect[(-id) - 1, std::forward<decltype(other_ids)>(other_ids)...]; }
                else { return m_posVect[id, std::forward<decltype(other_ids)>(other_ids)...]; }
            }
            std::unreachable();
        }

        // CONSTEVAL STATIC QUERYING
        consteval static size_t get_blockSize() { return c_blockSize; }
        consteval static Data_T get_defaultValue() { return c_defaultValue; }
        consteval static size_t get_depthFromTop() { return c_numOfDimensions; }

        consteval static size_t get_depth() {
            if constexpr (SpecializationOf<T, _OV>) { return T::get_depth() + 1; }
            else { return 1; }
        }

        // EXPANSION

        // This will expand recursivelly (happens when higher levels need to be expanded)
        // Returns how many times it performed expansion. If no expansion took place returns 0.
        template <typename... IDs>
        size_t expand_ifNecessary(IDs... ids) {
            static_assert(get_depth() == (sizeof...(ids)),
                          "You passed incorrect number of indices to 'expand_ifNecessary' ... ");

            size_t    res = 0;
            long long local_expandAtLvl;
            while ((local_expandAtLvl = find_firstInvalidSignedDepth(std::forward<decltype(ids)>(ids)...))) {
                res++;
                expand_atSignedLvl(local_expandAtLvl, ids...);
            }
            return res;
        }

        template <typename... OtherIDs>
        void expand_atSignedLvl(long long signedLvlToExpand, long long idx_onThisLevel,
                                OtherIDs... idxs_onOtherLevels) {
            static_assert(get_depth() == (sizeof...(idxs_onOtherLevels) + 1),
                          "Incorrect argument passing to 'expand_atSignedLvl' ...");

            // Specialized using if constexpr
            // This must be done, because one can only call 'expand_atSignedLvl' when T is a specialization of
            // Omni_Vector

            if constexpr (SpecializationOf<T, _OV>) {
                if (std::abs(signedLvlToExpand) != 1) {
                    // The whole 'block size' number of elements in which idx_onThisLevel lies needs to be called with
                    // 'expand_atSignedLvl'
                    long long fromID =
                        (std::abs(idx_onThisLevel + (idx_onThisLevel < 0 ? 1 : 0)) / c_blockSize) * c_blockSize;
                    long long untilBeforeID = fromID + c_blockSize;
                    if (idx_onThisLevel < 0) {
                        for (size_t i = fromID; i < untilBeforeID; ++i) {
                            this->m_negVect[i].expand_atSignedLvl(signedLvlToExpand + (signedLvlToExpand < 0 ? 1 : -1),
                                                                  idxs_onOtherLevels...);
                        }
                    }
                    else {
                        for (size_t i = fromID; i < untilBeforeID; ++i) {
                            this->m_posVect[i].expand_atSignedLvl(signedLvlToExpand + (signedLvlToExpand < 0 ? 1 : -1),
                                                                  idxs_onOtherLevels...);
                        }
                    }
                }
                else {
                    if (signedLvlToExpand < 0) {
                        for (int i = 0; i < c_blockSize; ++i) { this->m_negVect.push_back(typename T::_OV()); }
                    }
                    else {
                        for (int i = 0; i < c_blockSize; ++i) { this->m_posVect.push_back(typename T::_OV()); }
                    }
                }
            }

            else if constexpr (sizeof...(idxs_onOtherLevels) == 0) {
                if (signedLvlToExpand < 0) {
                    for (int i = 0; i < c_blockSize; ++i) { this->m_negVect.push_back(c_defaultValue); }
                }
                else {
                    for (int i = 0; i < c_blockSize; ++i) { this->m_posVect.push_back(c_defaultValue); }
                }
            }
            else {
                static_assert(false, "Incorrect argument passing to expand_atSignedLvl ... T is neither '_OV' nor is "
                                     "it the 'last level' with 'idxs_onOtherLevels' pack of size 0");
            }
        }


        // BOUNDS CHECKING

        // UNUSUAL BEHAVIOR
        // If the value returned is positive then the m_posVect access is invalid at that depth.
        // If the value returned is negative then the m_negVect access is invalid at that depth (absolute value).
        // If it returns 0 then the passed IDs are all VALID.
        // (this is done in order to not having to perform the same work twice)
        template <typename... IDS>
        requires true && ((std::convertible_to<IDS, long long>) && ...)
        long long find_firstInvalidSignedDepth(IDS const &&...ids) {
            long long depthOfInvalid = _find_DepthOfFirstInvalid(std::forward<decltype(ids)>(ids)...);
            if (depthOfInvalid > get_depth()) { return 0; }

            size_t    depthOfInvalidCPY = depthOfInvalid;
            long long idAtDepth         = 0;

            ((depthOfInvalidCPY-- && (idAtDepth = ids, true)) && ...);
            if (idAtDepth < 0) { return -depthOfInvalid; }
            else { return depthOfInvalid; }
            std::unreachable();
        };

        // TODO: Attempt to move this function inside the function above to avoid clutter
        template <typename FirstID, typename... IDs>
        requires std::convertible_to<FirstID, long long> && ((std::convertible_to<IDs, long long>) && ...)
        size_t _find_DepthOfFirstInvalid(FirstID const &&first_id, IDs const &&...ids) {
            static_assert(
                get_depth() == (sizeof...(ids) + 1),
                "Incorrect argument passing to findDepthOfFirstInvalid ... size of pack doesn't match getDepth()");

            if constexpr (SpecializationOf<T, _OV>) {
                if (test_idIsValid(first_id)) {
                    return 1 + (*this)[std::forward<decltype(first_id)>(first_id)]._find_DepthOfFirstInvalid(
                                   std::forward<decltype(ids)>(ids)...);
                }
                else { return 1; }
            }
            else if constexpr (sizeof...(ids) == 0) {
                if (test_idIsValid(first_id)) { return 2; }
                else { return 1; }
            }
            else { static_assert(false, "Incorrect argument passing to _findDepthOfFirstInvalid"); }
            std::unreachable();
        }

        bool test_idIsValid(long long id) {
            if ((static_cast<long long>(m_posVect.size()) > id) &&
                (static_cast<long long>(m_negVect.size()) > (-1 * id - 1))) {
                return true;
            }
            else { return false; }
            std::unreachable();
        };


        // ITERATORS

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using element_type      = T;
            using value_type        = T;
            using pointer           = T *;
            using reference         = T &;
            using difference_type   = std::ptrdiff_t;
        };
    };

private:
    template <size_t lvlToGo>
    consteval static auto _getContainedType_sampleInst() {

        if constexpr (lvlToGo == 1) { return _OV<Data_T>(); }
        else { return _OV<decltype(_getContainedType_sampleInst<lvlToGo - 1>())>(); }
    }

    // The member below is where all the contained data lives
    decltype(_getContainedType_sampleInst<c_numOfDimensions>()) m_data;

public:
    Omni_Vector() : m_data() {};

    template <typename First_ID, typename... Other_IDs>
    requires(std::same_as<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<First_ID>>>, long long>) &&
            ((std::same_as<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<Other_IDs>>>, long long>), ...)
    auto &operator[](First_ID const &&id, Other_IDs const &&...other_ids) {
        if constexpr (sizeof...(other_ids) == 0) { return m_data[std::forward<decltype(id)>(id)]; }
        else { return m_data[std::forward<decltype(id)>(id), std::forward<decltype(other_ids)>(other_ids)...]; }
        std::unreachable();
    }

    template <typename... IDs>
    size_t expand_ifNecessary(IDs... ids) {
        return m_data.expand_ifNecessary(std::forward<decltype(ids)>(ids)...);
    }


    consteval static size_t get_blockSize() { return c_blockSize; }
    consteval static Data_T get_defaultValue() { return c_defaultValue; }
    consteval static size_t get_numOfDimensions() { return c_numOfDimensions; }
};


/* aaa

 */
template <typename Data_T, size_t c_numOfDimensions = 1, Data_T c_defaultValue = Data_T(), size_t c_blockSize = 4>
requires std::is_trivially_copyable_v<Data_T>
class Omni_Store {
public:
    using Key_Type = typename std::array<long long, c_numOfDimensions>;

private:
    // FORWARD DECLARATIONS
    class _Chunk;

    // PRIVATE STATIC


    template <typename T, T... ints>
    consteval static size_t _c_detail_get_variadicPower(const std::integer_sequence<T, ints...>) {
        return (ints * ...);
    }

    template <typename T, T... ints>
    consteval static Key_Type _c_detail_get_cornerDefault(const std::integer_sequence<T, ints...>) {
        return Key_Type{ints...};
    }

    consteval static auto _c_detail_mds_convArrayPair() {
        std::array<std::pair<long long, long long>, c_numOfDimensions> res;
        for (int i = 0; i < c_numOfDimensions; ++i) {
            res[i].first  = i;
            res[i].second = 1;
            for (int j = 0; j < c_numOfDimensions - i - 1; ++j) { res[i].second *= c_blockSize; }
        }
        return res;
    }


    static constexpr const auto c_X_repeat_blockSize =
        (typename c_gen_X_repeat_sequence<c_numOfDimensions, c_blockSize>::type){};
    static constexpr const auto c_X_repeat_three = (typename c_gen_X_repeat_sequence<c_numOfDimensions, 3LL>::type){};
    static constexpr const auto c_X_repeat_LLONG_MIN =
        (typename c_gen_X_repeat_sequence<c_numOfDimensions, LLONG_MAX>::type){};

    static constexpr const auto      c_mds_convPairArray  = _c_detail_mds_convArrayPair();
    static constexpr const auto      c_IDs_sequence       = std::make_integer_sequence<size_t, c_numOfDimensions>{};
    static constexpr const size_t    c_chunkItemCount     = _c_detail_get_variadicPower(c_X_repeat_blockSize);
    static constexpr const size_t    c_blockOfChunksCount = _c_detail_get_variadicPower(c_X_repeat_three);
    static constexpr const long long c_blockSize_long     = c_blockSize;
    // static constexpr const _Chunk    fake_Chunk = _Chunk();

    // CHUNK NESTED TYPE DEFINITION
    class _Chunk {
    public:
        // MEMBERS
        std::array<Data_T, c_chunkItemCount>                             m_data;
        std::array<std::reference_wrapper<_Chunk>, c_blockOfChunksCount> m_refsToSurrChunks;
        bool                                                             m_refsToSurrChunkAllValid = false;

        // _DETAIL
        template <typename T, T... ints>
        constexpr auto _detail_gen_mdspan_toSelf(const std::integer_sequence<T, ints...>) {
            return Kokkos::mdspan(m_data.data(), ints...);
        }
        template <typename T, T... ints>
        constexpr auto _detail_gen_mdspan_toSurrChunks(const std::integer_sequence<T, ints...>) {
            return Kokkos::mdspan(m_refsToSurrChunks.data(), ints...);
        }

        void _setSurrValidFlag_True() { m_refsToSurrChunkAllValid = true; }

    public:
        // _DETAIL
        template <typename T, T... ints>
        consteval static auto _detail_gen_mdspan_type(const std::integer_sequence<T, ints...>) {
            std::array<Data_T, c_chunkItemCount> some_data_instance;
            return Kokkos::mdspan(some_data_instance.data(), ints...);
        }

        // CONSTRUCT
        constexpr _Chunk() : _Chunk(c_defaultValue) {};
        constexpr _Chunk(Data_T defaultDataValue) {
            m_data.fill(defaultDataValue);
            // m_refsToSurrChunks.fill(fake_Chunk);
        };

        // ACCESS
        constexpr auto gen_mdspan_toSelf() { return _detail_gen_mdspan_toSelf(c_X_repeat_blockSize); }
        constexpr auto gen_mdspan_ToSurrChunks() { return _detail_gen_mdspan_toSurrChunks(c_blockOfChunksCount); }

        bool is_refsToSurrAllValid() { return m_refsToSurrChunkAllValid; }


        template <typename... MDS_IDs, typename T, T... ints>
        std::reference_wrapper<_Chunk> get_surrChunk(MDS_IDs &&...mds_IDs, const std::integer_sequence<T, ints...>) {
            return Kokkos::mdspan(m_refsToSurrChunks.data(), ints...)[mds_IDs...];
        }
    };


    // PRIVATE DETAIL INTERNAL
    // Get reference to one Data_T instance ... the 'goal'
    template <typename T, T... ints>
    Data_T &_get_fromSelChunk(Key_Type const &key, const std::integer_sequence<T, ints...>) {
        return m_mds_to_chunkData[(key[ints] - m_selChunk_Corner[ints]) * m_selChunk_negOrPos[ints] +
                                  m_selChunk_offsetsForNeg[ints]...];
    }

    template <typename T, T... ints>
    constexpr Key_Type _get_chunkCornerFromKey(Key_Type const &key, const std::integer_sequence<T, ints...>) {
        return Key_Type{(((key[ints] + std::signbit(key[ints])) / c_blockSize_long) * c_blockSize_long) +
                        (std::signbit(key[ints]) * (-c_blockSize_long))...};
    }

    template <typename T, T... ints>
    constexpr void _update_selChunkMembers(Key_Type const &key, const std::integer_sequence<T, ints...>) {
        // Seems comlicated, but it just sets the helper variables so that access to said chunk is fast later on.
        // Most of this is done in order to 'remap' negative indices the correct way
        // Should be calculated nearly instantenously on any modern CPU
        m_selChunk_Corner        = _get_chunkCornerFromKey(key, c_IDs_sequence);
        m_selChunk_negOrPos      = Key_Type{(std::signbit(key[ints]) * (-2LL) + 1LL)...};
        m_selChunk_offsetsForNeg = Key_Type{std::signbit(key[ints]) * (-1LL)...};
    }


    template <typename... PAIRS>
    void convert_linIDtoArrOfIDs(Key_Type &arr_out, size_t const &linearID, const PAIRS &...pairs) {
        ((arr_out[(pairs).first] = linearID / (pairs).second), ...);
        arr_out[arr_out.size() - 1] = linearID % c_blockSize_long;
    }

    // 'Hard' lookup ... direct reference unavailable must lookup in 'dense map'
    template <typename T, T... ints>
    void _hardLookup(Key_Type const &key, const std::integer_sequence<T, ints...>) {

        _update_selChunkMembers(key, c_IDs_sequence);

        // BEWARE: May not insert if _Chunk with that key already exists ... in that case just returns it
        auto mpInsertResult = mp.insert({m_selChunk_Corner, _Chunk()});
        selChunk            = mpInsertResult.first->second;
        m_mds_to_chunkData  = selChunk.gen_mdspan_toSelf();

        if (not selChunk.get().is_refsToSurrAllValid()) {
            auto mds_SurrChunks = selChunk.get().gen_mdspan_ToSurrChunks();
        }
    }

    template <typename T, T... ints>
    void _softLookup(Key_Type const &key, const std::integer_sequence<T, ints...>) {
        Key_Type oldCorner = m_selChunk_Corner;
        _update_selChunkMembers(key, c_IDs_sequence);

        // First argument is Key_Type with each value in array 0, 1 or 2, based on which surrChunk ref we want
        selChunk = selChunk.get().get_surrChunk(
            Key_Type{(((m_selChunk_Corner[ints] - oldCorner[ints]) / c_blockSize) + 1)...}, c_X_repeat_three);

        if (not selChunk.get().is_refsToSurrAllValid()) {
            auto mds_SurrChunks = selChunk.get().gen_mdspan_ToSurrChunks();
        }
    }


    // MEMBERS


    // This is where all of the _Chunk data resides

public:
    // IS_ FUNCTIONS
    bool is_inSelChunk(Key_Type const &key) const {
        int invalidDims = 0;
        for (int i = 0; (i < c_numOfDimensions && invalidDims == 0); ++i) {
            invalidDims += (key[i] < m_selChunk_Corner[i]) || (key[i] >= (m_selChunk_Corner[i] + c_blockSize));
        }
        return not invalidDims;
    }
    bool is_outsideSurrChunks(Key_Type const &key) const {
        int invalidDims = 0;
        for (int i = 0; (i < c_numOfDimensions && invalidDims == 0); ++i) {
            invalidDims +=
                (key[i] < (m_selChunk_Corner[i] - c_blockSize)) || (key[i] >= (m_selChunk_Corner[i] + 2 * c_blockSize));
        }
        return not invalidDims;
    }

    // MAIN INTERFACE
    Data_T &get(Key_Type const &key) {
        if (is_outsideSurrChunks(key)) { _hardLookup(key, c_IDs_sequence); }
        else if (not is_inSelChunk(key)) { _softLookup(key, c_IDs_sequence); }
        return _get_fromSelChunk(key, c_IDs_sequence);
    }


    Key_Type m_selChunk_Corner = _c_detail_get_cornerDefault(c_X_repeat_LLONG_MIN);
    Key_Type m_selChunk_negOrPos;
    Key_Type m_selChunk_offsetsForNeg;

    std::reference_wrapper<_Chunk>                                  selChunk = _Chunk();
    decltype(_Chunk::_detail_gen_mdspan_type(c_X_repeat_blockSize)) m_mds_to_chunkData;

    robin_hood::unordered_node_map<Key_Type, _Chunk, AOC_commons::XXH3Hasher> mp;
};


} // namespace AOC2018