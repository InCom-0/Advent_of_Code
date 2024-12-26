#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;
long long day21_1(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto input    = incc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    return -999;
}

long long day21_2(std::string dataFile) {

    return -999;
}
} // namespace AOC2024