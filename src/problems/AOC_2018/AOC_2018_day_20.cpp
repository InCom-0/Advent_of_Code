#include <incom_omni.h>

#include <ctre.hpp>
#include <stack>


namespace AOC2018 {


std::pair<long long, long long> day20_1(std::string dataFile) {

    // DATA PREP
    auto any_ctre    = ctre::search<R"(.+)">;
    auto inputString = incom::commons::parseInputUsingCTRE::processFile(dataFile, any_ctre).front().front();

    std::string                                  inp(inputString.begin(), --inputString.end());
    incom::omni::MD_ChunkVector<char, 2, '#', 8> os1;
    decltype(os1)::Key_Type                      loc_key{0, 0};
    std::vector<decltype(loc_key)>               dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    size_t cursor = 0, curDoorCount = 0, maxDoorCount = 0, roomCountDoorsOver999 = 0;
    std::stack<std::pair<decltype(loc_key), size_t>> stackOfEndsAndDepths;

    // LAMBDAS
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

            if (charInRegex == '(') { stackOfEndsAndDepths.push({loc_key, curDoorCount}); }
            else if (charInRegex == ')') {
                curDoorCount = stackOfEndsAndDepths.top().second;
                loc_key      = stackOfEndsAndDepths.top().first;
                stackOfEndsAndDepths.pop();
            }
            else if (charInRegex == '|') {
                curDoorCount = stackOfEndsAndDepths.top().second;
                loc_key      = stackOfEndsAndDepths.top().first;
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

    // MAIN LOGIC
    exe_Descent();

    return std::make_pair(maxDoorCount, roomCountDoorsOver999);
}

} // namespace AOC2018