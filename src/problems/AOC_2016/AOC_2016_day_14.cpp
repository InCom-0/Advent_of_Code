#include <deque>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <md5.h>


namespace AOC2016 {
int day14_1(std::string salt, const int keysToProduce, const int keyStretchCount) {
    struct hashInQueue {
        std::string AAAhash;
        std::string strToFindInAAAAAhash;
        int         removeOnNumToAdd;
        bool        foundAAAAAhash = false;
    };

    auto AAActre = ctre::search<R"((.)\g{1}\g{1})">;
    MD5  md5;

    std::vector<hashInQueue> foundHashes;
    std::deque<hashInQueue>  que;
    int                      numToAdd = -1;

    while (foundHashes.size() < keysToProduce) {
        numToAdd++;
        std::string current = salt + std::to_string(numToAdd);
        for (int i = 0; i < keyStretchCount; ++i) { current = md5(current); }

        for (auto &hiq : que) {
            if (not hiq.foundAAAAAhash && current.contains(hiq.strToFindInAAAAAhash)) {
                hiq.foundAAAAAhash       = true;
                hiq.strToFindInAAAAAhash = current;
            }
        }
        auto rgx = AAActre(current);
        if (rgx) {
            std::string tmpString = rgx.to_string();
            tmpString.push_back(tmpString[0]);
            tmpString.push_back(tmpString[0]);
            que.push_back(hashInQueue(current, tmpString, numToAdd + 1000, false));
        }

        if (not que.empty()) {
            if (que.front().foundAAAAAhash) { foundHashes.push_back(que.front()); }
            if (que.front().foundAAAAAhash || que.front().removeOnNumToAdd == numToAdd) { que.pop_front(); }
        }
    }
    return foundHashes.back().removeOnNumToAdd - 1000;
}
} // namespace AOC2016
