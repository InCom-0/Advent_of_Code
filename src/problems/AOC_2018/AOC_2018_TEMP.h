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

-

Above all the most important feature:
It knows when and how many times it is nested within itself (enabled through features of the type system AND template
metaprogramming). This enables programmatic control of the whole structure by calling functions on the top level only
(for commonly used tasks). Thereby abstractomg away most of the pesky details of managing such nested data structures
(which is typically left to the programmer to deal with 'by hand')

_

Fast access to all levels using the traditional operator[]. 'Automatic construction' of the contained data structure
below the top level. 'Automatic expansion' of the contained data structure using the 'expand_ifNecessary(IDs... ids)'
member function (usually called before accessing data item with the same 'ids...'). No push_back(), push_front(),
pop_back(), pop_front(). Expands in 'chunks' of ('blockSize' power_to #of dimensions). Can't reduce size once expanded.

_

Usage:
Pass the type to itself as a type template argument and pass the last (bottom level) Omni_Vector the data type you
need contained.
EG: 'Omni_Vector<Omni_Vector<Omni_Vector<int>>>' to declare a 3-dimensional type containing 'int' as data
type.

_

Note:
It is not intended to be used the same way or for the same purpose as std::vector<T>.
The main intended use case is when one needs to execute some sort of 'explorative' algorithm in space of arbitrary
number of dimensions and the 'exploration' happens from a central point 'one step at a time' in unpredictable directions
(including extreme cases).
Regardless, it may allocate (and copy construct) plenty of 'unexplored' and practically unused data ... but not
exponentially.


 */


template <typename Data_T, size_t c_numOfLevels = 1, Data_T c_defaultValue = Data_T(), size_t c_blockSize = 8>
requires std::copy_constructible<Data_T> && std::is_default_constructible_v<Data_T>
class Omni_Vector_TOP {

public:
    template <typename T = Data_T>
    class Omni_Vector {
    private:
        // DATA MEMBERS
        std::vector<T> m_posVect;
        std::vector<T> m_negVect;

        //  HELPERS

        // static T __hlpr_constructInstanceOfT(auto const &&data, auto const &&blockSize) {
        //     if constexpr (SpecializationOf<T, Omni_Vector>) {
        //         return T(std::forward<decltype(data)>(data), std::forward<decltype(blockSize)>(blockSize));
        //     }
        //     else if constexpr (std::is_same_v<T, std::remove_cvref_t<decltype(data)>>) {
        //         return T(std::forward<decltype(data)>(data));
        //     }
        //     else {
        //         static_assert(false, "Impossible to construct due to passed default Data_t type is not the same as
        //         the "
        //                              "'last level' type instantiation in top level constructor");
        //     }
        // };

    public:
        // CONSTRUCTION AND DESTRUCTION
        Omni_Vector() = delete;

        // Constructs all 'levels' below automatically. Contained data is default copy initialized to the value of
        // 'data'. Default data is created in each dimension: 'blockSize' number of elements in each direction. For 2
        // dimensions with blocksize 16 => (2^2=4)*(16^2) = 1'024. For 3 dimensions with blocksize 16 => (2^3=8)*(16^3)
        // = 32'768. Beware of dimensionality !!!
        template <typename Data_T_Local>
        requires std::is_same_v<Data_T_Local, Data_T>
        Omni_Vector(Data_T_Local const &&data_v = c_defaultValue) {

            if constexpr (SpecializationOf<T, Omni_Vector>) {
                m_posVect = std::vector<T>(c_blockSize, T(std::forward<decltype(data_v)>(data_v)));
                m_negVect = std::vector<T>(c_blockSize, T(std::forward<decltype(data_v)>(data_v)));
            }
            else if constexpr (std::is_same_v<T, std::remove_cvref_t<Data_T>>) {
                m_posVect = std::vector<T>(c_blockSize, std::forward<decltype(data_v)>(data_v));
                m_negVect = std::vector<T>(c_blockSize, std::forward<decltype(data_v)>(data_v));
            }
            else {
                static_assert(false, "Impossible to construct due to passed default Data_t type is not the same as the "
                                     "'last level' type instantiation in top level constructor");
            }
        }

        // CONSTEVAL STATIC
        consteval static size_t get_depth() {
            if constexpr (SpecializationOf<T, Omni_Vector>) { return T::get_depth() + 1; }
            else { return 1; }
        }

        // ACCESS
        // TODO: Would be fantastic to add iterator and ranges support

        // Acess style modelling vector
        T &operator[](long long const id) {
            if (id < 0) { return m_negVect[(-id) - 1]; }
            else { return m_posVect[id]; }
        }

        // QUERYING
        consteval size_t get_blockSize() const { return c_blockSize; }


        // template <size_t lvl, size_t blockSize>
        // consteval static auto get_defaultInstanceAtLvl() {
        //     if constexpr (lvl > 1 && lvl <= get_depth()) { return T::template get_defaultInstanceAtLvl<lvl - 1>(); }
        //     else if constexpr (lvl == 1) {
        //         if constexpr (SpecializationOf<T, Omni_Vector>) {
        //             return T(decltype(get_defaultDataTypeInstance())(), blockSize);
        //         }
        //         else if constexpr (std::is_default_constructible_v<T>) { return T(); }
        //         else {
        //             static_assert(false,
        //                           "Contained data type (that is the bottom level type) is not
        //                           default-constructible");
        //         }
        //     }
        //     else {
        //         static_assert(false, "size_t template argument provided to 'get_defaultInstanceAtLvl()' is greater
        //         than "
        //                              "get_depth() or is 0");
        //     }
        // }
        // template <size_t blockSize>
        // consteval static auto get_defaultDataTypeInstance() {
        //     return get_defaultInstanceAtLvl<get_depth(), blockSize>();
        // }

        // EXPANSION

        // This will expand recursivelly (happens when higher levels need to be expanded)
        // Returns how many times it performed expansion. If no expansion took place returns 0.
        template <typename... IDs>
        size_t expand_ifNecessary(IDs... ids) {
            static_assert(
                get_depth() == (sizeof...(ids)),
                "Incorrect argument passing to __findDepthOfFirstInvalid ... size of pack doesn't match getDepth()");

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

            if constexpr (SpecializationOf<T, Omni_Vector>) {
                if (std::abs(signedLvlToExpand) != 1) {
                    // The whole 'block size' number of elements in which idx_onThisLevel lies needs to be called with
                    // 'expand_atSignedLvl'
                    long long fromID =
                        (std::abs(idx_onThisLevel + (idx_onThisLevel < 0 ? 1 : 0)) / m_blockSize) * m_blockSize;
                    long long untilBeforeID = fromID + m_blockSize;
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
                        for (int i = 0; i < this->m_blockSize; ++i) {
                            this->m_negVect.push_back(this->m_defaultTInstance);
                        }
                    }
                    else {
                        for (int i = 0; i < this->m_blockSize; ++i) {
                            this->m_posVect.push_back(this->m_defaultTInstance);
                        }
                    }
                }
            }

            else if constexpr (sizeof...(idxs_onOtherLevels) == 0) {
                if (signedLvlToExpand < 0) {
                    for (int i = 0; i < this->m_blockSize; ++i) { this->m_negVect.push_back(this->m_defaultTInstance); }
                }
                else {
                    for (int i = 0; i < this->m_blockSize; ++i) { this->m_posVect.push_back(this->m_defaultTInstance); }
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

        template <typename FirstID, typename... IDs>
        requires std::convertible_to<FirstID, long long> && ((std::convertible_to<IDs, long long>) && ...)
        size_t __find_DepthOfFirstInvalid(FirstID const &&first_id, IDs const &&...ids) {
            static_assert(
                get_depth() == (sizeof...(ids) + 1),
                "Incorrect argument passing to __findDepthOfFirstInvalid ... size of pack doesn't match getDepth()");

            if constexpr (SpecializationOf<T, Omni_Vector>) {
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

        // template <size_t c_depthToCreate>
        // requires(c_depthToCreate > 0)
        // consteval static auto get_self(T &t) {
        //     if constexpr (c_depthToCreate == 1) { return Omni_Vector<T>(std::forward<decltype(t)>(t), 1); }
        //     else { return Omni_Vector<decltype(Omni_Vector<T>::get_self<c_depthToCreate - 1>(t))>(t, 1); }
        // }
    };


    Omni_Vector_TOP(Data_T &t) {};

private:
    template <size_t lvlToGo>
    consteval static auto __getContainedType_inst() {

        if constexpr (lvlToGo == 1) { return Omni_Vector<Data_T>(c_defaultValue); }

        else { return Omni_Vector<decltype(__getContainedType_inst<lvlToGo - 1>())>(); }
    }

    decltype(__getContainedType_inst<c_numOfLevels>()) omm;
};

} // namespace AOC2018