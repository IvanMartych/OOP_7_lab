#pragma once
#include <string>
#include <memory>
#include <atomic>

class Visitor;

// абстрактный класс персонажа
class Npc {
private:
    int x_, y_;
    std::string type_;
    std::string name_;
    std::atomic<bool> isAlive_;

public:
    Npc(int x, int y, const std::string& type, const std::string& name);
    virtual ~Npc() = default;

    // печать данных персонажа
    virtual void printInfo() const;
    
    // применение посетителя
    virtual void accept(Visitor& visitor) = 0;

    // получение расстояния до другого npc
    double distanceTo(const Npc& other) const;

    // доступ к полям
    std::string getName() const;
    std::string getType() const;
    int getX() const;
    int getY() const;

    // управление жизнью
    bool isAlive() const;
    void kill();

    // движение
    void setPosition(int x, int y);
    void moveRandom(int maxX, int maxY);

    // параметры NPC (расстояния)
    virtual int getMoveDistance() const = 0;
    virtual int getKillDistance() const = 0;

    // оператор вывода
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc);
};