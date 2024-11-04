#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <numeric>


namespace AOC2019 {
long long day12_1(std::string dataFile) {

    struct Vec3 {
        long long x = 0;
        long long y = 0;
        long long z = 0;

        Vec3 &operator+=(Vec3 const &rhs) {
            this->x += rhs.x;
            this->y += rhs.y;
            this->z += rhs.z;
            return (*this);
        }
    };

    auto d_ctre = ctre::search<R"(-?\d+)">;

    auto input = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    std::vector<Vec3> positions;

    for (auto &inpLine : input) {
        positions.push_back({std::stoll(inpLine[0]), std::stoll(inpLine[1]), std::stoll(inpLine[2])});
    }
    std::vector<Vec3> velocities(positions.size(), Vec3{});


    // LAMBDA DEFINITIONS
    auto pairSolver = [](Vec3 &pos_a, Vec3 &pos_b, Vec3 &vel_a, Vec3 &vel_b) -> void {
        auto cmpResX = (pos_a.x <=> pos_b.x);
        auto cmpResY = (pos_a.y <=> pos_b.y);
        auto cmpResZ = (pos_a.z <=> pos_b.z);

        vel_a.x += ((cmpResX < 0) * 1) + (cmpResX > 0) * (-1);
        vel_b.x += ((cmpResX > 0) * 1) + (cmpResX < 0) * (-1);

        vel_a.y += ((cmpResY < 0) * 1) + (cmpResY > 0) * (-1);
        vel_b.y += ((cmpResY > 0) * 1) + (cmpResY < 0) * (-1);

        vel_a.z += ((cmpResZ < 0) * 1) + (cmpResZ > 0) * (-1);
        vel_b.z += ((cmpResZ > 0) * 1) + (cmpResZ < 0) * (-1);
    };

    auto oneIteration = [&]() {
        for (int lhsID = 0; lhsID < positions.size(); ++lhsID) {
            for (int rhsID = (lhsID + 1); rhsID < positions.size(); ++rhsID) {
                pairSolver(positions[lhsID], positions[rhsID], velocities[lhsID], velocities[rhsID]);
            }
        }

        for (int moonID = 0; moonID < positions.size(); ++moonID) { positions[moonID] += velocities[moonID]; }
    };

    for (int i = 0; i < 1000; ++i) { oneIteration(); }


    auto calcEnergyOfOne = [&](Vec3 const &one) -> long long {
        return (std::abs(one.x) + std::abs(one.y) + (std::abs(one.z)));
    };

    auto res =
        flux::zip(flux::ref(positions), flux::ref(velocities))
            .fold([&](auto &&init, auto &&a) { return init + (calcEnergyOfOne(a.first) * calcEnergyOfOne(a.second)); },
                  0ll);

    return res;
}

long long day12_2(std::string dataFile) {

    auto d_ctre = ctre::search<R"(-?\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre);

    // VofV, top level vector is the dimensions, bottom level are the positions of individual moons on that dimension
    std::vector positions(3, std::vector<long long>());


    for (auto &inpLine : input) {
        for (int i = 0; i < inpLine.size(); ++i) { positions[i].push_back(std::stoll(inpLine[i])); }
    }
    std::vector velocities(3, std::vector<long long>(positions.front().size(), 0));

    auto startPosi = positions;
    auto startVelo = velocities;

    // LAMBDA DEFINITIONS
    auto pairVelSolverOneDim = [&](int const &dimensionID, int const &lhsID, int const &rhsID) -> void {
        auto cmpRes = (positions[dimensionID][lhsID] <=> positions[dimensionID][rhsID]);

        velocities[dimensionID][lhsID] += ((cmpRes < 0) * 1) + (cmpRes > 0) * (-1);
        velocities[dimensionID][rhsID] += ((cmpRes > 0) * 1) + (cmpRes < 0) * (-1);
    };

    std::vector<long long> repeatingPeriods(3, 0);

    auto oneIteration = [&](int const &dim) {
        for (int lhsID = 0; lhsID < positions.front().size(); ++lhsID) {
            for (int rhsID = (lhsID + 1); rhsID < positions.front().size(); ++rhsID) {
                pairVelSolverOneDim(dim, lhsID, rhsID);
            }
        }

        for (int moonID = 0; moonID < positions.front().size(); ++moonID) {
            positions[dim][moonID] += velocities[dim][moonID];
        }
    };

    for (int dim = 0; dim < 3; ++dim) {
        long long periodCounter = 0;
        do {
            oneIteration(dim);
            periodCounter++;
        } while (not (startPosi[dim] == positions[dim] && startVelo[dim] == velocities[dim]));
        repeatingPeriods[dim] = periodCounter;
    }

    long long lcm = 1;
    for (auto &oneDimMult : repeatingPeriods) { lcm = std::lcm(lcm, oneDimMult); }

    return lcm;
}
} // namespace AOC2019