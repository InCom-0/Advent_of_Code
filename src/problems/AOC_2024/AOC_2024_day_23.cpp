#include <ankerl/unordered_dense.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;
long long day23_1(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::set<std::string, incc::XXH3Hasher>         uniqued;
    ankerl::unordered_dense::map<std::string, size_t, incc::XXH3Hasher> mpOfIDs;


    for (auto const &line : input) {
        for (auto const &computer : line) { uniqued.insert(computer); }
    }

    for (size_t i = 0; auto &uniqueComp : uniqued) { mpOfIDs.insert({uniqueComp, i++}); }

    std::vector links(uniqued.size(), bit::bit_vector<unsigned char>(uniqued.size(), bit::bit0));
    for (auto const &line : input) {
        links.at(mpOfIDs.at(line.front())).at(mpOfIDs.at(line.back())) = bit::bit1;
        links.at(mpOfIDs.at(line.back())).at(mpOfIDs.at(line.front())) = bit::bit1;
    }

    ankerl::unordered_dense::set<std::vector<size_t>, incc::XXH3Hasher> uniqueThrees;

    for (size_t startID = 0; startID < links.size(); ++startID) {
        for (size_t midID = startID + 1; midID < links.size(); ++midID) {

            if (links.at(startID).at(midID) == bit::bit1) {
                for (size_t lastID = midID + 1; lastID < links.size(); ++lastID) {

                    if (links.at(midID).at(lastID) == bit::bit1) {
                        if (links.at(lastID).at(startID) == bit::bit1) {
                            std::vector<size_t> toInput{startID, midID, lastID};
                            std::ranges::sort(toInput, std::less());
                            uniqueThrees.insert(std::move(toInput));
                        }
                    }
                }
            }
        }
    }
    long long ans = 0;
    for (auto const &resLine : uniqueThrees) {
        for (auto const &resItemID : resLine) {
            if ((uniqued.begin() + resItemID)->front() == 't') {
                ans++;
                break;
            }
        }
    }

    return ans;
}

std::string day23_2(std::string dataFile) {
    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::set<std::string, incc::XXH3Hasher>         uniqued;
    ankerl::unordered_dense::map<std::string, size_t, incc::XXH3Hasher> mpOfIDs;


    for (auto const &line : input) {
        for (auto const &computer : line) { uniqued.insert(computer); }
    }

    for (size_t i = 0; auto &uniqueComp : uniqued) { mpOfIDs.insert({uniqueComp, i++}); }

    std::vector links(uniqued.size(), bit::bit_vector<unsigned char>(uniqued.size(), bit::bit0));
    for (auto const &line : input) {
        links.at(mpOfIDs.at(line.front())).at(mpOfIDs.at(line.back())) = bit::bit1;
        links.at(mpOfIDs.at(line.back())).at(mpOfIDs.at(line.front())) = bit::bit1;
    }

    std::vector<size_t> curStack;
    std::vector<size_t> result;

    auto rec_finder = [&](this auto const &self, size_t curID) -> void {
        while (++curID < links.size()) {
            if (std::ranges::all_of(curStack,
                                    [&](auto const &item) { return links.at(curID).at(item) == bit::bit1; })) {
                curStack.push_back(curID);

                if (curStack.size() > result.size()) { result = curStack; }

                self(curID);

                curStack.pop_back();
            }
        }
    };

    for (size_t firstID = 0; firstID < links.size(); ++firstID) {

        curStack.push_back(firstID);

        rec_finder(firstID);

        curStack.pop_back();
    }

    std::vector<std::string> ansVec;
    for (auto const &resID : result) { ansVec.push_back(*(uniqued.begin() + resID)); }

    std::ranges::sort(ansVec, [](auto &&a, auto &&b) { return std::ranges::lexicographical_compare(a, b); });

    std::string ans;
    for (auto const &compName : ansVec) {
        ans.append(compName);
        ans.push_back(',');
    }
    ans.pop_back();

    return ans;
}
} // namespace AOC2024