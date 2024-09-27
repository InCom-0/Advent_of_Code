#include <incom_commons.h>
#include <omni.h>


#include <ankerl/unordered_dense.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <mdspan/mdspan.hpp>
#include <stack>


namespace AOC2018 {


std::pair<long long, long long> day20_1(std::string dataFile) {

    auto any_ctre = ctre::search<R"(.+)">;

    auto inputString = incom::commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front().front();

    std::string                              inp(inputString.begin(), --inputString.end());
    incom::omni::Omni_Store<char, 2, '#', 8> os1;
    decltype(os1)::Key_Type                  loc_key{0, 0};
    size_t                                   cursor                = 0;
    size_t                                   curDoorCount          = 0;
    size_t                                   maxDoorCount          = 0;
    size_t                                   roomCountDoorsOver999 = 0;

    std::vector<decltype(loc_key)> dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    ankerl::unordered_dense::map<decltype(loc_key), size_t, incom::commons::XXH3Hasher> roomDistanceMaps;

    std::stack<std::pair<decltype(loc_key), size_t>> stackOfAltEnds;


    auto placeDoorChar = [&](decltype(loc_key) const &stepDir, char &&chr) {
        loc_key[0] += stepDir[0];
        loc_key[1] += stepDir[1];

        char &loc_charInMapRef = os1.get(loc_key);
        loc_charInMapRef       = chr;
    };

    auto placeRoomChar = [&](decltype(loc_key) const &stepDir, char &&chr) {
        loc_key[0] += stepDir[0];
        loc_key[1] += stepDir[1];

        char &loc_charInMapRef = os1.get(loc_key);
        if (loc_charInMapRef != chr) {
            loc_charInMapRef = chr;
            curDoorCount++;
            if (curDoorCount > 999) { roomCountDoorsOver999++; }
            maxDoorCount = std::max(maxDoorCount, curDoorCount);
        }
    };


    auto exe_Descent = [&](this auto &self) -> void {
        while (cursor < inp.size()) {
            cursor++;
            char &charInRegex = inp[cursor];

            if (charInRegex == '(') { stackOfAltEnds.push({loc_key, curDoorCount}); }
            else if (charInRegex == ')') {
                curDoorCount = stackOfAltEnds.top().second;
                loc_key      = stackOfAltEnds.top().first;
                stackOfAltEnds.pop();
            }
            else if (charInRegex == '|') {
                curDoorCount = stackOfAltEnds.top().second;
                loc_key      = stackOfAltEnds.top().first;
            }
            else {
                if (charInRegex == 'N') {
                    placeDoorChar(dirs[0], '|');
                    placeRoomChar(dirs[0], '.');
                }
                else if (charInRegex == 'E') {
                    placeDoorChar(dirs[1], '-');
                    placeRoomChar(dirs[1], '.');
                }
                else if (charInRegex == 'S') {
                    placeDoorChar(dirs[2], '|');
                    placeRoomChar(dirs[2], '.');
                }
                else if (charInRegex == 'W') {
                    placeDoorChar(dirs[3], '-');
                    placeRoomChar(dirs[3], '.');
                }
                else {}
            }
        }
    };

    exe_Descent();


    return std::make_pair(maxDoorCount, roomCountDoorsOver999);
}

long long day21_1(std::string dataFile) {


    return -999;
}
} // namespace AOC2018