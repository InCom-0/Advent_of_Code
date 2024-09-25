#include <fstream>

#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {

int day21_1(int hp, int dmg, int armor, int myHP, std::string dataFile) {
    struct equip {
        int         cost   = 0;
        int         damage = 0;
        int         armor  = 0;
        std::string name   = "";
    };

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string        oneStr;
    std::vector<equip> weaponsL;
    std::vector<equip> armorL;
    std::vector<equip> ringL;

    std::vector<equip> toSort;

    for (int i = 0; i < 5; ++i) {
        std::getline(iStream, oneStr);
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());

        equip toInsert;
        toInsert.name = re.to_string();

        bg            = re.get_end_position();
        re            = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.cost = re.to_number();

        bg              = re.get_end_position();
        re              = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.damage = re.to_number();

        bg             = re.get_end_position();
        re             = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.armor = re.to_number();

        weaponsL.push_back(toInsert);
    }
    for (int i = 0; i < 5; ++i) {
        std::getline(iStream, oneStr);
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());

        equip toInsert;
        toInsert.name = re.to_string();

        bg            = re.get_end_position();
        re            = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.cost = re.to_number();

        bg              = re.get_end_position();
        re              = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.damage = re.to_number();

        bg             = re.get_end_position();
        re             = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.armor = re.to_number();

        armorL.push_back(toInsert);
    }
    for (int i = 0; i < 6; ++i) {
        std::getline(iStream, oneStr);
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+ \+\d)">(bg, oneStr.end());

        equip toInsert;
        toInsert.name = re.to_string();

        bg            = re.get_end_position();
        re            = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.cost = re.to_number();

        bg              = re.get_end_position();
        re              = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.damage = re.to_number();

        bg             = re.get_end_position();
        re             = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.armor = re.to_number();

        ringL.push_back(toInsert);
    }

    armorL.push_back(equip{0, 0, 0, "EMPTY"});
    ringL.push_back(equip{0, 0, 0, "EMPTY"});
    ringL.push_back(equip{0, 0, 0, "EMPTY"});

    for (int i = 0; i < ringL.size(); ++i) {
        for (int j = i + 1; j < ringL.size(); ++j) {
            toSort.push_back(equip{ringL[i].cost + ringL[j].cost, ringL[i].damage + ringL[j].damage,
                                   ringL[i].armor + ringL[j].armor, ringL[i].name + "_" + ringL[j].name});
        }
    }

    std::sort(toSort.begin(), toSort.end(),
              [](auto &a, auto &b) { return a.damage * 64 + a.armor < b.damage * 64 + b.armor; });

    toSort = flux::ref(toSort)
                 .chunk_by([](auto &&a, auto &&b) { return a.damage * 64 + a.armor == b.damage * 64 + b.armor; })
                 .map([](auto &&a) {
                     return *std::min_element(a.begin(), a.end(),
                                              [](auto &&yyy, auto &&zzz) { return yyy.cost < zzz.cost; });
                 })
                 .to<std::vector<equip>>();

    toSort = flux::cartesian_product_map(
                 [](auto &&a, auto &&b, auto &&c) {
                     return equip{a.cost + b.cost + c.cost, a.damage + b.damage + c.damage, a.armor + b.armor + c.armor,
                                  a.name + "_" + b.name + "_" + c.name};
                 },
                 flux::ref(weaponsL), flux::ref(armorL), std::move(toSort))
                 .to<std::vector<equip>>();

    std::sort(toSort.begin(), toSort.end(),
              [](auto &a, auto &b) { return a.damage * 64 + a.armor < b.damage * 64 + b.armor; });

    auto ans = flux::ref(toSort)
                   .chunk_by([](auto &&a, auto &&b) { return a.damage * 64 + a.armor == b.damage * 64 + b.armor; })
                   .map([](auto &&a) {
                       return *std::min_element(a.begin(), a.end(),
                                                [](auto &&yyy, auto &&zzz) { return yyy.cost < zzz.cost; });
                   })
                   .filter([&](auto &&a) {
                       int dmgPerGiven    = std::max(1, a.damage - armor);
                       int dmgPerReceived = std::max(1, dmg - a.armor);

                       if ((hp - 1) / dmgPerGiven <= (myHP - 1) / dmgPerReceived) { return true; }
                       return false;
                   })
                   .min([](auto &&a, auto &&b) { return a.cost < b.cost; })
                   .value_or(equip());

    return ans.cost;
}

int day21_2(int hp, int dmg, int armor, int myHP, std::string dataFile) {
    struct equip {
        int         cost   = 0;
        int         damage = 0;
        int         armor  = 0;
        std::string name   = "";
    };

    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string        oneStr;
    std::vector<equip> weaponsL;
    std::vector<equip> armorL;
    std::vector<equip> ringL;

    std::vector<equip> toSort;

    for (int i = 0; i < 5; ++i) {
        std::getline(iStream, oneStr);
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());

        equip toInsert;
        toInsert.name = re.to_string();

        bg            = re.get_end_position();
        re            = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.cost = re.to_number();

        bg              = re.get_end_position();
        re              = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.damage = re.to_number();

        bg             = re.get_end_position();
        re             = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.armor = re.to_number();

        weaponsL.push_back(toInsert);
    }
    for (int i = 0; i < 5; ++i) {
        std::getline(iStream, oneStr);
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+)">(bg, oneStr.end());

        equip toInsert;
        toInsert.name = re.to_string();

        bg            = re.get_end_position();
        re            = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.cost = re.to_number();

        bg              = re.get_end_position();
        re              = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.damage = re.to_number();

        bg             = re.get_end_position();
        re             = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.armor = re.to_number();

        armorL.push_back(toInsert);
    }
    for (int i = 0; i < 6; ++i) {
        std::getline(iStream, oneStr);
        auto bg = oneStr.begin();
        auto re = ctre::search<R"(\w+ \+\d)">(bg, oneStr.end());

        equip toInsert;
        toInsert.name = re.to_string();

        bg            = re.get_end_position();
        re            = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.cost = re.to_number();

        bg              = re.get_end_position();
        re              = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.damage = re.to_number();

        bg             = re.get_end_position();
        re             = ctre::search<R"(\d+)">(bg, oneStr.end());
        toInsert.armor = re.to_number();

        ringL.push_back(toInsert);
    }

    armorL.push_back(equip{0, 0, 0, "EMPTY"});
    ringL.push_back(equip{0, 0, 0, "EMPTY"});
    ringL.push_back(equip{0, 0, 0, "EMPTY"});

    for (int i = 0; i < ringL.size(); ++i) {
        for (int j = i + 1; j < ringL.size(); ++j) {
            toSort.push_back(equip{ringL[i].cost + ringL[j].cost, ringL[i].damage + ringL[j].damage,
                                   ringL[i].armor + ringL[j].armor, ringL[i].name + "_" + ringL[j].name});
        }
    }

    std::sort(toSort.begin(), toSort.end(),
              [](auto &a, auto &b) { return a.damage * 64 + a.armor < b.damage * 64 + b.armor; });

    toSort = flux::ref(toSort)
                 .chunk_by([](auto &&a, auto &&b) { return a.damage * 64 + a.armor == b.damage * 64 + b.armor; })
                 .map([](auto &&a) {
                     return *std::max_element(a.begin(), a.end(),
                                              [](auto &&yyy, auto &&zzz) { return yyy.cost < zzz.cost; });
                 })
                 .to<std::vector<equip>>();

    toSort = flux::cartesian_product_map(
                 [](auto &&a, auto &&b, auto &&c) {
                     return equip{a.cost + b.cost + c.cost, a.damage + b.damage + c.damage, a.armor + b.armor + c.armor,
                                  a.name + "_" + b.name + "_" + c.name};
                 },
                 flux::ref(weaponsL), flux::ref(armorL), std::move(toSort))
                 .to<std::vector<equip>>();

    std::sort(toSort.begin(), toSort.end(),
              [](auto &a, auto &b) { return a.damage * 64 + a.armor < b.damage * 64 + b.armor; });

    auto ans = flux::ref(toSort)
                   .chunk_by([](auto &&a, auto &&b) { return a.damage * 64 + a.armor == b.damage * 64 + b.armor; })
                   .map([](auto &&a) {
                       return *std::max_element(a.begin(), a.end(),
                                                [](auto &&yyy, auto &&zzz) { return yyy.cost < zzz.cost; });
                   })
                   .filter([&](auto &&a) {
                       int dmgPerGiven    = std::max(1, a.damage - armor);
                       int dmgPerReceived = std::max(1, dmg - a.armor);

                       if ((hp - 1) / dmgPerGiven <= (myHP - 1) / dmgPerReceived) { return false; }
                       return true;
                   })
                   .max([](auto &&a, auto &&b) { return a.cost < b.cost; })
                   .value_or(equip());

    return ans.cost;
}
} // namespace AOC2015