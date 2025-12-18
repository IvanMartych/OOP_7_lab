#pragma once
#include <string>
#include "npc.h"
#include <map>
#include <memory>
#include "observer.h"
#include <vector>
#include <shared_mutex>
#include <mutex>
#include <atomic>
#include <thread>
#include <queue>


#define MAX_WIDTH 500
#define MAX_HEIGHT 500
#define MAP_WIDTH 100
#define MAP_HEIGHT 100

// арена для сражений npc
class Arena {
private:
    int width_, height_;
    std::map<std::string, std::unique_ptr<Npc>> npcs_;
    std::vector<std::shared_ptr<Observer>> observers_;
    
    // многопоточность
    mutable std::shared_mutex npcsMutex_;
    mutable std::mutex observerMutex_;
    std::atomic<bool> gameRunning_;
    
    // очередь боев
    std::queue<std::pair<std::string, std::string>> combatQueue_;
    std::mutex combatQueueMutex_;

public:
    Arena(int width = MAX_WIDTH, int height = MAX_HEIGHT);

    // работа с файлами
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    // управление наблюдателями
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);

    // боевая механика
    void startBattle(double range);

    // добавление npc
    void addNpc(std::unique_ptr<Npc> npc);
    void createAndAddNpc(const std::string& type, 
                         const std::string& name, 
                         int x, int y);

    // информация и очистка
    void printAllNpcs() const;
    size_t getNpcCount() const;
    void clear();

    void notifyObservers(const std::string& event);
    
    // многопоточные методы для 7й лабы
    void generateRandomNpcs(int count);
    void startGame(int durationSeconds);
    void printMap() const;
    std::vector<Npc*> getSurvivors() const;
    
private:
    // функции потоков
    void movementThreadFunc();
    void combatThreadFunc();
    void printThreadFunc();
};