#include <cmath>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;

long long day7_1(std::string dataFile) {
    using namespace std::literals;
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    using LINE_t = std::pair<size_t, std::vector<size_t>>;

    std::vector<LINE_t> parsed;
    for (auto const &strLine : input) {
        parsed.push_back(LINE_t(std::stoull(strLine.front()), std::vector<size_t>()));
        for (auto beg = strLine.begin() + 1; beg < strLine.end(); ++beg) {
            parsed.back().second.push_back(std::stoull(*beg));
        }
    }


    auto flt = std::views::filter(parsed, [](auto const &a) {
        size_t lastSecID = a.second.size() - 1;
        size_t currSecID = 1;
        size_t curVal    = a.second[0];

        auto rec_solver = [&](this auto &self) -> bool {
            if (currSecID < lastSecID) {
                size_t lastVal  = curVal;
                curVal         += a.second[currSecID++];
                if (self()) { return true; };

                curVal  = lastVal;
                curVal *= a.second[currSecID - 1];
                if (self()) { return true; };
                curVal = lastVal;
                currSecID--;
                return false;
            }
            else {
                size_t lastVal  = curVal;
                curVal         += a.second[currSecID++];
                if (curVal == a.first) { return true; };

                curVal  = lastVal;
                curVal *= a.second[currSecID - 1];
                if (curVal == a.first) { return true; };
                curVal = lastVal;
                currSecID--;
                return false;
            }
        };

        return rec_solver();
    });

    auto res = std::ranges::fold_left(flt, 0uz, [](auto &&ini, auto const &a) { return ini + a.first; });

    return res;
}

long long day7_2(std::string dataFile) {
    using namespace std::literals;
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    using LINE_t = std::pair<size_t, std::vector<size_t>>;

    std::vector<LINE_t> parsed;
    for (auto const &strLine : input) {
        parsed.push_back(LINE_t(std::stoull(strLine.front()), std::vector<size_t>()));
        for (auto beg = strLine.begin() + 1; beg < strLine.end(); ++beg) {
            parsed.back().second.push_back(std::stoull(*beg));
        }
    }


    auto flt = std::views::filter(parsed, [](auto const &a) {
        size_t lastSecID = a.second.size() - 1;
        size_t currSecID = 1;
        size_t curVal    = a.second[0];

        auto rec_solver = [&](this auto &self) -> bool {
            if (currSecID < lastSecID) {
                size_t lastVal  = curVal;
                curVal         *= std::pow(10, std::trunc(log10(a.second[currSecID])) + 1);
                curVal         += a.second[currSecID++];

                if (self()) { return true; };

                curVal  = lastVal;
                curVal += a.second[currSecID - 1];
                if (self()) { return true; };

                curVal  = lastVal;
                curVal *= a.second[currSecID - 1];
                if (self()) { return true; };
                curVal = lastVal;
                currSecID--;
                return false;
            }
            else {
                size_t lastVal  = curVal;
                curVal         *= std::pow(10, std::trunc(log10(a.second[currSecID])) + 1);
                curVal         += a.second[currSecID++];

                if (curVal == a.first) { return true; };

                curVal  = lastVal;
                curVal += a.second[currSecID - 1];
                if (curVal == a.first) { return true; };

                curVal  = lastVal;
                curVal *= a.second[currSecID - 1];
                if (curVal == a.first) { return true; };
                curVal = lastVal;
                currSecID--;
                return false;
            }
        };

        return rec_solver();
    });

    auto res = std::ranges::fold_left(flt, 0uz, [](auto &&ini, auto const &a) { return ini + a.first; });

    return res;
}
} // namespace AOC2024