#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.h>


namespace AOC2025 {
size_t
day6_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"([^\s]+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    size_t accu = 0uz;
    for (size_t id = 0;
         auto const &[a, b, c, d] : std::views::zip(input.at(0), input.at(1), input.at(2), input.at(3))) {
        if (input.back().at(id).front() == '+') {
            accu += (std::stoll(a) + std::stoll(b) + std::stoll(c) + std::stoll(d));
        }
        else if (input.back().at(id).front() == '*') {
            accu += (std::stoll(a) * std::stoll(b) * std::stoll(c) * std::stoll(d));
        }
        else { assert(false); }
        id++;
    }

    return accu;
}

size_t
day6_2(std::string dataFile) {
    auto notSpace_ctre = ctre::search<R"([^\s]+)">;
    auto any_ctre      = ctre::search<R"(.+)">;
    auto input         = incom::commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    std::vector<std::string> data(input.front().size(), std::string{});

    // Essentially transpose the input so that we can use other std facilities
    for (int col = 0; col < input.front().size(); col++) {
        for (int row = 0; row < input.size(); ++row) { data.at(col).push_back(input.at(row).at(col)); }
    }

    size_t accu = 0uz;
    for (size_t lineID = 0uz; lineID < data.size(); lineID++) {
        if (data.at(lineID).front() == ' ' && data.at(lineID).at(data.at(lineID).size() - 2) == ' ') { continue; }

        char const oper = data.at(lineID).back();
        data.at(lineID).pop_back();

        std::vector<long long> values{};
        while (lineID < data.size() &&
               (not std::ranges::all_of(data.at(lineID), [](char oneChr) { return oneChr == ' '; }))) {
            values.push_back(std::stoll(data.at(lineID++)));
        }

        if (oper == '+') { accu += std::ranges::fold_left_first(values, std::plus{}).value_or(0); }
        else if (oper == '*') { accu += std::ranges::fold_left_first(values, std::multiplies{}).value_or(0); }
        else { assert(false); }
    }


    return accu;
}
} // namespace AOC2025