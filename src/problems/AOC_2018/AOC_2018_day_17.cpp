#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2018 {

struct base_test {
    int a;
    base_test(int aa) : a{aa} {};
    virtual std::vector<int> v_func() = 0;
};

struct derived_test : base_test {
    using base_test::base_test;
    std::vector<int> v_func() override { return {0, 1, 2}; }
};


int day99_1() {
    derived_test alpha(5);

    return -999;
}
} // namespace AOC2018