#pragma once
#include <string>
#include "npc.h"

// пегас (мирный персонаж)
class Pegasus : public Npc {
private:
    static const std::string type_;

public:
    Pegasus(int x, int y, const std::string& name);
    
    void printInfo() const override;
    void accept(Visitor& visitor) override;
    
    int getMoveDistance() const override { return 30; }
    int getKillDistance() const override { return 10; }
};
