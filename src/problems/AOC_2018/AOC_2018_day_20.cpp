#include <omni.h>
#include <incom_commons.h>


#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <mdspan/mdspan.hpp>


namespace AOC2018 {


long long day20_1(std::string dataFile) {


    std::array<long long, 3> rr{0, 0, 0};


    incom::omni::Omni_Store<char, 3, '_', 4> os2;

    for (int i = 0; i < 12; ++i) {
        char &zz = os2.get(rr) ;
        zz = 'X';

        rr[0]--;
        rr[1]--;
        rr[2]--;
    }


    return -999;
}

long long day21_1(std::string dataFile) {


    return -999;
}
} // namespace AOC2018