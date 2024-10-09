#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>


namespace AOC2019 {
long long day1_1(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    auto input = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre);

    std::vector<long long> data;
    for (auto const &line : input.front()) { data.push_back(std::stoll(line)); }

    long long accu = 0;
    for (auto const &num : data) {
        accu += (num/3) - 2;
    }

    return accu;
}

long long day1_2(std::string dataFile) {

    auto d_ctre = ctre::search<R"(\d+)">;
    auto input = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre);

    std::vector<long long> data;
    for (auto const &line : input.front()) { data.push_back(std::stoll(line)); }

    long long accu = 0;
    for (auto const &num : data) {
        long long rawAdd = (num/3) - 2;
        long long fuel4FuelAdd = rawAdd;
        while ((fuel4FuelAdd = (fuel4FuelAdd/3) - 2) > 0) {
            rawAdd += fuel4FuelAdd;
        }
        accu += rawAdd;
    }

    return accu;
}
} // namespace AOC2019