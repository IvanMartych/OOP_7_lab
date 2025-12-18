#include "../include/pegasus.h"
#include "../include/visitor.h"
#include <iostream>

const std::string Pegasus::type_ = "Pegasus";

Pegasus::Pegasus(int x, int y, const std::string& name)
    : Npc(x, y, type_, name) 
{
    // конструктор пегаса
}

void Pegasus::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Pegasus::printInfo() const {
    std::cout << "Pegasus " << getName()
              << " soaring @ (" << getX() << ", " << getY() << ")"
              << std::endl;
}