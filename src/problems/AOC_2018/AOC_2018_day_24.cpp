#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <unordered_set>


namespace AOC2018 {
struct Group {
    enum class Dmg_T {
        slashing,
        bludgeoning,
        fire,
        cold,
        radiation
    };

    long long m_units;
    long long m_hpPerUnit;
    long long m_damage;
    long long m_initiative;
    size_t    side;
    Dmg_T     m_damageType;

    std::vector<Dmg_T> m_immunities;
    std::vector<Dmg_T> m_weaknesses;


    void attackAnother(Group &otherGroup) {
        size_t dmgMultiplier = 1;
        for (auto &otherOneImun : otherGroup.m_immunities) { dmgMultiplier *= (m_damageType != otherOneImun); }
        for (auto &otherOneWeak : otherGroup.m_weaknesses) { dmgMultiplier *= ((m_damageType == otherOneWeak) + 1); }

        otherGroup.m_units -= ((m_units * m_damage * dmgMultiplier) / otherGroup.m_hpPerUnit);
        if (otherGroup.m_units < 0) { otherGroup.m_units = 0; }
    }
};

struct Game {
    std::vector<Group> m_groups;

    std::vector<std::pair<std::reference_wrapper<Group>, std::reference_wrapper<Group>>> calc_targetSelection() {

        std::vector<std::pair<std::reference_wrapper<Group>, std::reference_wrapper<Group>>> res;
        std::vector<std::pair<std::reference_wrapper<Group>, bool>>                          local_groups;

        for (auto &grp : m_groups) { local_groups.push_back({grp, false}); }

        std::ranges::sort(local_groups, [](auto &&a, auto &&b) {
            if (a.first.get().m_units * a.first.get().m_damage == b.first.get().m_units * b.first.get().m_damage) {
                return a.first.get().m_initiative > b.first.get().m_initiative;
            }
            else {
                return a.first.get().m_units * a.first.get().m_damage > b.first.get().m_units * b.first.get().m_damage;
            }
        });

        for (auto &loc_grp : local_groups) {
            auto fv = std::views::filter(local_groups, [&](auto &&a) {
                return (a.first.get().side != loc_grp.first.get().side && (not a.second));
            });

            auto mx = std::ranges::max_element(fv, [&](auto &&a, auto &&b) {
                size_t dmgMultiplier_a = 1;
                for (auto &otherOneImun : a.first.get().m_immunities) {
                    dmgMultiplier_a *= (loc_grp.first.get().m_damageType != otherOneImun);
                }
                for (auto &otherOneWeak : a.first.get().m_weaknesses) {
                    dmgMultiplier_a *= ((loc_grp.first.get().m_damageType == otherOneWeak) + 1);
                }

                size_t dmgMultiplier_b = 1;
                for (auto &otherOneImun : b.first.get().m_immunities) {
                    dmgMultiplier_b *= (loc_grp.first.get().m_damageType != otherOneImun);
                }
                for (auto &otherOneWeak : b.first.get().m_weaknesses) {
                    dmgMultiplier_b *= ((loc_grp.first.get().m_damageType == otherOneWeak) + 1);
                }


                if (loc_grp.first.get().m_units * loc_grp.first.get().m_damage * dmgMultiplier_a !=
                    loc_grp.first.get().m_units * loc_grp.first.get().m_damage * dmgMultiplier_b) {
                    return loc_grp.first.get().m_units * loc_grp.first.get().m_damage * dmgMultiplier_a <
                           loc_grp.first.get().m_units * loc_grp.first.get().m_damage * dmgMultiplier_b;
                }
                else if (a.first.get().m_units * a.first.get().m_damage !=
                         b.first.get().m_units * b.first.get().m_damage) {
                    return a.first.get().m_units * a.first.get().m_damage <
                           b.first.get().m_units * b.first.get().m_damage;
                }
                else { return a.first.get().m_initiative < b.first.get().m_initiative; }
            });

            if (mx == fv.end()) { continue; }

            size_t dmgMultiplier = 1;
            for (auto &otherOneImun : mx->first.get().m_immunities) {
                dmgMultiplier *= (loc_grp.first.get().m_damageType != otherOneImun);
            }

            if (loc_grp.first.get().m_units * loc_grp.first.get().m_damage * dmgMultiplier != 0) {
                mx->second = true;
                res.push_back({loc_grp.first, mx->first});
            }
        }

        return res;
    }
    void cleanDeadGroups() {
        std::erase_if(m_groups, [&](auto &&gr) { return gr.m_units == 0; });
    }
    void exe_attacks(
        std::vector<std::pair<std::reference_wrapper<Group>, std::reference_wrapper<Group>>> &targettingVect) {
        std::ranges::sort(targettingVect,
                          [&](auto &&a, auto &&b) { return a.first.get().m_initiative > b.first.get().m_initiative; });

        for (auto &combatPair : targettingVect) { combatPair.first.get().attackAnother(combatPair.second.get()); }
    }

    void exe_oneRound() {
        auto loc_targetting = calc_targetSelection();
        exe_attacks(loc_targetting);

        cleanDeadGroups();
    }
    size_t calc_numOfOriginalGroups() {
        std::unordered_set<size_t> tempSet;
        for (auto &grp : m_groups) { tempSet.insert(grp.side); }
        return tempSet.size();
    }
    size_t calc_numOfsidesAlive() {
        std::unordered_set<size_t> tempSet;
        for (auto &grp : m_groups) {
            if (grp.m_units > 0) { tempSet.insert(grp.side); }
        }
        return tempSet.size();
    }
    long long calc_remainingUnits() {
        return std::ranges::fold_left(m_groups, 0LL, [&](auto &&accu, auto &&grp) { return accu + grp.m_units; });
    }
};


long long day24_1(std::string dataFile) {

    // TYPE DEFS
    std::unordered_map<std::string, int> nameMap{
        {"slashing", 0}, {"bludgeoning", 1}, {"fire", 2}, {"cold", 3}, {"radiation", 4}};

    auto d_ctre = ctre::search<R"(\d+)">;
    auto w_ctre = ctre::search<R"(\w+)">;

    auto weakTo_ctre   = ctre::search<R"((?<=weak to )[^;\)]+)">;
    auto immuneTo_ctre = ctre::search<R"((?<=immune to )[^;\)]+)">;


    // DATA PREP
    auto mainParsed =
        incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre, d_ctre, w_ctre, d_ctre);

    auto weakParsed   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, weakTo_ctre);
    auto immuneParsed = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, immuneTo_ctre);

    Game theGame;

    int nextStart;
    for (int i = 0; i < mainParsed.size(); ++i) {
        if (mainParsed[i].size() != 5) {
            if (mainParsed[i + 1].size() != 5) {
                nextStart = i + 2;
                break;
            }
            continue;
        }
        theGame.m_groups.push_back({std::stoul(mainParsed[i][0]), std::stoul(mainParsed[i][1]),
                                    std::stoul(mainParsed[i][2]), std::stoul(mainParsed[i][4]), 0,
                                    Group::Dmg_T{nameMap.at(mainParsed[i][3])}});

        if (immuneParsed[i].size() != 0) {
            for (auto &oneImun :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(immuneParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_immunities.push_back(Group::Dmg_T{nameMap.at(oneImun)});
            }
        }
        if (weakParsed[i].size() != 0) {
            for (auto &oneWeakness :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(weakParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_weaknesses.push_back(Group::Dmg_T{nameMap.at(oneWeakness)});
            }
        }
    }

    for (int i = nextStart; i < mainParsed.size(); ++i) {
        theGame.m_groups.push_back({std::stoul(mainParsed[i][0]), std::stoul(mainParsed[i][1]),
                                    std::stoul(mainParsed[i][2]), std::stoul(mainParsed[i][4]), 1,
                                    Group::Dmg_T{nameMap.at(mainParsed[i][3])}});
        if (immuneParsed[i].size() != 0) {
            for (auto &oneImun :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(immuneParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_immunities.push_back(Group::Dmg_T{nameMap.at(oneImun)});
            }
        }
        if (weakParsed[i].size() != 0) {
            for (auto &oneWeakness :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(weakParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_weaknesses.push_back(Group::Dmg_T{nameMap.at(oneWeakness)});
            }
        }
    }

    // MAIN LOGIC


    while (theGame.calc_numOfsidesAlive() == 2) { theGame.exe_oneRound(); }

    return theGame.calc_remainingUnits();
}

long long day24_2(std::string dataFile) {
    // TYPE DEFS
    std::unordered_map<std::string, int> nameMap{
        {"slashing", 0}, {"bludgeoning", 1}, {"fire", 2}, {"cold", 3}, {"radiation", 4}};

    auto d_ctre = ctre::search<R"(\d+)">;
    auto w_ctre = ctre::search<R"(\w+)">;

    auto weakTo_ctre   = ctre::search<R"((?<=weak to )[^;\)]+)">;
    auto immuneTo_ctre = ctre::search<R"((?<=immune to )[^;\)]+)">;


    // DATA PREP
    auto mainParsed =
        incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre, d_ctre, d_ctre, w_ctre, d_ctre);

    auto weakParsed   = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, weakTo_ctre);
    auto immuneParsed = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, immuneTo_ctre);

    Game theGame;

    int nextStart;
    for (int i = 0; i < mainParsed.size(); ++i) {
        if (mainParsed[i].size() != 5) {
            if (mainParsed[i + 1].size() != 5) {
                nextStart = i + 2;
                break;
            }
            continue;
        }
        theGame.m_groups.push_back({std::stoul(mainParsed[i][0]), std::stoul(mainParsed[i][1]),
                                    std::stoul(mainParsed[i][2]), std::stoul(mainParsed[i][4]), 0,
                                    Group::Dmg_T{nameMap.at(mainParsed[i][3])}});

        if (immuneParsed[i].size() != 0) {
            for (auto &oneImun :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(immuneParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_immunities.push_back(Group::Dmg_T{nameMap.at(oneImun)});
            }
        }
        if (weakParsed[i].size() != 0) {
            for (auto &oneWeakness :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(weakParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_weaknesses.push_back(Group::Dmg_T{nameMap.at(oneWeakness)});
            }
        }
    }

    for (int i = nextStart; i < mainParsed.size(); ++i) {
        theGame.m_groups.push_back({std::stoul(mainParsed[i][0]), std::stoul(mainParsed[i][1]),
                                    std::stoul(mainParsed[i][2]), std::stoul(mainParsed[i][4]), 1,
                                    Group::Dmg_T{nameMap.at(mainParsed[i][3])}});
        if (immuneParsed[i].size() != 0) {
            for (auto &oneImun :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(immuneParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_immunities.push_back(Group::Dmg_T{nameMap.at(oneImun)});
            }
        }
        if (weakParsed[i].size() != 0) {
            for (auto &oneWeakness :
                 incom::commons::parseInputUsingCTRE::processOneLineRPToneVect(weakParsed[i].front(), w_ctre)) {
                theGame.m_groups.back().m_weaknesses.push_back(Group::Dmg_T{nameMap.at(oneWeakness)});
            }
        }
    }

    Game      theGame_backup = theGame;
    long long addPower       = 0;
    long long lastJump       = 1;

    // MAIN LOGIC
    // This is essentially a binary search ... but with an added twist of searching in potentially 'infinite' range of values.
    do {
        addPower += lastJump;
        lastJump  = addPower;

        theGame = theGame_backup;
        for (auto &grp : theGame.m_groups) {
            if (grp.side == 0) { grp.m_damage += addPower; }
        }

        while (theGame.calc_numOfsidesAlive() == 2) { theGame.exe_oneRound(); }
    } while (theGame.m_groups.back().side == 1);

    addPower -= lastJump / 4;
    lastJump /= 4;
    long long minRemImmuneUnits = LLONG_MAX;
    long long ans = LLONG_MAX;

    do {
        // Prep new game setup (incl. add power)
        theGame = theGame_backup;
        for (auto &grp : theGame.m_groups) {
            if (grp.side == 0) { grp.m_damage += addPower; }
        }

        // Execute the game ... added logic to break a deadlock should it occur
        long long tempRemUnits = LLONG_MAX;
        while (theGame.calc_numOfsidesAlive() == 2) {
            theGame.exe_oneRound();
            if (tempRemUnits == theGame.calc_remainingUnits()) { break; }
            else { tempRemUnits = theGame.calc_remainingUnits(); }
        }

        // Set new addPower value and 
        lastJump /= 2;
        if (theGame.m_groups.back().side == 1) { addPower += lastJump; }
        else {
            if (addPower < ans) {
                ans = addPower;
                minRemImmuneUnits = theGame.calc_remainingUnits();
            }
            addPower -= lastJump;
        }

    } while (lastJump != 0);

    return minRemImmuneUnits;
}
} // namespace AOC2018