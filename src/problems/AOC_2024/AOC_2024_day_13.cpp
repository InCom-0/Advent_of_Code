#include <cmath>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>


namespace AOC2024 {
namespace incc = incom::commons;

long long day13_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre);

    size_t accu = 0;
    for (int i = 0; i < input.size(); i += 4) {
        std::pair<double, double> button_A{std::stod(input[i].front()), std::stod(input[i].back())};
        std::pair<double, double> button_B{std::stod(input[i + 1].front()), std::stod(input[i + 1].back())};
        std::pair<double, double> target{std::stod(input[i + 2].front()), std::stod(input[i + 2].back())};
        double                    buttonATarget = (target.first * button_B.second - button_B.first * target.second) /
                               (button_A.first * button_B.second - button_A.second * button_B.first);
        double buttonBTarget = (target.first - buttonATarget * button_A.first) / button_B.first;
        if (buttonATarget >= 0 && buttonATarget < 101 && buttonBTarget >= 0 && buttonBTarget < 101 &&
            static_cast<double>(std::trunc(buttonATarget)) == buttonATarget &&
            static_cast<double>(std::trunc(buttonBTarget)) == buttonBTarget) {
            accu += 3 * buttonATarget;
            accu += buttonBTarget;
        }
    }
    return accu;
}

long long day13_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre);

    size_t accu = 0;
    for (int i = 0; i < input.size(); i += 4) {
        std::pair<double, double> button_A{std::stod(input[i].front()), std::stod(input[i].back())};
        std::pair<double, double> button_B{std::stod(input[i + 1].front()), std::stod(input[i + 1].back())};
        std::pair<double, double> target{std::stod(input[i + 2].front()), std::stod(input[i + 2].back())};
        target.first         += 10'000'000'000'000;
        target.second        += 10'000'000'000'000;
        double buttonATarget  = (target.first * button_B.second - button_B.first * target.second) /
                               (button_A.first * button_B.second - button_A.second * button_B.first);
        double buttonBTarget = (target.first - buttonATarget * button_A.first) / button_B.first;
        if (buttonATarget >= 0 && buttonBTarget >= 0 &&
            static_cast<double>(std::trunc(buttonATarget)) == buttonATarget &&
            static_cast<double>(std::trunc(buttonBTarget)) == buttonBTarget) {
            accu += 3 * buttonATarget;
            accu += buttonBTarget;
        }
    }
    return accu;
}
} // namespace AOC2024