#include <ctime>
#include <ctre/wrapper.hpp>

#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <chrono>
#include <ctre.hpp>
#include <flux.hpp>
#include <string>
#include <unordered_map>


namespace AOC2018 {
int day4_1(std::string dataFile) {

    // CUSTOM TYPES
    auto dateTime_ctre = ctre::search<R"([^\[]+(?=\]))">;
    auto lastWord_ctre = ctre::search<R"(\w+)">;

    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, dateTime_ctre, lastWord_ctre, lastWord_ctre);

    std::chrono::system_clock::time_point                              tm1;
    std::vector<std::pair<std::chrono::system_clock::time_point, int>> data;
    std::istringstream                                                 ss;
    std::unordered_map<int, std::vector<int>>                          guardToMinutes;

    // DATA PREP
    for (auto &line : VofV) {
        line.front()[0] = '2';
        line.front()[1] = '0';
        ss.str(line.front());
        std::chrono::from_stream(ss, "%Y-%m-%d %H:%M", tm1);
        int ins;
        if (line.back() == "asleep") { ins = INT_MIN; }
        else if (line.back() == "up") { ins = 0; }
        else {
            ins = std::stoi(line[2]);
            guardToMinutes.emplace(ins, std::vector<int>(60, 0));
        }


        data.push_back(std::make_pair(tm1, ins));
    }
    std::ranges::sort(data, [](auto &&a, auto &&b) { return a.first < b.first; });

    // MAIN LOGIC
    int thisGuardID;
    for (int i = 0; i < data.size(); ++i) {

        // Guard arrival
        if (data[i].second > 0) { thisGuardID = data[i].second; }

        // Start sleeping
        else if (data[i].second == INT_MIN) {
            int goToSleepTime =
                std::chrono::hh_mm_ss(data[i].first - std::chrono::floor<std::chrono::days>(data[i].first))
                    .minutes()
                    .count();
            int wakeUpTime;
            while (true) {
                if (i + 1 == data.size() || data[i + 1].second > 0) {
                    wakeUpTime = std::chrono::minutes(60).count();
                    break;
                }
                else if (data[i + 1].second == INT_MIN) {
                    i++;
                    continue;
                }
                else {
                    wakeUpTime = std::chrono::hh_mm_ss(data[i + 1].first -
                                                       std::chrono::floor<std::chrono::days>(data[i + 1].first))
                                     .minutes()
                                     .count();
                    break;
                }
            }

            std::vector<int> &tempVect = guardToMinutes[thisGuardID];
            for (int j = goToSleepTime; j < wakeUpTime; ++j) { tempVect[j]++; }
        }
    }

    // RESULT PROCESSING
    auto maxEle = std::ranges::max_element(guardToMinutes, [](auto &&a, auto &&b) {
        return std::ranges::fold_left(a.second, 0, std::plus()) < std::ranges::fold_left(b.second, 0, std::plus());
    });

    return (*maxEle).first * (std::ranges::max_element((*maxEle).second, std::less()) - (*maxEle).second.begin());
}

int day4_2(std::string dataFile) {

    // CUSTOM TYPES
    auto dateTime_ctre = ctre::search<R"([^\[]+(?=\]))">;
    auto lastWord_ctre = ctre::search<R"(\w+)">;

    auto VofV = AOC_commons::parseInputUsingCTRE::processFileRPT(dataFile, dateTime_ctre, lastWord_ctre, lastWord_ctre);

    std::chrono::system_clock::time_point                              tm1;
    std::vector<std::pair<std::chrono::system_clock::time_point, int>> data;
    std::istringstream                                                 ss;
    std::unordered_map<int, std::vector<int>>                          guardToMinutes;

    // DATA PREP
    for (auto &line : VofV) {
        line.front()[0] = '2';
        line.front()[1] = '0';
        ss.str(line.front());
        std::chrono::from_stream(ss, "%Y-%m-%d %H:%M", tm1);
        int ins;
        if (line.back() == "asleep") { ins = INT_MIN; }
        else if (line.back() == "up") { ins = 0; }
        else {
            ins = std::stoi(line[2]);
            guardToMinutes.emplace(ins, std::vector<int>(60, 0));
        }


        data.push_back(std::make_pair(tm1, ins));
    }
    std::ranges::sort(data, [](auto &&a, auto &&b) { return a.first < b.first; });

    // MAIN LOGIC
    int thisGuardID;
    for (int i = 0; i < data.size(); ++i) {

        // Guard arrival
        if (data[i].second > 0) { thisGuardID = data[i].second; }

        // Start sleeping
        else if (data[i].second == INT_MIN) {
            int goToSleepTime =
                std::chrono::hh_mm_ss(data[i].first - std::chrono::floor<std::chrono::days>(data[i].first))
                    .minutes()
                    .count();
            int wakeUpTime;
            while (true) {
                if (i + 1 == data.size() || data[i + 1].second > 0) {
                    wakeUpTime = std::chrono::minutes(60).count();
                    break;
                }
                else if (data[i + 1].second == INT_MIN) {
                    i++;
                    continue;
                }
                else {
                    wakeUpTime = std::chrono::hh_mm_ss(data[i + 1].first -
                                                       std::chrono::floor<std::chrono::days>(data[i + 1].first))
                                     .minutes()
                                     .count();
                    break;
                }
            }

            std::vector<int> &tempVect = guardToMinutes[thisGuardID];
            for (int j = goToSleepTime; j < wakeUpTime; ++j) { tempVect[j]++; }
        }
    }

    // RESULT PROCESSING
    auto maxEle = std::ranges::max_element(guardToMinutes, [](auto &&a, auto &&b) {
        return *std::ranges::max_element(a.second, std::less()) < *std::ranges::max_element(b.second, std::less());
    });

    return (*maxEle).first * (std::ranges::max_element((*maxEle).second, std::less()) - (*maxEle).second.begin());
}
} // namespace AOC2018