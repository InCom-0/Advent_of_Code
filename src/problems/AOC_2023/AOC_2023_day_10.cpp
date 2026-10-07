#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <limits>
#include <optional>
#include <stack>
#include <string>


namespace AOC2023 {

template <size_t N>
struct NoOpAlwaysTrue {
    constexpr bool
    operator()(std::array<size_t, N> const &) const noexcept {
        return true;
    }
};

template <size_t N>
struct NoOpOnFill {
    constexpr void
    operator()(std::array<size_t, N> const &) const noexcept {}
};

// Explores 'Dims-dimensional' space in Chebyshev-layered fashion (as if by chessboard distance)
template <size_t Dims, typename F_Allowed, typename F_OnFill>
requires(Dims > 1) && requires(std::array<size_t, Dims> const &item, F_Allowed f_a, F_OnFill f_of) {
    { f_a(item) } -> std::same_as<bool>;  // The F_Allowed need to be able to take 'Pos_t const&'
    { f_of(item) } -> std::same_as<void>; // The F_OnFill need to be able to take 'Pos_t const&'
}
class FloodFill {

#if defined(INCSTD_MDSPAN_UNDER_KOKKOS)
    template <class IndexType, size_t Rank>
    using pf_dextents = Kokkos::dextents<IndexType, Rank>;

    template <class ElementType, class Extents>
    using pf_mdspan = Kokkos::mdspan<ElementType, Extents>;
#else
    template <class IndexType, size_t Rank>
    using pf_dextents = std::dextents<IndexType, Rank>;

    template <class ElementType, class Extents>
    using pf_mdspan = std::mdspan<ElementType, Extents>;
#endif


public:
    using Pos_t     = std::array<size_t, Dims>;
    using Extents_t = pf_dextents<size_t, Dims>;
    using View_t    = pf_mdspan<char, Extents_t>;

    using DirChngs_t = std::array<Pos_t, Dims * 2>;


public:
    static constexpr auto c_IDs_sequence            = std::make_index_sequence<Dims>{};
    static constexpr auto c_IDs_sequenceMinus       = std::make_index_sequence<Dims - 1>{};
    static constexpr auto c_IDs_sequenceMinusDouble = std::make_index_sequence<2 * (Dims - 1)>{};

    Pos_t m_areaSzs_perDim;
    Pos_t m_areaMins_perDim;

private:
public:
    std::vector<char> m_visited_storage;
    View_t            m_visited;

    F_Allowed m_f_allowed;
    F_OnFill  m_f_onfill;


public:
    FloodFill(Pos_t areaSzs)
        : FloodFill(areaSzs, NoOpAlwaysTrue<Dims>{}) {}
    // F_allowed is a unary functor(lambda) taking std::array<size_t, Dims> const &

    FloodFill(Pos_t areaSzs, F_Allowed &&f_a)
        : FloodFill(areaSzs, std::forward<F_Allowed>(f_a), NoOpOnFill<Dims>{}) {}

    // F_allowed is a unary functor(lambda) taking std::array<size_t, Dims> const &
    FloodFill(Pos_t areaSzs, F_Allowed &&f_a, F_OnFill &&f_of)
        : FloodFill(areaSzs, {}, std::forward<F_Allowed>(f_a), std::forward<F_OnFill>(f_of)) {}

    // F_allowed is a unary functor(lambda) taking std::array<size_t, Dims> const &
    FloodFill(
        Pos_t areaSzs, Pos_t areaMins, F_Allowed &&f_a, F_OnFill &&f_of = [](Pos_t const &) {})
        : m_areaSzs_perDim(std::move(areaSzs)),
          m_areaMins_perDim{std::move(areaMins)},

          m_visited_storage(_ctor_total_sz(m_areaSzs_perDim), '.'),
          m_visited(m_visited_storage.data(), _ctor_make_extents(m_areaSzs_perDim)),
          m_f_allowed(std::forward<F_Allowed>(f_a)),
          m_f_onfill(std::forward<F_OnFill>(f_of)) {}


    constexpr bool
    is_inArea(Pos_t const &p) {
        return [&]<size_t... Is>(std::index_sequence<Is...>) {
            return ((p[Is] >= m_areaMins_perDim[Is]) && ...) && ((p[Is] < m_areaSzs_perDim[Is]) && ...);
        }(c_IDs_sequence);
    }

    constexpr void
    visit_at(Pos_t const &p) {
        [&]<size_t... Is>(std::index_sequence<Is...>) -> void { m_visited[p[Is]...] = '#'; }(c_IDs_sequence);
    }

    constexpr bool
    is_alreadyVisited(Pos_t const &p) {
        return [&]<size_t... Is>(std::index_sequence<Is...>) -> bool {
            return m_visited[p[Is]...] != '.';
        }(c_IDs_sequence);
    }

    constexpr std::size_t
    execute_fill(Pos_t seed) {
        using DimRange_t = std::array<Pos_t, 2uz>;
        using Frame_t    = std::array<DimRange_t, 2 * (Dims - 1)>;

        std::size_t         res{};
        std::stack<Frame_t> seedScanRngs{};

        std::optional<Pos_t> leftSeed = seed;
        Pos_t                rightSeed;

        auto fillFromSeed = [&]() {
            rightSeed = leftSeed.value();
            rightSeed.back()++;

            while (is_inArea(leftSeed.value()) && m_f_allowed(leftSeed.value())) {
                res++;
                visit_at(leftSeed.value());
                m_f_onfill(leftSeed.value());
                leftSeed.value().back()--;
            }
            while (is_inArea(rightSeed) && m_f_allowed(rightSeed)) {
                res++;
                visit_at(rightSeed);
                m_f_onfill(rightSeed);
                rightSeed.back()++;
            }
            leftSeed.value().back()++;
            rightSeed.back()--;

            // Adding new ranges to scan
            seedScanRngs.push([&]<size_t... Is>(std::index_sequence<Is...>) {
                return Frame_t{{((void)Is, DimRange_t{leftSeed.value(), rightSeed})...}};
            }(c_IDs_sequenceMinusDouble));


            [&]<size_t... Is>(std::index_sequence<Is...>) {
                for (size_t id{}; auto const &oneDir : incstd::explorers::directions::get_dirChanges<Dims - 1>()) {
                    ((seedScanRngs.top().at(id).front()[Is] += oneDir[Is]), ...);
                    id++;
                }

                // seedScanRngs.top();
            }(c_IDs_sequenceMinus);
        };

        auto searchForSeed = [&]() -> std::optional<Pos_t> {
            std::optional<Pos_t> res{};

            while (not seedScanRngs.empty()) {
                Frame_t &oneFrame = seedScanRngs.top();
                for (auto &[from, to] : oneFrame) {
                    while ((is_alreadyVisited(from) || not m_f_allowed(from)) && from.back() <= to.back()) {
                        from.back()++;
                    }
                    if (from.back() <= to.back()) {
                        res = from;
                        from.back()++;
                        goto RET;
                    }
                }
                seedScanRngs.pop();
            }

        RET:
            return res;
        };

        if (not is_inArea(leftSeed.value()) || not m_f_allowed(leftSeed.value()) ||
            is_alreadyVisited(leftSeed.value())) {
            goto RET;
        }

        fillFromSeed();
        while (leftSeed = searchForSeed(), leftSeed) { fillFromSeed(); }

    RET:
        return res;
    }


private:
    static constexpr size_t
    _ctor_total_sz(Pos_t const &sizes) {
        return std::ranges::fold_left(sizes, 1uz, std::multiplies{});
    }

    static constexpr Extents_t
    _ctor_make_extents(Pos_t const &sizes) {
        return [&]<size_t... Is>(std::index_sequence<Is...>) { return Extents_t(sizes[Is]...); }(c_IDs_sequence);
    }

    static constexpr Pos_t
    make_filledPos_size_t() {
        return [&]<size_t... Is>(std::index_sequence<Is...>) {
            return Pos_t{((void)Is, std::numeric_limits<size_t>::max())...};
        }(std::make_index_sequence<Dims>{});
    }
};

template <size_t N>
FloodFill(std::array<size_t, N> const &) -> FloodFill<N, NoOpAlwaysTrue<N>, NoOpOnFill<N>>;

template <size_t N, typename F_A>
FloodFill(std::array<size_t, N> const &, F_A &&) -> FloodFill<N, std::remove_cvref_t<F_A>, NoOpOnFill<N>>;

template <size_t N, typename F_A, typename F_OF>
FloodFill(std::array<size_t, N> const &, F_A &&, F_OF &&)
    -> FloodFill<N, std::remove_cvref_t<F_A>, std::remove_cvref_t<F_OF>>;

template <size_t N, typename F_A, typename F_OF>
FloodFill(std::array<size_t, N> const &, std::array<size_t, N> const &, F_A &&, F_OF &&)
    -> FloodFill<N, std::remove_cvref_t<F_A>, std::remove_cvref_t<F_OF>>;


auto
day10_0(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto res    = incom::aoc::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();
    res.push_back(std::string(res.back().size(), '#'));
    res.push_back(std::string(res.back().size(), '#'));
    std::ranges::rotate(res, res.end() - 1);

    for (auto &line : res) {
        line.push_back('#');
        line.push_back('#');
        std::ranges::rotate(line, line.end() - 1);
    }
    return res;
}

auto
day10_1(std::string dataFile) {
    auto input                  = day10_0(dataFile);
    auto const [yStart, xStart] = [&]() {
        size_t yStart = 0uz;
        size_t xStart = 0uz;

        while (yStart < input.size()) {
            if (auto it = std::ranges::find(input.at(yStart), 'S'); it != input.at(yStart).end()) {
                return std::make_pair(yStart, static_cast<size_t>(it - input.at(yStart).begin()));
            }
            yStart++;
        }
        return std::make_pair(0uz, 0uz);
    }();


    struct Dirs {
        int yFrom = 0;
        int xFrom = 0;
        int yTo   = 0;
        int xTo   = 0;
    };

    std::array<Dirs, 256> mp{};
    mp.at('|') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 1, .xTo = 0};
    mp.at('-') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 0, .xTo = 1};
    mp.at('L') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('J') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = -1};
    mp.at('7') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 1, .xTo = 0};
    mp.at('F') = Dirs{.yFrom = 1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('.') = Dirs{.yFrom = 0, .xFrom = 0, .yTo = 0, .xTo = 0};

    auto convertFrom  = [&](char c) { return mp.at(c); };
    auto [yCur, xCur] = std::make_pair(yStart, xStart);


    Dirs curDir([&]() {
        std::pair<int, int>              r;
        std::vector<std::pair<int, int>> vec;
        if (Dirs d = mp.at(input[yCur - 1][xCur]); ((d.yFrom == 1 && d.xFrom == 0) || (d.yTo == 1 && d.xTo == 0))) {
            vec.push_back({-1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur - 1]); ((d.yFrom == 0 && d.xFrom == 1) || (d.yTo == 0 && d.xTo == 1))) {
            vec.push_back({0, -1});
        }
        if (Dirs d = mp.at(input[yCur + 1][xCur]); ((d.yFrom == -1 && d.xFrom == 0) || (d.yTo == -1 && d.xTo == 0))) {
            vec.push_back({1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur + 1]); ((d.yFrom == 0 && d.xFrom == -1) || (d.yTo == 0 && d.xTo == -1))) {
            vec.push_back({0, 1});
        }

        return Dirs{vec.begin()->first, (vec.begin())->second, (vec.begin() + 1)->first, (vec.begin() + 1)->second};
    }());


    size_t stepsCounter{};
    bool   switched = false;

    do {
        yCur = yCur + (switched ? curDir.yFrom : curDir.yTo);
        xCur = xCur + (switched ? curDir.xFrom : curDir.xTo);

        auto const &nextDirs = mp[input[yCur][xCur]];
        if ((((nextDirs.yTo * (-1)) == (switched ? curDir.yFrom : curDir.yTo)) &&
             ((nextDirs.xTo * (-1)) == (switched ? curDir.xFrom : curDir.xTo)))) {
            switched = true;
        }
        else { switched = false; }

        curDir = nextDirs;
        stepsCounter++;
    } while (not(yCur == yStart && xCur == xStart));

    return stepsCounter / 2;
}

auto
day10_2(std::string dataFile) {
    auto const input            = day10_0(dataFile);
    auto const [yStart, xStart] = [&]() {
        size_t yStart = 0uz;
        size_t xStart = 0uz;

        while (yStart < input.size()) {
            if (auto it = std::ranges::find(input.at(yStart), 'S'); it != input.at(yStart).end()) {
                return std::make_pair(yStart, static_cast<size_t>(it - input.at(yStart).begin()));
            }
            yStart++;
        }
        return std::make_pair(0uz, 0uz);
    }();


    struct Dirs {
        int yFrom = 0;
        int xFrom = 0;
        int yTo   = 0;
        int xTo   = 0;
    };

    auto [yCur, xCur] = std::make_pair(yStart, xStart);
    std::array<Dirs, 256> mp{};
    mp.at('|') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 1, .xTo = 0};
    mp.at('-') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 0, .xTo = 1};
    mp.at('L') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('J') = Dirs{.yFrom = -1, .xFrom = 0, .yTo = 0, .xTo = -1};
    mp.at('7') = Dirs{.yFrom = 0, .xFrom = -1, .yTo = 1, .xTo = 0};
    mp.at('F') = Dirs{.yFrom = 1, .xFrom = 0, .yTo = 0, .xTo = 1};
    mp.at('.') = Dirs{.yFrom = 0, .xFrom = 0, .yTo = 0, .xTo = 0};
    mp.at('S') = [&]() {
        std::pair<int, int>              r;
        std::vector<std::pair<int, int>> vec;
        if (Dirs d = mp.at(input[yCur - 1][xCur]); ((d.yFrom == 1 && d.xFrom == 0) || (d.yTo == 1 && d.xTo == 0))) {
            vec.push_back({-1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur - 1]); ((d.yFrom == 0 && d.xFrom == 1) || (d.yTo == 0 && d.xTo == 1))) {
            vec.push_back({0, -1});
        }
        if (Dirs d = mp.at(input[yCur + 1][xCur]); ((d.yFrom == -1 && d.xFrom == 0) || (d.yTo == -1 && d.xTo == 0))) {
            vec.push_back({1, 0});
        }
        if (Dirs d = mp.at(input[yCur][xCur + 1]); ((d.yFrom == 0 && d.xFrom == -1) || (d.yTo == 0 && d.xTo == -1))) {
            vec.push_back({0, 1});
        }

        return Dirs{vec.begin()->first, (vec.begin())->second, (vec.begin() + 1)->first, (vec.begin() + 1)->second};
    }();

    Dirs curDirs = mp.at('S');


    bool      switched = false;
    long long LR{};

    auto inputCopy = input;

    auto swtch = [&](size_t const &yC, size_t const &xC) {
        switch (input[yC][xC]) {
            case 'L': switched ? LR++ : LR--; break;
            case 'J': switched ? LR-- : LR++; break;
            case '7': switched ? LR-- : LR++; break;
            case 'F': switched ? LR-- : LR++; break;
            default:  void();
        }
    };

    size_t stepsCounter{};
    do {
        swtch(yCur, xCur);
        inputCopy[yCur][xCur] = '#';

        yCur = yCur + (switched ? curDirs.yFrom : curDirs.yTo);
        xCur = xCur + (switched ? curDirs.xFrom : curDirs.xTo);

        auto const &nextDirs = mp[input[yCur][xCur]];
        if ((((nextDirs.yTo * (-1)) == (switched ? curDirs.yFrom : curDirs.yTo)) &&
             ((nextDirs.xTo * (-1)) == (switched ? curDirs.xFrom : curDirs.xTo)))) {
            switched = true;
        }
        else { switched = false; }

        curDirs = nextDirs;
        stepsCounter++;
    } while (not(yCur == yStart && xCur == xStart));

    curDirs  = mp.at('S');
    switched = false;

    auto rotLR = [&](std::array<long long, 2> const &rot, bool const swtch) {
        using MpItem = std::pair<std::array<long long, 2>, std::array<long long, 2>>;
        constexpr std::array<MpItem, 4> mp{
            {{{-1, 0}, {0, -1}}, {{0, -1}, {1, 0}}, {{1, 0}, {0, 1}}, {{0, 1}, {-1, 0}}}};

        std::array<long long, 2> res =
            std::ranges::find_if(mp, [&](MpItem const &item) { return item.first == rot; })->second;
        if (swtch) {
            res[0] *= -1;
            res[1] *= -1;
        }
        return res;
    };


    auto   ff = FloodFill(std::array{input.size(), input.front().size()},
                          [&](std::array<size_t, 2uz> const &pos) { return inputCopy[pos[0]][pos[1]] != '#'; });
    size_t enclosedCounter{};

    do {
        auto const [yChng, xChng] =
            rotLR({switched ? curDirs.yTo : curDirs.yFrom, switched ? curDirs.xTo : curDirs.xFrom}, LR < 0);

        auto const [yChng2, xChng2] =
            rotLR({switched ? curDirs.yFrom : curDirs.yTo, switched ? curDirs.xFrom : curDirs.xTo}, LR >= 0);

        enclosedCounter += ff.execute_fill({yCur + yChng, xCur + xChng});
        enclosedCounter += ff.execute_fill({yCur + yChng2, xCur + xChng2});

        yCur = yCur + (switched ? curDirs.yFrom : curDirs.yTo);
        xCur = xCur + (switched ? curDirs.xFrom : curDirs.xTo);

        auto const &nextDirs = mp[input[yCur][xCur]];
        if ((((nextDirs.yTo * (-1)) == (switched ? curDirs.yFrom : curDirs.yTo)) &&
             ((nextDirs.xTo * (-1)) == (switched ? curDirs.xFrom : curDirs.xTo)))) {
            switched = true;
        }
        else { switched = false; }

        curDirs = nextDirs;
    } while (not(yCur == yStart && xCur == xStart));


    return enclosedCounter;
}
} // namespace AOC2023