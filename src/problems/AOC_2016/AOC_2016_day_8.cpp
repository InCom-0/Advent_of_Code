#include <fstream>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

#include "AOC_2016_day_8.h"

namespace AOC2016 {


int day8_1(std::string &&dataFile) {
    auto disp = day8_2(std::forward<decltype(dataFile)>(dataFile));

    auto ans = flux::ref(disp).fold(
        [](auto &&init, auto &&a) {
            return init + flux::ref(a).fold([](auto &&init2, auto &&aa) { return init2 + aa; }, 0);
        },
        0);

    return ans;
}

std::vector<std::vector<int>> day8_2(std::string &&dataFile) {
    struct Display {
        std::vector<std::vector<int>> disp = std::vector<std::vector<int>>();

        void turnTLrectON(int &colSize, int &rowSize) {
            for (int i = 0; i < rowSize; ++i) {
                for (int j = 0; j < colSize; ++j) { disp[i][j] = 1; }
            }
        }
        void shiftRowBy(int &rowID, int shiftBy) {
            shiftBy           = shiftBy % disp[rowID].size();
            int rotatePointID = shiftBy < 0 ? (-1) * shiftBy : disp[rowID].size() - shiftBy;
            std::ranges::rotate(disp[rowID].begin(), disp[rowID].begin() + rotatePointID, disp[rowID].end());
        }
        void shiftColBy(int &colID, int shiftBy) {
            int rowsCount                  = disp.size();
            shiftBy                        = shiftBy % rowsCount;
            int              rotatePointID = shiftBy < 0 ? (-1) * shiftBy : disp.size() - shiftBy;
            std::vector<int> colTranp(rowsCount, 0);

            for (int i = 0; i < rowsCount; ++i) { colTranp[i] = disp[i][colID]; }
            std::ranges::rotate(colTranp, colTranp.begin() + rotatePointID);
            for (int i = 0; i < rowsCount; ++i) { disp[i][colID] = colTranp[i]; }
        }
    };

    Display display1{std::vector<std::vector<int>>(6, std::vector<int>(50, 0))};

    struct ActionBase {
        int first = 0;
        int last  = 0;
    };
    struct Rect : ActionBase {};
    struct RotR : ActionBase {};
    struct RotC : ActionBase {};

    std::vector<std::variant<Rect, RotR, RotC>> actionsToTake;

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    std::string oneStr;

    auto beforeX_ctre     = ctre::search<R"(\d+(?=x\d+))">;
    auto afterX_ctre      = ctre::search<R"((?<=\dx)\d+)">;
    auto afterEquals_ctre = ctre::search<R"((?<==)\d+)">;
    auto afterByVal       = ctre::search<R"((?<=by )\d+)">;

    while (std::getline(iStream, oneStr)) {
        if (oneStr.starts_with("rect")) {
            actionsToTake.push_back(Rect{beforeX_ctre(oneStr.begin(), oneStr.end()).to_number(),
                                         afterX_ctre(oneStr.begin(), oneStr.end()).to_number()});
        }
        else if (oneStr.starts_with("rotate row")) {
            actionsToTake.push_back(RotR{afterEquals_ctre(oneStr.begin(), oneStr.end()).to_number(),
                                         afterByVal(oneStr.begin(), oneStr.end()).to_number()});
        }
        else if (oneStr.starts_with("rotate column")) {
            actionsToTake.push_back(RotC{afterEquals_ctre(oneStr.begin(), oneStr.end()).to_number(),
                                         afterByVal(oneStr.begin(), oneStr.end()).to_number()});
        }
        else { return std::vector<std::vector<int>>(); }
    }

    for (auto &oneAction : actionsToTake) {
        std::visit(overloaded{
                       [&](Rect &a) -> void { display1.turnTLrectON(a.first, a.last); },
                       [&](RotR &a) -> void { display1.shiftRowBy(a.first, a.last); },
                       [&](RotC &a) -> void { display1.shiftColBy(a.first, a.last); },
                   },
                   oneAction);
    }

    return std::move(display1.disp);
}

} // namespace AOC2016