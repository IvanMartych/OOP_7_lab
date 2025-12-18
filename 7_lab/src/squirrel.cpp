#include "../include/squirrel.h"
#include "../include/visitor.h"
#include <iostream>

const std::string Squirrel::type_ = "Squirrel";

Squirrel::Squirrel(int x, int y, const std::string& name)
    : Npc(x, y, type_, name) 
{
    // конструктор белки
}

void Squirrel::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Squirrel::printInfo() const {
    std::cout << "Squirrel " << getName()
              << " @ position (" << getX() << ", " << getY() << ")"
              << std::endl;
} 