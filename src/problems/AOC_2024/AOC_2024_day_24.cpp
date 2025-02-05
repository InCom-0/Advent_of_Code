#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <fmt/format.h>
#include <incom_commons.h>


namespace AOC2024 {
namespace incc = incom::commons;

size_t day24_1(std::string dataFile) {

    enum class BinOpEnum {
        SIMPLE,
        AND,
        OR,
        XOR
    };

    struct Gate {

        BinOpEnum           op{BinOpEnum::SIMPLE};
        std::optional<bool> val = std::optional<bool>();

        std::reference_wrapper<Gate> left  = (*this);
        std::reference_wrapper<Gate> right = (*this);

        void computeVal() {
            switch (op) {
                case BinOpEnum::AND:    val = left.get().getVal() & right.get().getVal(); break;
                case BinOpEnum::OR:     val = left.get().getVal() | right.get().getVal(); break;
                case BinOpEnum::XOR:    val = left.get().getVal() ^ right.get().getVal(); break;
                case BinOpEnum::SIMPLE: break;
            }
        };

        bool getVal() {
            if (not val) { computeVal(); }
            return val.value();
        }
    };

    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    ankerl::unordered_dense::segmented_map<std::string, Gate, incc::XXH3Hasher> mapOfGates;

    for (auto const &line : input) {
        if (line.size() == 2) {
            mapOfGates.insert({line.front(), Gate{BinOpEnum::SIMPLE, line.back().front() == '1' ? true : false}});
        }
        else if (line.size() == 4) {
            if (line[1] == "AND") { mapOfGates.insert({line.back(), Gate{BinOpEnum::AND}}); }
            else if (line[1] == "OR") { mapOfGates.insert({line.back(), Gate{BinOpEnum::OR}}); }
            else if (line[1] == "XOR") { mapOfGates.insert({line.back(), Gate{BinOpEnum::XOR}}); }
        }
    }
    for (auto const &line : input) {
        if (line.size() == 4) {
            auto &it = mapOfGates.at(line.back());
            it.left  = mapOfGates.at(line.front());
            it.right = mapOfGates.at(line[2]);
        }
    }
    std::vector<std::pair<std::string, bool>> resVector;
    for (auto &gateInMap : mapOfGates) {
        if (gateInMap.first.front() == 'z') { resVector.push_back({gateInMap.first, gateInMap.second.getVal()}); }
    }

    std::ranges::sort(resVector,
                      [](auto &&a, auto &&b) { return std::ranges::lexicographical_compare(a.first, b.first); });

    size_t ans = 0;
    for (size_t i = 0; auto const &resItem : resVector) { ans |= (static_cast<size_t>(resItem.second) << i++); }

    return ans;
}

std::string day24_2(std::string dataFile) {

    enum class BinOpEnum {
        SIMPLE,
        AND,
        OR,
        XOR
    };

    struct Gate {
        std::string gateName;

        BinOpEnum           op{BinOpEnum::SIMPLE};
        std::optional<bool> val = std::optional<bool>();

        std::reference_wrapper<Gate> left  = (*this);
        std::reference_wrapper<Gate> right = (*this);
    };

    auto w_ctre = ctre::search<R"(\w+)">;
    auto input  = incc::parseInputUsingCTRE::processFileRPT(dataFile, w_ctre);

    // Map with stable references
    ankerl::unordered_dense::segmented_map<std::string, Gate, incc::XXH3Hasher> mapOfGates;

    // Put all the gates in the map
    for (auto const &line : input) {
        if (line.size() == 2) {
            mapOfGates.insert(
                {line.front(), Gate{line.front(), BinOpEnum::SIMPLE, line.back().front() == '1' ? true : false}});
        }
        else if (line.size() == 4) {
            if (line[1] == "AND") { mapOfGates.insert({line.back(), Gate{line.back(), BinOpEnum::AND}}); }
            else if (line[1] == "OR") { mapOfGates.insert({line.back(), Gate{line.back(), BinOpEnum::OR}}); }
            else if (line[1] == "XOR") { mapOfGates.insert({line.back(), Gate{line.back(), BinOpEnum::XOR}}); }
        }
    }

    // Set the 'left' and 'right' to the right reference of some gate already inside the mapOfGates map
    for (auto const &line : input) {
        if (line.size() == 4) {
            auto &it = mapOfGates.at(line.back());
            it.left  = mapOfGates.at(line.front());
            it.right = mapOfGates.at(line[2]);
        }
    }

    std::vector<std::string> incorrect_gates;
    auto                     eva_zGateCorrectness = [&](Gate const &z_gateRef) -> void {
        // If first two or last one -> don't evaluate
        if (z_gateRef.gateName == "z00" || z_gateRef.gateName == "z01" || z_gateRef.gateName == "z45") { return; }

        // If is not actually a XOR gate
        if (z_gateRef.op != BinOpEnum::XOR) { incorrect_gates.push_back(z_gateRef.gateName); }

        // Evaluation function from the x__ and y__ at this level
        auto eva_XORtoZgate = [&](Gate const &XORtoZgate) -> void {
            if ((XORtoZgate.left.get().gateName.front() == 'x' || XORtoZgate.right.get().gateName.front() == 'x') &&
                (XORtoZgate.left.get().gateName.front() == 'y' || XORtoZgate.right.get().gateName.front() == 'y')) {}
            else { incorrect_gates.push_back(XORtoZgate.gateName); }
        };

        auto eva_ORtoZgate = [&](Gate const &ORtoZgate) -> void {
            if (ORtoZgate.left.get().op != BinOpEnum::AND) { incorrect_gates.push_back(ORtoZgate.left.get().gateName); }
            if (ORtoZgate.right.get().op != BinOpEnum::AND) {
                incorrect_gates.push_back(ORtoZgate.right.get().gateName);
            }
        };


        if (z_gateRef.left.get().op == BinOpEnum::XOR) {
            if (z_gateRef.right.get().op == BinOpEnum::OR) {
                eva_XORtoZgate(z_gateRef.left.get());
                eva_ORtoZgate(z_gateRef.right.get());
            }
            else { incorrect_gates.push_back(z_gateRef.right.get().gateName); }
        }
        else if (z_gateRef.left.get().op == BinOpEnum::OR) {
            if (z_gateRef.right.get().op == BinOpEnum::XOR) {
                eva_XORtoZgate(z_gateRef.right.get());
                eva_ORtoZgate(z_gateRef.left.get());
            }
            else { incorrect_gates.push_back(z_gateRef.right.get().gateName); }
        }
        else { incorrect_gates.push_back(z_gateRef.gateName); }
    };

    for (const auto &[_, one_gate] : mapOfGates) {
        if (one_gate.gateName.front() == 'z') { eva_zGateCorrectness(one_gate); }
    }

    std::ranges::sort(incorrect_gates, std::less());
    auto [firstIT, endIT] = std::ranges::unique(incorrect_gates);
    incorrect_gates.erase(firstIT, endIT);

    std::string ans = "";
    for (auto const &item : incorrect_gates) { ans += (item + ','); }
    ans.pop_back();
    return ans;
}
} // namespace AOC2024