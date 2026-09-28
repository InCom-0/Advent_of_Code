#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cassert>
#include <charconv>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <limits>
#include <ranges>
#include <string>
#include <type_traits>


namespace AOC2023 {
struct MapItem {
    long long dest_start   = {};
    long long source_start = {};
    long long length       = {};
};


template <typename T, std::size_t NUM = 2>
requires(NUM > 1) && std::is_default_constructible_v<T>
class multiBuffer {
private:
    std::array<T, NUM> __data;

    std::array<T *, NUM> __dataPTRs = [&]<typename SZ, SZ... ints>(const std::integer_sequence<SZ, ints...> &) {
        return std::array<T *, NUM>{&__data[ints]...};
    }(std::make_index_sequence<NUM>{});

public:
    // CONSTRUCTION
    multiBuffer()
        : __data([]<typename SZ, SZ... ints>(const std::integer_sequence<SZ, ints...> &) {
              return std::array<T, NUM>{(ints, T{})...};
          }(std::make_index_sequence<NUM>{})) {};

    // By default 'initial_data' gets only copied into 'Current', rest is default constructed
    template <bool fillAll = false>
    multiBuffer(T const &initial_data)
        : __data([&]<typename SZ, SZ... ints>(const std::integer_sequence<SZ, ints...> &) {
              if constexpr (fillAll) { return std::array<T, NUM>{(ints, initial_data)...}; }
              else {
                  auto res    = std::array<T, NUM>{(ints, T{})...};
                  res.front() = initial_data;
                  return res;
              }
          }(std::make_index_sequence<NUM>{})) {};


    multiBuffer(multiBuffer const &other) = delete;
    multiBuffer(multiBuffer &&other)      = delete;


    // GETTING THE CONTAINED DATA
    T &
    getCurrent() const {
        return *__dataPTRs[0];
    }
    T &
    getNext() const {
        return *__dataPTRs[1];
    }

    template <std::size_t ID>
    requires(ID < NUM)
    T &
    getNth() const {
        return *__dataPTRs[ID];
    }

    // SWAPPING / ROTATING
    // 'Next' becomes 'Current', 'Current' goes to last, others similarly
    void
    rotate() {
        std::ranges::rotate(__dataPTRs, __dataPTRs.begin() + 1);
    }

    void
    rotate_reverse() {
        std::ranges::rotate(__dataPTRs, __dataPTRs.end() - 1);
    }
};


auto
day5_0(std::string const &dataFile) {
    auto number_ctre = ctre::search<R"(\d+)">;
    auto colon_ctre  = ctre::search<R"([[:punct:]])">;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { assert(false); }


    std::pair<std::vector<long long>, std::vector<std::vector<MapItem>>> res;

    std::vector<long long>            &seeds = res.first;
    std::vector<std::vector<MapItem>> &maps  = res.second;

    std::string oneStr;
    std::getline(iStream, oneStr);

    auto                  result = number_ctre(oneStr.begin(), oneStr.end());
    std::string::iterator bg     = oneStr.begin();
    std::string::iterator end    = oneStr.end();

    while (result = number_ctre(bg, end), result) {
        bg = result.get_end_position();
        seeds.emplace_back();
        std::from_chars(result.data(), result.data() + result.size(), seeds.back());
    }

    while (std::getline(iStream, oneStr)) {
        bg  = oneStr.begin();
        end = oneStr.end();

        if (result = colon_ctre(bg, end); result) { maps.emplace_back(); }
        else if (result = number_ctre(bg, end); result) {
            maps.back().emplace_back();
            std::from_chars(result.data(), result.data() + result.size(), maps.back().back().dest_start);

            bg = result.get_end_position();
            if (result = number_ctre(bg, end); result) {
                std::from_chars(result.data(), result.data() + result.size(), maps.back().back().source_start);
                bg = result.get_end_position();
            }
            if (result = number_ctre(bg, end); result) {
                std::from_chars(result.data(), result.data() + result.size(), maps.back().back().length);
            }
            else { assert(false); }
        }
        else {} // Nothing
    }

    return res;
}

size_t
day5_1(std::string dataFile) {
    auto const [seeds, maps] = day5_0(dataFile);
    auto curNums             = seeds;


    long long scratch{};
    for (auto const &oneMapping : maps) {
        for (auto &oneNum : curNums) {
            for (auto const &mapTtem : oneMapping) {
                scratch = oneNum - mapTtem.source_start;
                if (scratch >= 0 && scratch < mapTtem.length) {
                    oneNum = mapTtem.dest_start + scratch;
                    break;
                }
            }
        }
    }


    return std::ranges::fold_left(curNums, std::numeric_limits<long long>::max(),
                                  [](auto const &accu, auto const &item) { return std::min(accu, item); });
}

size_t
day5_2(std::string dataFile) {
    auto [seeds, maps] = day5_0(dataFile);
    for (auto &oneMapping : maps) {
        std::ranges::sort(oneMapping, [](auto const &a, auto const &b) { return a.source_start < b.source_start; });
    }

    multiBuffer seedRngs(std::views::pairwise(seeds) | std::views::stride(2) |
                         std::views::transform([](auto const &pr) {
                             return MapItem{.source_start = std::get<0>(pr), .length = std::get<1>(pr)};
                         }) |
                         std::ranges::to<std::vector>());

    // seedRngs.getNext().clear();


    for (auto const &oneMapping : maps) {
        for (auto &curSeed : seedRngs.getCurrent()) {

            while (curSeed.length != 0) {
                if (auto iter = std::ranges::find_if(oneMapping,
                                                     [&](auto const &mapLine) {
                                                         return (mapLine.source_start <= curSeed.source_start) &&
                                                                ((mapLine.source_start + mapLine.length) >
                                                                 curSeed.source_start);
                                                     });
                    iter != oneMapping.end()) {

                    long long const newLength =
                        std::min(((iter->source_start + iter->length) - curSeed.source_start), curSeed.length);
                    seedRngs.getNext().push_back(
                        MapItem{.source_start = (iter->dest_start + (curSeed.source_start - iter->source_start)),
                                .length       = newLength});
                    curSeed.source_start += newLength;
                    curSeed.length       -= newLength;
                }

                else {
                    if (auto iter = std::ranges::find_if(oneMapping,
                                                         [&](auto const &mapLine) {
                                                             return (mapLine.source_start > curSeed.source_start) &&
                                                                    (mapLine.source_start <
                                                                     (curSeed.source_start + curSeed.length));
                                                         });
                        iter != oneMapping.end()) {

                        long long const newLength = iter->source_start - curSeed.source_start;
                        seedRngs.getNext().push_back(
                            MapItem{.source_start = curSeed.source_start, .length = newLength});
                        curSeed.source_start += newLength;
                        curSeed.length       -= newLength;
                    }
                    else {
                        seedRngs.getNext().push_back(curSeed);
                        curSeed.source_start += curSeed.length;
                        curSeed.length        = 0;
                    }
                }
            }
        }

        seedRngs.rotate();
        seedRngs.getNext().clear();
    }


    return std::ranges::fold_left(seedRngs.getCurrent(), std::numeric_limits<long long>::max(),
                                  [](auto const &accu, auto const &item) { return std::min(accu, item.source_start); });
}
} // namespace AOC2023