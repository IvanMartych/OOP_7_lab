#include "../include/combat_visitor.h"

// определение, может ли атакующий убить защищающегося
bool CombatVisitor::canKill(Npc* attacker, Npc* defender) {
    if (attacker->getType() == "Knight") {
        return knightVs(defender->getType());
    } else if (attacker->getType() == "Squirrel") {
        return squirrelVs(defender->getType());
    } else if (attacker->getType() == "Pegasus") {
        return pegasusVs(defender->getType());
    }
    return false;
}

// рыцарь убивает белок
bool CombatVisitor::knightVs(const std::string& defenderType) {
    return (defenderType == "Squirrel");
}

// белка убивает пегасов
bool CombatVisitor::squirrelVs(const std::string& defenderType) {
    return (defenderType == "Pegasus");
}

// пегас никого не атакует
bool CombatVisitor::pegasusVs(const std::string& /* defenderType */) {
    return false;
}