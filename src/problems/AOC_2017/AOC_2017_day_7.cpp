#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>

namespace AOC2017 {
std::string day7_1(std::string dataFile) {
    auto wrd_ctre       = ctre::search<R"(\w+)">;
    auto weight_ctre    = ctre::search<R"((?<=\()\d+)">;
    auto discOnTop_ctre = ctre::search<R"((?<=-> ).+)">;

    auto VofV = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, wrd_ctre, weight_ctre, discOnTop_ctre);
    std::vector<std::vector<std::string>> VVVVV;

    for (auto &line : VofV) {
        if (line.size() == 3) {
            auto const VofS = incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(line.back(), wrd_ctre);
            line.pop_back();
            for (auto &itemOnTop : VofS) { line.push_back(itemOnTop); }
        }
    }

    std::unordered_map<std::string, int, incom::commons::XXH3Hasher> UOmap;
    for (auto &line : VofV) {
        for (int i = 2; i < line.size(); ++i) { UOmap[line[i]]++; }
        UOmap[line[0]]++;
    }
    auto fluxMap = flux::from_range(UOmap);
    auto res     = fluxMap.find_min([](auto &&a, auto &&b) { return a.second < b.second; });

    return flux::read_at(fluxMap, res).first;
}

/*
 * Created as an experiment to learn whether it is reasonable to implement a 'tree-like' structure using references
 * while the data is stored in a contiguous vector.
 *
 * CONCLUSION: It is pretty good.
 * One just has to take care of how the references (using a wrapper) get passed around.
 *
 *
 */
int day7_2(std::string dataFile) {
    auto wrd_ctre       = ctre::search<R"(\w+)">;
    auto weight_ctre    = ctre::search<R"((?<=\()\d+)">;
    auto discOnTop_ctre = ctre::search<R"((?<=-> ).+)">;

    // INPUT PROCESSING
    auto VofV = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, wrd_ctre, weight_ctre, discOnTop_ctre);

    for (auto &line : VofV) {
        if (line.size() == 3) {
            auto const VofS = incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(line.back(), wrd_ctre);
            line.pop_back();
            for (auto &itemOnTop : VofS) { line.push_back(itemOnTop); }
        }
    }

    // VARIABLE PREP
    struct prog {
        std::string                               myName;
        int                                       myWeight = INT_MIN;
        std::vector<std::reference_wrapper<prog>> onTop    = std::vector<std::reference_wrapper<prog>>();
    };

    std::vector<prog>                                                                                      progStorage;
    std::unordered_map<std::string, std::pair<std::reference_wrapper<prog>, int>, incom::commons::XXH3Hasher> mp;
    progStorage.reserve(VofV.size()); // To ensure no reallocations are ever made.

    // CLOSURE DEFINITIONS
    auto createOrRefer = [&](std::string &progToCreate) -> std::reference_wrapper<prog> {
        auto fnd = mp.find(progToCreate);
        if (fnd != mp.end()) { return fnd->second.first; }
        else {
            progStorage.push_back(prog(progToCreate));
            mp.emplace(progToCreate, std::pair<std::reference_wrapper<prog>, int>(progStorage.back(), 1));
            return progStorage.back();
        }
        std::unreachable();
    };
    auto updateProg = [&](prog &toUpdate, std::vector<std::string> &updateWith) {
        toUpdate.myWeight            = std::stoi(updateWith[1]);
        mp.at(updateWith[0]).second += 1;
        for (auto iter = updateWith.begin() + 2; iter != updateWith.end(); ++iter) {
            toUpdate.onTop.push_back(createOrRefer(iter.operator*()));
            mp.at(iter.operator*()).second += 1;
        }
    };

    /*
     * The lambda below performs what is essentially a DFS.
     * A lot of nonsense is required to handle the edge case of strictly 2 'programs' on the 'incorrect' level ...
     * ... impossible to know whether the higher one is the right one or the lower one is. That is decided by the level
     * below.
     *
     * Could probably avoid this by inverting the algo (that is start from the 'leaves' not the 'root') AND using BFS
     * instead of DFS (naturally).
     */
    std::vector<int> ans;
    auto             evaluateIncorrectWeight = [&](this auto &self, prog &prog2Evaluate) -> int {
        std::vector<int> evaResult;
        for (auto &ref2prog : prog2Evaluate.onTop) { evaResult.push_back(self(ref2prog.get())); }
        auto flux_evaResult = flux::ref(evaResult);
        if (evaResult.size() > 1) {
            auto [min, max] = flux_evaResult.find_minmax([](auto &&a, auto &&b) { return a < b; });
            if (flux::read_at(flux_evaResult, min) != flux::read_at(flux_evaResult, max)) {
                if (flux_evaResult.count_if([&, m = flux::read_at(flux_evaResult, min)](auto &&a) { return a == m; }) ==
                    1) {
                    auto refToAns = prog2Evaluate.onTop[flux::distance(flux_evaResult, flux_evaResult.first(), min)];
                    ans.push_back(refToAns.get().myWeight);
                    ans.push_back(refToAns.get().myWeight +
                                  (flux::read_at(flux_evaResult, max) - flux::read_at(flux_evaResult, min)));
                    refToAns.get().myWeight = flux::read_at(flux_evaResult, max);
                    evaResult[flux::distance(flux_evaResult, flux_evaResult.first(), min)] =
                        flux::read_at(flux_evaResult, max);
                }
                else {
                    auto refToAns = prog2Evaluate.onTop[flux::distance(flux_evaResult, flux_evaResult.first(), max)];
                    ans.push_back(refToAns.get().myWeight);
                    ans.push_back(refToAns.get().myWeight -
                                  (flux::read_at(flux_evaResult, max) - flux::read_at(flux_evaResult, min)));
                    refToAns.get().myWeight = flux::read_at(flux_evaResult, min);
                    evaResult[flux::distance(flux_evaResult, flux_evaResult.first(), max)] =
                        flux::read_at(flux_evaResult, min);
                }
            }
        }
        return prog2Evaluate.myWeight + flux_evaResult.fold([](auto &&ini, auto &&a) { return ini + a; }, 0);
    };

    // MAIN LOGIC
    for (auto &line : VofV) { updateProg(createOrRefer(line.front()), line); }
    std::string startName = std::min_element(mp.begin(), mp.end(), [](auto &&a, auto &&b) {
                                return a.second.second < b.second.second;
                            })->first;
    evaluateIncorrectWeight(mp.at(startName).first.get());

    if (ans.size() == 2) { return ans.back(); }
    else if (ans.size() == 4) { return ans.front(); }
    else { return INT_MIN; }
    std::unreachable();
}

} // namespace AOC2017