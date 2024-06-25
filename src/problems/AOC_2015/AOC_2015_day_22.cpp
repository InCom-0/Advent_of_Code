#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {

/*
Implemented through sort of a functional style.
Using lambdas as both 'Actions' a player can take as well as 'Effects' and other 'Events'.
Technically extensible design capable of easily accomodating all sort of actions or effects the player or the boss can take.

Recursive approach is used to find the optimum.
Not sure how one would design a systematically better algo for something like this.

Note: No doubt less than optimal (to put it mildly) as the algo investigates all possibilities one by one.
Note: Technically possible to get an endless loop if no solution exists.
Note: Could be improved quite a bit by computing repetitive 'subtrees' only once and reuse the results
(ie. mana below recharge cost => known finite & comparativelly small number of possibilities which will always be the same.)

Note: Basically designed as an excercise to familirize myself with how one could create a barebones simple 'event' based system
*/
int day22_1(int Ehp, int Edmg, int hp, int mana) {
    struct gameState {
        int myHP;
        int myArmor;
        int myMana;

        int bossHP;
        int bossDMG;
        int bossArmor;

        int curSpent;
        int round;

        bool poisoned;
        bool recharging;
        bool shielded;
    };

    gameState curState{hp, 0, mana, Ehp, Edmg, 0, 0, -1, false, false, false};

    int possibilitiesInvestigated = 0;
    int minimumSpent = INT_MAX;
    std::vector<int> current;
    std::vector<std::function<void()> > actions;

    std::vector<int> minimumHistory;
    std::vector<std::vector<int>> sequenceHistory;

    struct ET {
        std::vector<std::vector<std::function<void(int)> > > insideTable;
        void checkAndAddUntil(int endID) {
            while (insideTable.size() < endID+1) insideTable.push_back(std::vector<std::function<void(int)>>());
        }
        void addEvents(const std::function<void(int)> &toAdd, int startID, int endID) {
            for (int i = startID; i <= endID; ++i) insideTable[i].push_back(toAdd);
        }
        void removeEvents(const std::function<void(int)> &toRemove, int startID, int endID) {
            for (int i = startID; i <= endID; ++i) insideTable[i].pop_back();
        }
    };
    ET eventTable;

    auto poisonEffect = [&](int antiReverse) { curState.bossHP -= 3 * antiReverse; };
    auto poisonStartEnd = [&](int antiReverse) { curState.poisoned ^= true;};

    auto rechargeEffect = [&](int antiReverse) { curState.myMana += 101 * antiReverse; };
    auto rechargeStartEnd = [&](int antiReverse) { curState.recharging ^= true;};

    auto shieldEffectStart = [&](int antiReverse) {
        curState.myArmor += 7 * antiReverse;
        curState.shielded ^= true;
    };
    auto shieldEffectEnd = [&](int antiReverse) {
        curState.myArmor -= 7 * antiReverse;
        curState.shielded ^= true;
    };


    auto testConditions = [&]() {
        possibilitiesInvestigated++;
        if (curState.myMana < 0) {return 0;}
        if (curState.curSpent > minimumSpent) {return 0;}
        if (curState.bossHP <= 0) {
            minimumSpent = std::min(minimumSpent, curState.curSpent);
            minimumHistory.push_back(curState.curSpent);
            sequenceHistory.push_back(current);
            return 0;
        }
        if (curState.myHP <= 0) {return 0;}
        possibilitiesInvestigated--;
        return 1;
    };

    auto playerRound = [&]() {
        curState.round++;
        eventTable.checkAndAddUntil(curState.round);
        for (auto event: eventTable.insideTable[curState.round]) event(1);
        if (testConditions()) for (auto nextAction: actions) nextAction();
        for (auto event: eventTable.insideTable[curState.round]) event(-1);
        curState.round--;
    };
    auto bossAction = [&] (int antiReverse) {
        curState.myHP -= (curState.bossDMG-curState.myArmor)*antiReverse;
    };
    auto bossRound = [&]() -> void {
        curState.round++;
        eventTable.checkAndAddUntil(curState.round);
        for (auto event: eventTable.insideTable[curState.round]) event(1);
        bossAction(1);

        if (testConditions()) playerRound();

        bossAction(-1);
        for (auto event: eventTable.insideTable[curState.round]) event(-1);
        curState.round--;
    };


    auto missile = [&]() -> void {
        curState.bossHP -= 4;
        curState.myMana -= 53;
        curState.curSpent += 53;
        current.push_back(1);

        if (testConditions()) bossRound();

        current.pop_back();
        curState.bossHP += 4;
        curState.myMana += 53;
        curState.curSpent -= 53;
    };
    auto drain = [&]() -> void {
        curState.bossHP -= 2;
        curState.myHP += 2;
        curState.myMana -= 73;
        curState.curSpent += 73;
        current.push_back(2);

        if (testConditions()) bossRound();

        current.pop_back();
        curState.bossHP += 2;
        curState.myHP -= 2;
        curState.myMana += 73;
        curState.curSpent -= 73;
    };
    auto shield = [&]() -> void {
        if (curState.shielded) return;

        curState.myMana -= 113;
        curState.curSpent += 113;
        eventTable.checkAndAddUntil(curState.round + 6);
        eventTable.addEvents(shieldEffectStart, curState.round + 1, curState.round + 1);
        eventTable.addEvents(shieldEffectEnd, curState.round + 6, curState.round + 6);

        current.push_back(3);

        if (testConditions()) bossRound();

        current.pop_back();

        eventTable.removeEvents(shieldEffectEnd, curState.round + 6, curState.round + 6);
        eventTable.removeEvents(shieldEffectStart, curState.round + 1, curState.round + 1);
        curState.myMana += 113;
        curState.curSpent -= 113;
    };
    auto poison = [&]() -> void {
        if (curState.poisoned) return;

        curState.myMana -= 173;
        curState.curSpent += 173;
        eventTable.checkAndAddUntil(curState.round + 6);
        eventTable.addEvents(poisonStartEnd, curState.round +1, curState.round +1);
        eventTable.addEvents(poisonEffect, curState.round + 1, curState.round + 6);
        eventTable.addEvents(poisonStartEnd, curState.round +6, curState.round +6);

        current.push_back(4);

        if (testConditions()) bossRound();

        current.pop_back();

        eventTable.removeEvents(poisonStartEnd, curState.round + 6, curState.round + 6);
        eventTable.removeEvents(poisonEffect, curState.round + 1, curState.round + 6);
        eventTable.removeEvents(poisonStartEnd, curState.round + 1, curState.round + 1);

        curState.myMana += 173;
        curState.curSpent -= 173;
        return;
    };
    auto recharge = [&]() -> void {
        if (curState.recharging) return;

        curState.myMana -= 229;
        curState.curSpent += 229;
        eventTable.checkAndAddUntil(curState.round + 5);
        eventTable.addEvents(rechargeStartEnd, curState.round +1, curState.round +1);
        eventTable.addEvents(rechargeEffect, curState.round + 1, curState.round + 5);
        eventTable.addEvents(rechargeStartEnd, curState.round +5, curState.round +5);

        current.push_back(5);

        if (testConditions()) bossRound();

        current.pop_back();

        eventTable.removeEvents(rechargeStartEnd, curState.round + 5, curState.round + 5);
        eventTable.removeEvents(rechargeEffect, curState.round + 1, curState.round + 5);
        eventTable.removeEvents(rechargeStartEnd, curState.round + 1, curState.round + 1);

        curState.myMana += 229;
        curState.curSpent -= 229;
        return;
    };

    actions.push_back(missile);
    actions.push_back(drain);
    actions.push_back(shield);
    actions.push_back(poison);
    actions.push_back(recharge);

    playerRound();

    return minimumSpent;
}
int day22_2(int Ehp, int Edmg, int hp, int mana) {
    struct gameState {
        int myHP;
        int myArmor;
        int myMana;

        int bossHP;
        int bossDMG;
        int bossArmor;

        int curSpent;
        int round;

        bool poisoned;
        bool recharging;
        bool shielded;
    };

    gameState curState{hp, 0, mana, Ehp, Edmg, 0, 0, -1, false, false, false};

    int possibilitiesInvestigated = 0;
    int minimumSpent = INT_MAX;
    std::vector<int> current;
    std::vector<std::function<void()> > actions;

    std::vector<int> minimumHistory;
    std::vector<std::vector<int>> sequenceHistory;

    struct ET {
        std::vector<std::vector<std::function<void(int)> > > insideTable;
        void checkAndAddUntil(int endID) {
            while (insideTable.size() < endID+1) insideTable.push_back(std::vector<std::function<void(int)>>());
        }
        void addEvents(const std::function<void(int)> &toAdd, int startID, int endID) {
            for (int i = startID; i <= endID; ++i) insideTable[i].push_back(toAdd);
        }
        void removeEvents(const std::function<void(int)> &toRemove, int startID, int endID) {
            for (int i = startID; i <= endID; ++i) insideTable[i].pop_back();
        }
    };
    ET eventTable;

    auto poisonEffect = [&](int antiReverse) { curState.bossHP -= 3 * antiReverse; };
    auto poisonStartEnd = [&](int antiReverse) { curState.poisoned ^= true;};

    auto rechargeEffect = [&](int antiReverse) { curState.myMana += 101 * antiReverse; };
    auto rechargeStartEnd = [&](int antiReverse) { curState.recharging ^= true;};

    auto shieldEffectStart = [&](int antiReverse) {
        curState.myArmor += 7 * antiReverse;
        curState.shielded ^= true;
    };
    auto shieldEffectEnd = [&](int antiReverse) {
        curState.myArmor -= 7 * antiReverse;
        curState.shielded ^= true;
    };


    auto testConditions = [&]() {
        possibilitiesInvestigated++;
        if (curState.myMana < 0) {return 0;}
        if (curState.curSpent > minimumSpent) {return 0;}
        if (curState.bossHP <= 0) {
            minimumSpent = std::min(minimumSpent, curState.curSpent);
            minimumHistory.push_back(curState.curSpent);
            sequenceHistory.push_back(current);
            return 0;
        }
        if (curState.myHP <= 0) {return 0;}
        possibilitiesInvestigated--;
        return 1;
    };

    auto playerRound = [&]() {
        curState.round++;
        eventTable.checkAndAddUntil(curState.round);
        curState.myHP--;
        for (auto event: eventTable.insideTable[curState.round]) event(1);
        if (testConditions()) for (auto nextAction: actions) nextAction();
        for (auto event: eventTable.insideTable[curState.round]) event(-1);
        curState.myHP++;
        curState.round--;
    };
    auto bossAction = [&] (int antiReverse) {
        curState.myHP -= (curState.bossDMG-curState.myArmor)*antiReverse;
    };
    auto bossRound = [&]() -> void {
        curState.round++;
        eventTable.checkAndAddUntil(curState.round);
        for (auto event: eventTable.insideTable[curState.round]) event(1);
        bossAction(1);

        if (testConditions()) playerRound();

        bossAction(-1);
        for (auto event: eventTable.insideTable[curState.round]) event(-1);
        curState.round--;
    };


    auto missile = [&]() -> void {
        curState.bossHP -= 4;
        curState.myMana -= 53;
        curState.curSpent += 53;
        current.push_back(1);

        if (testConditions()) bossRound();

        current.pop_back();
        curState.bossHP += 4;
        curState.myMana += 53;
        curState.curSpent -= 53;
    };
    auto drain = [&]() -> void {
        curState.bossHP -= 2;
        curState.myHP += 2;
        curState.myMana -= 73;
        curState.curSpent += 73;
        current.push_back(2);

        if (testConditions()) bossRound();

        current.pop_back();
        curState.bossHP += 2;
        curState.myHP -= 2;
        curState.myMana += 73;
        curState.curSpent -= 73;
    };
    auto shield = [&]() -> void {
        if (curState.shielded) return;

        curState.myMana -= 113;
        curState.curSpent += 113;
        eventTable.checkAndAddUntil(curState.round + 6);
        eventTable.addEvents(shieldEffectStart, curState.round + 1, curState.round + 1);
        eventTable.addEvents(shieldEffectEnd, curState.round + 6, curState.round + 6);

        current.push_back(3);

        if (testConditions()) bossRound();

        current.pop_back();

        eventTable.removeEvents(shieldEffectEnd, curState.round + 6, curState.round + 6);
        eventTable.removeEvents(shieldEffectStart, curState.round + 1, curState.round + 1);
        curState.myMana += 113;
        curState.curSpent -= 113;
    };
    auto poison = [&]() -> void {
        if (curState.poisoned) return;

        curState.myMana -= 173;
        curState.curSpent += 173;
        eventTable.checkAndAddUntil(curState.round + 6);
        eventTable.addEvents(poisonStartEnd, curState.round +1, curState.round +1);
        eventTable.addEvents(poisonEffect, curState.round + 1, curState.round + 6);
        eventTable.addEvents(poisonStartEnd, curState.round +6, curState.round +6);

        current.push_back(4);

        if (testConditions()) bossRound();

        current.pop_back();

        eventTable.removeEvents(poisonStartEnd, curState.round + 6, curState.round + 6);
        eventTable.removeEvents(poisonEffect, curState.round + 1, curState.round + 6);
        eventTable.removeEvents(poisonStartEnd, curState.round + 1, curState.round + 1);

        curState.myMana += 173;
        curState.curSpent -= 173;
        return;
    };
    auto recharge = [&]() -> void {
        if (curState.recharging) return;

        curState.myMana -= 229;
        curState.curSpent += 229;
        eventTable.checkAndAddUntil(curState.round + 5);
        eventTable.addEvents(rechargeStartEnd, curState.round +1, curState.round +1);
        eventTable.addEvents(rechargeEffect, curState.round + 1, curState.round + 5);
        eventTable.addEvents(rechargeStartEnd, curState.round +5, curState.round +5);

        current.push_back(5);

        if (testConditions()) bossRound();

        current.pop_back();

        eventTable.removeEvents(rechargeStartEnd, curState.round + 5, curState.round + 5);
        eventTable.removeEvents(rechargeEffect, curState.round + 1, curState.round + 5);
        eventTable.removeEvents(rechargeStartEnd, curState.round + 1, curState.round + 1);

        curState.myMana += 229;
        curState.curSpent -= 229;
        return;
    };

    actions.push_back(missile);
    actions.push_back(drain);
    actions.push_back(shield);
    actions.push_back(poison);
    actions.push_back(recharge);

    playerRound();

    return minimumSpent;
}

}