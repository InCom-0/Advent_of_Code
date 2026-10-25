#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/core/buffers.hpp>
#include <incstd/core/hashing.hpp>
#include <incstd/solvers/directions.hpp>
#include <ranges>
#include <string>
#include <utility>


namespace AOC2023 {

namespace {
struct Pos {
    long long y{};
    long long x{};

    auto
    operator<=>(Pos const &) const = default;
};

void
XXH3Hash(Pos p, XXH3_state_t *s) {
    XXH3_64bits_update(s, &p, sizeof(long long) * 2);
}
void
XXH3Hash(std::pair<Pos, size_t> p, XXH3_state_t *s) {
    XXH3_64bits_update(s, &p.first, sizeof(long long) * 2);
    XXH3_64bits_update(s, &p.second, sizeof(size_t));
}
} // namespace

auto
day21_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;

    auto res = std::make_pair(incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front(), Pos{});

    for (size_t row{}; auto &line : res.first) {
        for (size_t col{}; auto &c : line) {
            if (c == 'S') {
                res.second.y = row;
                res.second.x = col;
                c            = '.';
            }
            col++;
        }
        row++;
    }
    return res;
}

size_t
day21_1(std::string dataFile) {

    auto  input  = day21_0(dataFile);
    auto &theMap = input.first;

    incstd::buffers::DoubleBuffer<ankerl::unordered_dense::set<Pos, incstd::hashing::XXH3Hasher>> buf;
    buf.getCurrent().insert(input.second);


    for (size_t stepsToDo = 64uz; stepsToDo > 0; --stepsToDo) {
        for (auto const &curPos : buf.getCurrent()) {
            for (auto const &[yChng, xChng] : incstd::solvers::directions::get_dirChanges_2D()) {
                const Pos np{.y = curPos.y + yChng, .x = curPos.x + xChng};
                if (np.y < 0 || np.y >= theMap.size() || np.x < 0 || np.x >= theMap.front().size() ||
                    theMap[np.y][np.x] != '.') {
                    continue;
                }

                buf.getNext().insert(std::move(np));
            }
        }
        buf.swap_buffers();
        buf.getNext().clear();
    }


    int a;
    return buf.getCurrent().size();
}

size_t
day21_2(std::string dataFile) {
    auto  input  = day21_0(dataFile);
    auto &theMap = input.first;

    incstd::buffers::MultiBuffer<ankerl::unordered_dense::map<Pos, size_t, incstd::hashing::XXH3Hasher>> buf;
    buf.getCurrent().insert({input.second, 0uz});


    for (size_t curStepID = 1uz; curStepID < 194; ++curStepID) {
        for (auto const &curPos : buf.getCurrent()) {
            for (auto const &[yChng, xChng] : incstd::solvers::directions::get_dirChanges_2D()) {
                const Pos np{.y = curPos.first.y + yChng, .x = curPos.first.x + xChng};
                if (np.y < 0 || np.y >= theMap.size() || np.x < 0 || np.x >= theMap.front().size() ||
                    theMap[np.y][np.x] != '.' || buf.getCurrent().contains(np)) {
                    continue;
                }

                buf.getNext().insert({std::move(np), curStepID});
            }
        }
        buf.swap_buffers();
    }

    size_t const n          = 202300uz;
    size_t       oddPlaces  = buf.getCurrent().size();
    size_t       evenPlaces = buf.getNext().size();

    size_t oddCorners = std::ranges::count_if(
        std::views::filter(buf.getCurrent(), [](auto const &item) { return item.second > 65; }),
        [](auto const &item2) { return true; });

    size_t evenCorners =
        std::ranges::count_if(std::views::filter(buf.getNext(), [](auto const &item) { return item.second > 65; }),
                              [](auto const &item2) { return true; });

    return ((n + 1) * (n + 1) * oddPlaces) + ((n * n) * evenPlaces) - ((n + 1) * oddCorners) + (n * evenCorners);
}

} // namespace AOC2023