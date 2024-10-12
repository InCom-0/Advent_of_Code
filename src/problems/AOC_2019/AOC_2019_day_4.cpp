#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <string>


namespace AOC2019 {
long long day4_1(std::string dataFile) {

    auto const d_ctre = ctre::search<R"(\d+)">;

    auto const str_input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::string const str_start = str_input.front().front();
    std::string const str_end   = str_input.front().back();

    long long const range_start = std::stoll(str_start);
    long long const range_end   = std::stoll(str_end);

    long long lvl      = 0;
    long long accu     = 0;
    long long sameNext = 0;


    auto recur = [&](this auto &self, int const &previousLvlVal, bool const startDigit, bool const endDigit) {
        if (lvl == str_end.size()) {
            accu += (sameNext != 0);
            return;
        }
        for (int lvlVal = (std::max((startDigit ? int(str_start[lvl] - 48) : int(0)), previousLvlVal));
             lvlVal <= (endDigit ? str_end[lvl] - 48 : 9); ++lvlVal) {

            bool sd = (lvlVal == (str_start[lvl] - 48)) && startDigit;
            bool ed = (lvlVal == (str_end[lvl] - 48)) && endDigit;

            sameNext += (previousLvlVal == lvlVal);
            lvl++;
            self(lvlVal, sd, ed);
            lvl--;
            sameNext -= (previousLvlVal == lvlVal);
        }
    };

    int prevLevInit = 0;
    recur(prevLevInit, true, true);


    return accu;
}

long long day4_2(std::string dataFile) {

    auto const d_ctre = ctre::search<R"(\d+)">;

    auto const str_input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::string const str_start = str_input.front().front();
    std::string const str_end   = str_input.front().back();

    long long const range_start = std::stoll(str_start);
    long long const range_end   = std::stoll(str_end);

    long long lvl      = 0;
    long long accu     = 0;
    long long sameNext = 0;
    long long doubles  = 0;


    auto recur = [&](this auto &self, int const &previousLvlVal, bool const startDigit, bool const endDigit) {
        if (lvl == str_end.size()) {
            accu += (doubles != 0);
            return;
        }
        for (int lvlVal = (std::max((startDigit ? int(str_start[lvl] - 48) : int(0)), previousLvlVal));
             lvlVal <= (endDigit ? str_end[lvl] - 48 : 9); ++lvlVal) {

            bool sd = (lvlVal == (str_start[lvl] - 48)) && startDigit;
            bool ed = (lvlVal == (str_end[lvl] - 48)) && endDigit;

            long long const old_sameNext  = sameNext;
            sameNext               += (previousLvlVal == lvlVal);
            sameNext               *= (previousLvlVal == lvlVal);

            doubles += (sameNext == 1);
            doubles -= (sameNext == 2);

            lvl++;
            self(lvlVal, sd, ed);
            lvl--;

            doubles -= (sameNext == 1);
            doubles += (sameNext == 2);

            sameNext = old_sameNext;
        }
    };

    int prevLevInit = 0;
    recur(prevLevInit, true, true);


    return accu;
}
} // namespace AOC2019