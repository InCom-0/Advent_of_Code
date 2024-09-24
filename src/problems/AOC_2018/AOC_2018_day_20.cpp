#include "AOC_2018_TEMP.h"
#include <AOC_commons.h>


#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <mdspan/mdspan.hpp>


namespace AOC2018 {


long long day20_1(std::string dataFile) {


    std::array<long long, 3> rr{0, 0, 0};


    std::array<long long, 8> arrr {1,2,3,4,5,6,7,8};
    auto mds = Kokkos::mdspan(arrr.data(), 2,2,2);


    std::cout << mds [0,0,0] << '\n';
    std::cout << mds [0,1,1] << '\n';
    std::cout << mds [1,0,0] << '\n';
    std::cout << mds [1,1,1] << '\n';


    Omni_Store<char, 3, '_', 4> os2;




    return -999;
}

long long day21_1(std::string dataFile) {


    return -999;
}
} // namespace AOC2018