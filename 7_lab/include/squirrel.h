#pragma once
#include <string>
#include "npc.h"

// белка (нападает на пегасов)
class Squirrel : public Npc {
private:
    static const std::string type_;

public:
    Squirrel(int x, int y, const std::string& name);
    
    void printInfo() const override;
    void accept(Visitor& visitor) override;
    
    int getMoveDistance() const override { return 5; }
    int getKillDistance() const override { return 5; }
};