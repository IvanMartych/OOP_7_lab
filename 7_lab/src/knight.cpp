#include "../include/knight.h"
#include "../include/visitor.h"
#include <iostream>

const std::string Knight::type_ = "Knight";

Knight::Knight(int x, int y, const std::string& name)
    : Npc(x, y, type_, name) 
{
    // конструктор странствующего рыцаря
}

void Knight::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Knight::printInfo() const {
    std::cout << "Wandering Knight: " << getName()
              << " @ (" << getX() << ", " << getY() << ")"
              << std::endl;
}