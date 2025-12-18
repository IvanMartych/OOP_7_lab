#pragma once
#include "visitor.h"
#include "npc.h"

// посетитель для обработки боевой логики между NPC
class CombatVisitor : public Visitor {
public:
    // проверка, может ли атакующий убить защищающегося
    bool canKill(Npc* attacker, Npc* defender);

    void visit(Knight&) override {}
    void visit(Squirrel&) override {}
    void visit(Pegasus&) override {}

private:
    bool knightVs(const std::string& defenderType);
    bool squirrelVs(const std::string& defenderType);
    bool pegasusVs(const std::string& defenderType);
};