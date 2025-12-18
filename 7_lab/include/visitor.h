#pragma once

// предварительные объявления
class Knight;
class Squirrel;
class Pegasus;

// паттерн посетитель для операций с NPC
class Visitor {
public:
    virtual ~Visitor() = default;

    // методы посещения для каждого типа NPC
    virtual void visit(Knight& knight) = 0;
    virtual void visit(Squirrel& squirrel) = 0;
    virtual void visit(Pegasus& pegasus) = 0;
};
