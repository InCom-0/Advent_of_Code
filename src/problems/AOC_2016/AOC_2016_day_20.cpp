
#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <md5.h>


namespace AOC2016 {


unsigned long day20_1(std::string dataFile) {
    struct IPrange_t {
        unsigned long long start;
        unsigned long long end;
    };

    std::vector<IPrange_t> blockList;

    auto getRngStart = ctre::search<R"(\d+(?=\-))">;
    auto getRngEnd   = ctre::search<R"((?<=\-)\d+)">;

    auto VofV_IP_ranges = incom::commons::parseInputUsingCTRE::processFile(dataFile, getRngStart, getRngEnd);

    for (int i = 0; i < VofV_IP_ranges[0].size(); ++i) {
        blockList.push_back(IPrange_t{std::stoul(VofV_IP_ranges[0][i]), std::stoul(VofV_IP_ranges[1][i])});
    }
    std::sort(blockList.begin(), blockList.end(), [](auto &&a, auto &&b) { return a.start < b.start; });

    unsigned long long maxVal = 0;

    for (auto &ip_rng_val : blockList) {
        if (ip_rng_val.start > maxVal) { break; }
        else { maxVal = std::max(maxVal, ip_rng_val.end + 1); }
    }
    return maxVal;
}


unsigned long day20_2(std::string dataFile) {
    struct IPrange_t {
        unsigned long long start;
        unsigned long long end;
    };

    std::vector<IPrange_t> blockList;

    auto getRngStart = ctre::search<R"(\d+(?=\-))">;
    auto getRngEnd   = ctre::search<R"((?<=\-)\d+)">;

    auto VofV_IP_ranges = incom::commons::parseInputUsingCTRE::processFile(dataFile, getRngStart, getRngEnd);

    for (int i = 0; i < VofV_IP_ranges[0].size(); ++i) {
        blockList.push_back(IPrange_t{std::stoul(VofV_IP_ranges[0][i]), std::stoul(VofV_IP_ranges[1][i])});
    }
    std::sort(blockList.begin(), blockList.end(), [](auto &&a, auto &&b) { return a.start < b.start; });

    unsigned long      allowedIP_count = 0;
    unsigned long long maxVal          = 0;

    for (auto &ip_rng_val : blockList) {
        if (ip_rng_val.start > maxVal) {
            allowedIP_count  += (ip_rng_val.start - maxVal);
            maxVal           = ip_rng_val.end + 1;
        }
        else { maxVal = std::max(maxVal, ip_rng_val.end + 1); }
    }

    return allowedIP_count;
}

} // namespace AOC2016
