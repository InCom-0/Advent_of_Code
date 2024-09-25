#include <fstream>

#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
int day16_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string                   oneStr;
    std::vector<std::vector<int>> allAunts;

    while (std::getline(iStream, oneStr)) {
        std::vector<int> oneAunt;

        auto bg = oneStr.begin();
        auto re = ctre::search<R"((?<=children: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=cats: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }


        re = ctre::search<R"((?<=samoyeds: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=pomeranians: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=akitas: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=vizslas: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }


        re = ctre::search<R"((?<=goldfish: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=trees: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=cars: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=perfumes: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        allAunts.push_back(oneAunt);
    }
    std::vector<int> toFind{3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

    auto fref = flux::ref(allAunts);
    auto ras  = flux::filter(fref, [&](auto &a) {
        if (a[1] != toFind[1] && a[1] != INT_MIN) { return false; }
        if (a[7] != toFind[7] && a[7] != INT_MIN) { return false; }
        if (a[3] != toFind[3] && a[3] != INT_MIN) { return false; }
        if (a[6] != toFind[6] && a[6] != INT_MIN) { return false; }
        if (a[0] != toFind[0] && a[0] != INT_MIN) { return false; }
        if (a[2] != toFind[2] && a[2] != INT_MIN) { return false; }
        if (a[4] != toFind[4] && a[4] != INT_MIN) { return false; }
        if (a[5] != toFind[5] && a[5] != INT_MIN) { return false; }
        if (a[8] != toFind[8] && a[8] != INT_MIN) { return false; }
        if (a[9] != toFind[9] && a[9] != INT_MIN) { return false; }
        return true;
    });

    return std::find(allAunts.begin(), allAunts.end(), flux::read_at(ras, ras.first())) - allAunts.begin() + 1;
}

int day16_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string                   oneStr;
    std::vector<std::vector<int>> allAunts;

    while (std::getline(iStream, oneStr)) {
        std::vector<int> oneAunt;

        auto bg = oneStr.begin();
        auto re = ctre::search<R"((?<=children: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=cats: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }


        re = ctre::search<R"((?<=samoyeds: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=pomeranians: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=akitas: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=vizslas: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }


        re = ctre::search<R"((?<=goldfish: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=trees: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=cars: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        re = ctre::search<R"((?<=perfumes: )\d+)">(bg, oneStr.end());
        if (bool(re)) { oneAunt.push_back(re.to_number()); }
        else { oneAunt.push_back(INT_MIN); }

        allAunts.push_back(oneAunt);
    }
    std::vector<int> toFind{3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

    auto fref = flux::ref(allAunts);
    auto ras  = flux::filter(fref, [&](auto &a) {
        if (a[1] <= toFind[1] && a[1] != INT_MIN) { return false; }
        if (a[7] <= toFind[7] && a[7] != INT_MIN) { return false; }
        if (a[3] >= toFind[3] && a[3] != INT_MIN) { return false; }
        if (a[6] >= toFind[6] && a[6] != INT_MIN) { return false; }
        if (a[0] != toFind[0] && a[0] != INT_MIN) { return false; }
        if (a[2] != toFind[2] && a[2] != INT_MIN) { return false; }
        if (a[4] != toFind[4] && a[4] != INT_MIN) { return false; }
        if (a[5] != toFind[5] && a[5] != INT_MIN) { return false; }
        if (a[8] != toFind[8] && a[8] != INT_MIN) { return false; }
        if (a[9] != toFind[9] && a[9] != INT_MIN) { return false; }
        return true;
    });

    return std::find(allAunts.begin(), allAunts.end(), flux::read_at(ras, ras.first())) - allAunts.begin() + 1;
}

} // namespace AOC2015