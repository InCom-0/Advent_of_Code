#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2018 {

template <typename T, template <typename...> typename Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> typename Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};
template <typename T, template <typename...> typename Template>
concept SpecializationOf = is_specialization_of<T, Template>::value;

/* Multi-Dimensional Omni Vector container type

Multi-Dimensional => Supports arbitrary number of dimensions (as in eg. 'vector<vector<int>>' is 2-dimensional)

Omni => Each 'dimension' goes both ways ... that is you can index into 'MD_Omni_Vector' with negative indices.

_

Fast access to all levels using the traditional operator[]. 'Automatic construction' of the contained data structure
below the top level. 'Automatic expansion' of the contained data structure using the 'expand_ifNecessary(IDs... ids)'
member function (usually called before accessing data item with the same 'ids...'). No push_back(), push_front(),
pop_back(), pop_front(). Expands in 'chunks' of ('blockSize' power_to #of dimensions). Can't reduce size once expanded.

_

Usage:
Pass the type to itself as a type template argument and pass the last (bottom level) MD_Omni_Vector the data type you
need contained.
EG: 'MD_Omni_Vector<MD_Omni_Vector<MD_Omni_Vector<int>>>' to declare a 3-dimensional type containing 'int' as data
type.

_

Note:
It is not intended to be used the same way or for the same purpose as std::vector<T>.
The main intended use case is when one needs to execute some sort of 'explorative' algorithm in space of arbitrary
number of dimensions and the 'exploration' happens from a central point 'one step at a time' in unpredictable directions
(including extreme cases).
Regardless, it may allocate (and default construct) plenty of 'unexplored' and practically unused data ... but not exponentially

 */
template <typename T>
requires std::copy_constructible<T>
class MD_Omni_Vector {
private:
    // DATA MEMBERS
    size_t         m_blockSize;
    T              m_defaultTInstance;
    std::vector<T> m_posVect;
    std::vector<T> m_negVect;

    //  HELPERS
    static T __hlpr_constructInstanceOfT(auto const &&data, auto const &&blockSize) {
        if constexpr (SpecializationOf<T, MD_Omni_Vector>) {
            return T(std::forward<decltype(data)>(data), std::forward<decltype(blockSize)>(blockSize));
        }
        else if constexpr (std::is_same_v<T, std::remove_cvref_t<decltype(data)>>) {
            return T(std::forward<decltype(data)>(data));
        }
        else {
            static_assert(false, "Impossible to construct due to passed default Data_t type is not the same as the "
                                 "'last level' type instantiation in top level constructor");
        }
    };

public:
    // CONSTRUCTION AND DESTRUCTION
    MD_Omni_Vector() = delete;

    // Constructs all 'levels' below automatically. Contained data is default copy initialized to the value of 'data'.
    // Default data is created in each dimension: 'blockSize' number of elements in each direction.
    // For 2 dimensions with blocksize 16 => (2^2=4)*(16^2) = 1'024.
    // For 3 dimensions with blocksize 16 => (2^3=8)*(16^3) = 32'768.
    // Beware of dimensionality !!!
    template <typename Data_t, typename BlockSize_t>
    requires std::is_convertible_v<BlockSize_t, size_t> && std::copy_constructible<Data_t>
    MD_Omni_Vector(Data_t const &&data, BlockSize_t const &&blockSize)
        : m_defaultTInstance(__hlpr_constructInstanceOfT(std::forward<decltype(data)>(data),
                                                         std::forward<decltype(blockSize)>(blockSize))) {

        m_blockSize = blockSize;

        if constexpr (SpecializationOf<T, MD_Omni_Vector>) {
            m_posVect = std::vector<T>(
                blockSize, T(std::forward<decltype(data)>(data), std::forward<decltype(blockSize)>(blockSize)));
            m_negVect = std::vector<T>(
                blockSize, T(std::forward<decltype(data)>(data), std::forward<decltype(blockSize)>(blockSize)));
        }
        else if constexpr (std::is_same_v<T, std::remove_cvref_t<Data_t>>) {
            m_posVect = std::vector<T>(blockSize, std::forward<decltype(data)>(data));
            m_negVect = std::vector<T>(blockSize, std::forward<decltype(data)>(data));
        }
        else {
            static_assert(false, "Impossible to construct due to passed default Data_t type is not the same as the "
                                 "'last level' type instantiation in top level constructor");
        }
    }

    // CONSTEVAL STATIC
    consteval static size_t get_depth() {
        if constexpr (SpecializationOf<T, MD_Omni_Vector>) { return T::get_depth() + 1; }
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
    constexpr size_t get_blockSize() const { return (m_blockSize); }

    template <size_t lvl>
    auto get_defaultInstanceAtLvl() {
        if constexpr (lvl > 1 && lvl <= get_depth()) {
            return m_defaultTInstance.template get_defaultInstanceAtLvl<lvl - 1>();
        }
        else if constexpr (lvl == 1) { return m_defaultTInstance; }
        else {
            static_assert(false, "size_t template argument provided to 'get_defaultInstanceAtLvl()' is greater than "
                                 "get_depth() or is 0");
        }
    }

    auto get_defaultDataTypeInstance() { return get_defaultInstanceAtLvl<get_depth()>(); }

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
    void expand_atSignedLvl(long long signedLvlToExpand, long long idx_onThisLevel, OtherIDs... idxs_onOtherLevels) {
        static_assert(
            get_depth() == (sizeof...(idxs_onOtherLevels) + 1),
            "Incorrect argument passing to 'expand_atSignedLvl' ... (size of pack + 1) doesn't match getDepth()");

        // Specialized using if constexpr
        // This must be done, because one can only call 'expand_atSignedLvl' when T is a specialization of
        // MD_Omni_Vector

        if constexpr (SpecializationOf<T, MD_Omni_Vector>) {
            if (std::abs(signedLvlToExpand) != 1) {
                // The whole 'block size' number of elements in which idx_onThisLevel lies needs to be called with
                // 'expand_atSignedLvl'
                long long fromID        = (std::abs(idx_onThisLevel) / m_blockSize) * m_blockSize;
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
                    for (int i = 0; i < this->m_blockSize; ++i) { this->m_negVect.push_back(this->m_defaultTInstance); }
                }
                else {
                    for (int i = 0; i < this->m_blockSize; ++i) { this->m_posVect.push_back(this->m_defaultTInstance); }
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
                          "Incorrect argument passing to expand_atSignedLvl ... T is neither MD_Omni_Vector nor is "
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

        if constexpr (SpecializationOf<T, MD_Omni_Vector>) {
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
        if ((static_cast<long long>(m_posVect.size()) > id) && (static_cast<long long>(m_negVect.size()) > (-1 * id))) {
            return true;
        }
        else { return false; }
        std::unreachable();
    };
};


long long day20_1(std::string dataFile) {

    MD_Omni_Vector<MD_Omni_Vector<MD_Omni_Vector<char>>> ts('X', 16);

    long long test_1 = ts.find_firstInvalidSignedDepth(1, -18, 10);

    auto defInst  = ts.get_defaultInstanceAtLvl<3>();
    auto defInst2 = ts.get_defaultInstanceAtLvl<2>();

    ts.expand_ifNecessary(5, -89, 17);


    std::cout << ts.get_depth() << '\n';


    return -999;
}

long long day21_1(std::string dataFile) {


    return -999;
}
} // namespace AOC2018