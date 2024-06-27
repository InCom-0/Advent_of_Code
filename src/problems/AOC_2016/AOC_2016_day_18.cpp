#include <fstream>

#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>



namespace AOC2016 {
int day18_1(std::string dataFile, int const rowsToConsider) {

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return INT_MIN; }

    std::string input;
    std::getline(iStream, input);

    std::vector<bit::bit_vector<unsigned char>> map;
    map.push_back(bit::bit_vector<unsigned char>());

    map.front().push_back(bit::bit0);
    for (auto const &letter : input) { map.front().push_back(bit::bit_value((bool)(letter - '.'))); }
    map.front().push_back(bit::bit0);

    int trapCounter = bit::count(map.front().begin() + 1, map.front().end() - 1, bit::bit0);

    for (int i = 0; i < rowsToConsider - 1; ++i) {
        map.push_back(bit::bit_vector<unsigned char>());
        map[i + 1].push_back(bit::bit0);

        /*Comparing the equalty of first a last element of each 'tripple' is all we need.
        For some reason flux slide+for_each is way less performant than handrolled algo below
        ... could be worth investigating why that happens and what if anything could be done to alleviate this.*/
        for (int j = 0; j < map[i].size() - 2; ++j) {
            if ((map[i].begin() + j).operator*() == (map[i].begin() + j + 2).operator*()) {
                map[i + 1].push_back(bit::bit0);
                trapCounter++;
            }
            else { map[i + 1].push_back(bit::bit1); }
        }
        map[i + 1].push_back(bit::bit0);
    }
    return trapCounter;
}

} // namespace AOC2016
