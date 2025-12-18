#include "../include/npc.h"
#include <cmath>
#include <ostream>
#include <iostream>
#include <random>

// реализация конструктора
Npc::Npc(int x, int y, const std::string& type, const std::string& name)
    : x_(x), y_(y), type_(type), name_(name), isAlive_(true) {}

// реализация геттеров для координат и свойств
int Npc::getX() const { 
    return x_; 
}

int Npc::getY() const { 
    return y_; 
}

std::string Npc::getType() const { 
    return type_; 
}

std::string Npc::getName() const { 
    return name_; 
}

// вычисление расстояния по теореме пифагора
double Npc::distanceTo(const Npc& other) const {
    int dx = x_ - other.x_;
    int dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
}

void Npc::printInfo() const {
    std::cout << *this << std::endl;
}

// управление жизнью
bool Npc::isAlive() const {
    return isAlive_.load();
}

void Npc::kill() {
    isAlive_.store(false);
}

// установка позиции
void Npc::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

// случайное движение в пределах карты
void Npc::moveRandom(int maxX, int maxY) {
    if (!isAlive()) return;

    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    
    int moveRange = getMoveDistance();
    std::uniform_int_distribution<> distrib(-moveRange, moveRange);
    
    int newX = x_ + distrib(gen);
    int newY = y_ + distrib(gen);
    
    // проверка границ
    if (newX < 0) newX = 0;
    if (newX > maxX) newX = maxX;
    if (newY < 0) newY = 0;
    if (newY > maxY) newY = maxY;
    
    x_ = newX;
    y_ = newY;
}

// оператор вывода в поток
std::ostream& operator<<(std::ostream& os, const Npc& npc) {
    os << "NPC [" << npc.type_ << "] " << npc.name_
       << " @ (" << npc.x_ << ", " << npc.y_ << ")"
       << (npc.isAlive_.load() ? " [ALIVE]" : " [DEAD]");
    return os;
}