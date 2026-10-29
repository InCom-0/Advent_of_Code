#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <cstdint>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.hpp>
#include <incstd/solvers/directions.hpp>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <utility>


namespace AOC2023 {
namespace {
using Pos_t = std::array<long long, 2uz>;
struct Vert {
    Pos_t               p{};
    std::vector<size_t> targetVerts;
    std::vector<size_t> distances;
};
} // namespace

auto
day23_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;

    auto input = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();
    auto inputVertRecord =
        std::vector(input.size(), std::vector(input.front().size(), std::numeric_limits<size_t>::max()));


    Pos_t const start{1uz, 0uz};
    Pos_t const end{static_cast<long long>(input.size() - 1),
                    static_cast<long long>(std::ranges::find(input.back(), '.') - input.back().begin())};

    size_t            curVertID{};
    std::vector<Vert> verts;
    input[start[0]][start[1] + 1] = '>';

    auto moveUntil = [&](this auto const &self, Pos_t p, Pos_t lastDir) -> std::pair<std::optional<Pos_t>, size_t> {
        size_t stepInSection{};
        while (p != end && (input[p[0]][p[1]] == '.')) {
            for (auto const &d : incstd::solvers::directions::get_dirChanges_2D()) {
                if (d[0] == -lastDir[0] && d[1] == -lastDir[1]) { continue; }

                if (input[p[0] + d[0]][p[1] + d[1]] != '#') {
                    p[0]    += d[0];
                    p[1]    += d[1];
                    lastDir  = d;
                    stepInSection++;
                    break;
                }
            }
        }

        if (p != end) {
            p[0] += lastDir[0];
            p[1] += lastDir[1];
            return std::make_pair(p, stepInSection + 3);
        }
        else { return std::make_pair(p, stepInSection + 2); }
    };


    auto exploreFromVert = [&](this auto const &self, Pos_t p) -> size_t {
        constexpr auto DirSigns = std::array<char, 4uz>{'v', '^', '>', '<'};

        size_t const thisVertID     = curVertID++;
        inputVertRecord[p[0]][p[1]] = thisVertID;
        verts.push_back(Vert{.p = p});

        if (p != end) {
            for (size_t i{}; auto const &d : incstd::solvers::directions::get_dirChanges_2D()) {
                // Explore only if we have an 'arrow' leading from the 'p' of vert
                if (p[0] + d[0] >= 0 && p[0] + d[0] < input.size() && p[1] + d[1] >= 0 &&
                    p[1] + d[1] < input.at(p[0]).size() && input[p[0] + d[0]][p[1] + d[1]] == DirSigns[i++]) {
                    auto [newVertPos, dist] = moveUntil(Pos_t{p[0] + 2 * d[0], p[1] + 2 * d[1]}, d);

                    if (newVertPos.has_value()) {
                        // We havent visited this vert ... add its ID, link to it and then explore from it.
                        if (inputVertRecord[newVertPos.value()[0]][newVertPos.value()[1]] ==
                            std::numeric_limits<size_t>::max()) {

                            auto const otherVertID = self(newVertPos.value());
                            verts[thisVertID].targetVerts.push_back(std::move(otherVertID));
                            verts[thisVertID].distances.push_back(std::move(dist));
                        }
                        // We already visited this vert ... therefore just 'link' to its ID
                        else {
                            verts[thisVertID].targetVerts.push_back(
                                inputVertRecord[newVertPos.value()[0]][newVertPos.value()[1]]);
                            verts[thisVertID].distances.push_back(std::move(dist));
                        }
                    }
                    // We are at the end ... do nothing
                    else {}
                }
            }
        }
        return thisVertID;
    };

    exploreFromVert(start);


    return std::make_pair(input, verts);
}

size_t
day23_1(std::string dataFile) {
    auto const [input, verts] = day23_0(dataFile);

    std::vector<uint8_t> visited(verts.size(), 0);
    std::vector<size_t>  maxStepsTo(verts.size(), 0uz);

    Pos_t const end{static_cast<long long>(input.size() - 1),
                    static_cast<long long>(std::ranges::find(input.back(), '.') - input.back().begin())};

    size_t curSteps      = 0uz;
    size_t maxStepsToTar = 0uz;
    auto   recu          = [&](this auto const &self, size_t const vertID) -> void {
        if (verts[vertID].p == end) { maxStepsToTar = std::max(maxStepsToTar, curSteps); }
        for (auto const &[target, distance] : std::views::zip(verts[vertID].targetVerts, verts[vertID].distances)) {
            if (visited[target] == 0) {
                curSteps += distance;
                visited[target]++;

                self(target);

                visited[target]--;
                curSteps -= distance;
            }
        }
    };

    recu(0uz);

    return maxStepsToTar;
}

size_t
day23_2(std::string dataFile) {
    auto const [input, verts] = day23_0(dataFile);

    std::vector<uint8_t> visited(verts.size(), 0);
    std::vector<size_t>  maxStepsTo(verts.size(), 0uz);

    Pos_t const end{static_cast<long long>(input.size() - 1),
                    static_cast<long long>(std::ranges::find(input.back(), '.') - input.back().begin())};

    auto vertsAdj = verts;
    for (size_t id{}; auto const &vrt : verts) {
        for (auto const &[vrtTar, vrtDist] : std::views::zip(vrt.targetVerts, vrt.distances)) {
            vertsAdj[vrtTar].targetVerts.push_back(id);
            vertsAdj[vrtTar].distances.push_back(vrtDist);
        }
        id++;
    }

    size_t curSteps      = 0uz;
    size_t maxStepsToTar = 0uz;

    auto recu = [&](this auto const &self, size_t const vertID) -> void {
        if (vertsAdj[vertID].p == end) { maxStepsToTar = std::max(maxStepsToTar, curSteps); }
        for (auto const &[target, distance] :
             std::views::zip(vertsAdj[vertID].targetVerts, vertsAdj[vertID].distances)) {
            if (visited[target] == 0) {
                curSteps += distance;
                visited[target]++;

                self(target);

                visited[target]--;
                curSteps -= distance;
            }
        }
    };

    recu(0uz);

    return maxStepsToTar;
}

} // namespace AOC2023