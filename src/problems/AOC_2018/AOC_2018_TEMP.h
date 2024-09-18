#pragma once

#include <ctre.hpp>
#include <flux.hpp>
#include <more_concepts/more_concepts.hpp>
#include <type_traits>
#include <xxhash.h>


namespace AOC2018 {

template <typename T, template <typename...> typename Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> typename Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};
template <typename T, template <typename...> typename Template>
concept SpecializationOf = is_specialization_of<T, Template>::value;


/* 'Omni Vector' container type

Omni => The vector goes both ways ... that is you can index into 'Omni_Vector' with negative indices.
Omni => Made to nativelly supports N dimensions aka: nesting (as in eg. 'vector<vector<int>>' is 2-dimensional)
By no means 'industrial grade'

-

Above all the most important feature:
There is an internal type (__OV) that the Omni Vector uses and that supports templated nesting within itself.
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

Mostly created as an excercise into compile time (meta)programming and usage of the various more modern features of the C++ typesystem.


 */


template <typename Data_T, size_t c_numOfDimensions = 1, Data_T c_defaultValue = Data_T(), size_t c_blockSize = 8>
requires std::copy_constructible<Data_T> && std::is_default_constructible_v<Data_T>
class Omni_Vector {

private:
    template <typename T>
    class __OV {
    private:
        // DATA MEMBERS
        std::vector<T> m_posVect;
        std::vector<T> m_negVect;

    public:
        // CONSTRUCTION AND DESTRUCTION

        // Constructs all 'levels' below automatically. Contained data is default copy initialized to the value of
        // 'data_v'. Default data is created in each dimension: 'blockSize' number of elements in each direction. For 2
        // dimensions with blocksize 16 => (2^2=4)*(16^2) = 1'024. For 3 dimensions with blocksize 16 => (2^3=8)*(16^3)
        // = 32'768. Beware of dimensionality !!!
        __OV() {
            if constexpr (SpecializationOf<T, __OV>) {
                m_posVect = std::vector<T>(c_blockSize, T());
                m_negVect = std::vector<T>(c_blockSize, T());
            }
            else if constexpr (std::is_same_v<T, std::remove_cvref_t<Data_T>>) {
                m_posVect = std::vector<T>(c_blockSize, c_defaultValue);
                m_negVect = std::vector<T>(c_blockSize, c_defaultValue);
            }
            else {
                static_assert(false, "Impossible to construct due to passed default Data_t type is not the same as the "
                                     "'last level' type instantiation in top level constructor");
            }
        }

        // ACCESS
        // TODO: Would be fantastic to add iterator and ranges support

        // Acess style modelling vector
        T &operator[](long long const id) {
            if (id < 0) { return m_negVect[(-id) - 1]; }
            else { return m_posVect[id]; }
        }

        // CONSTEVAL STATIC QUERYING
        consteval static size_t get_blockSize() { return c_blockSize; }
        consteval static Data_T get_defaultValue() { return c_defaultValue; }
        consteval static size_t get_depthFromTop() { return c_numOfDimensions; }

        consteval static size_t get_depth() {
            if constexpr (SpecializationOf<T, __OV>) { return T::get_depth() + 1; }
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
            static_assert(
                get_depth() == (sizeof...(idxs_onOtherLevels) + 1),
                "Incorrect argument passing to 'expand_atSignedLvl' ... (size of pack + 1) doesn't match getDepth()");

            // Specialized using if constexpr
            // This must be done, because one can only call 'expand_atSignedLvl' when T is a specialization of
            // Omni_Vector

            if constexpr (SpecializationOf<T, __OV>) {
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
                        for (int i = 0; i < c_blockSize; ++i) { this->m_negVect.push_back(typename T::__OV()); }
                    }
                    else {
                        for (int i = 0; i < c_blockSize; ++i) { this->m_posVect.push_back(typename T::__OV()); }
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
                static_assert(false,
                              "Incorrect argument passing to expand_atSignedLvl ... T is neither Omni_Vector nor is "
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
            long long depthOfInvalid = __find_DepthOfFirstInvalid(std::forward<decltype(ids)>(ids)...);
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
        size_t __find_DepthOfFirstInvalid(FirstID const &&first_id, IDs const &&...ids) {
            static_assert(
                get_depth() == (sizeof...(ids) + 1),
                "Incorrect argument passing to __findDepthOfFirstInvalid ... size of pack doesn't match getDepth()");

            if constexpr (SpecializationOf<T, __OV>) {
                if (test_idIsValid(first_id)) {
                    return 1 + (*this)[std::forward<decltype(first_id)>(first_id)].__find_DepthOfFirstInvalid(
                                   std::forward<decltype(ids)>(ids)...);
                }
                else { return 1; }
            }
            else if constexpr (sizeof...(ids) == 0) {
                if (test_idIsValid(first_id)) { return 2; }
                else { return 1; }
            }
            else { static_assert(false, "Incorrect argument passing to __findDepthOfFirstInvalid"); }
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
    consteval static auto __getContainedType_sampleInst() {

        if constexpr (lvlToGo == 1) { return __OV<Data_T>(); }
        else { return __OV<decltype(__getContainedType_sampleInst<lvlToGo - 1>())>(); }
    }

    // The member below is where all the contained data lives
    decltype(__getContainedType_sampleInst<c_numOfDimensions>()) m_data;

public:
    Omni_Vector() : m_data() {};

    auto &operator[](long long const id) { return m_data[id]; }

    template <typename... IDs>
    size_t expand_ifNecessary(IDs... ids) {
        return m_data.expand_ifNecessary(std::forward<decltype(ids)>(ids)...);
    }


    consteval static size_t get_blockSize() { return c_blockSize; }
    consteval static Data_T get_defaultValue() { return c_defaultValue; }
    consteval static size_t get_numOfDimensions() { return c_numOfDimensions; }
};


} // namespace AOC2018