#include "../include/arena.h"
#include "../include/factory.h"
#include "../include/combat_visitor.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>

// конструктор с валидацией границ
Arena::Arena(int width, int height) : gameRunning_(false) {
    if (width > MAX_WIDTH || height > MAX_HEIGHT) {
        throw std::out_of_range("Arena dimensions exceed maximum limits.");
    }
    this->width_ = width;
    this->height_ = height;
}

// добавление NPC с валидацией
void Arena::addNpc(std::unique_ptr<Npc> npc) {
    const std::string name = npc->getName();

    // проверка границ
    if (npc->getX() < 0 || npc->getX() > width_ ||
        npc->getY() < 0 || npc->getY() > height_) {
        throw std::out_of_range("NPC coordinates out of bounds.");
    }

    // проверка на дубликаты имен
    if (npcs_.find(name) != npcs_.end()) {
        throw std::invalid_argument("NPC with this name already exists.");
    }
    
    npcs_[name] = std::move(npc);
}

void Arena::createAndAddNpc(const std::string& type, 
                            const std::string& name, 
                            int x, int y) {
    addNpc(NpcFactory::createNpc(type, name, x, y));
}

// вывод всех NPC, находящихся на арене
void Arena::printAllNpcs() const {
    for (const auto& pair : npcs_) {
        std::cout << *(pair.second) << std::endl;
    }
}

// возврат текущего количества NPC
size_t Arena::getNpcCount() const {
    return npcs_.size();
}

// сохранение NPC в файл
void Arena::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    for (const auto& [name, npc] : npcs_) {
        file << npc->getType() << " "
             << npc->getName() << " "
             << npc->getX() << " "
             << npc->getY() << std::endl;
    }
}

// загрузка NPC из файла
void Arena::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        auto npc = NpcFactory::createFromString(line);
        addNpc(std::move(npc));
    }
}

// очистка арены
void Arena::clear() {
    npcs_.clear();
}

// управление наблюдателями
void Arena::addObserver(std::shared_ptr<Observer> observer) {
    observers_.push_back(observer);
}

void Arena::removeObserver(std::shared_ptr<Observer> observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end()
    );
}

// уведомление всех наблюдателей
void Arena::notifyObservers(const std::string& event) {
    for (auto& observer : observers_) {
        observer->notify(event);
    }
}

// боевая система: проверка всех пар NPC в пределах дальности
void Arena::startBattle(double range) {
    CombatVisitor visitor;
    std::vector<std::string> toRemove;

    // проверка каждой пары NPC
    for (auto& [name1, npc1] : npcs_) {
        for (auto& [name2, npc2] : npcs_) {
            if (name1 == name2) continue;
            if (name1 > name2) continue; // избегаем дублирования пар

            // проверка, находятся ли в пределах дальности боя
            if (npc1->distanceTo(*npc2) > range) continue;
            
            // проверка боя в обоих направлениях
            bool npc1KillsNpc2 = visitor.canKill(npc1.get(), npc2.get());
            bool npc2KillsNpc1 = visitor.canKill(npc2.get(), npc1.get());
            
            if (npc1KillsNpc2 && npc2KillsNpc1) {
                // взаимное убийство
                std::string event = npc1->getName() + " (" + npc1->getType() + 
                                   ") and " + npc2->getName() + " (" + npc2->getType() + 
                                   ") killed each other";
                notifyObservers(event);
                toRemove.push_back(name1);
                toRemove.push_back(name2);
            } else if (npc1KillsNpc2) {
                // только npc1 убивает npc2
                std::string event = npc1->getName() + " (" + npc1->getType() + 
                                   ") killed " + npc2->getName() + " (" + npc2->getType() + ")";
                notifyObservers(event);
                toRemove.push_back(name2);
            } else if (npc2KillsNpc1) {
                // только npc2 убивает npc1
                std::string event = npc2->getName() + " (" + npc2->getType() + 
                                   ") killed " + npc1->getName() + " (" + npc1->getType() + ")";
                notifyObservers(event);
                toRemove.push_back(name1);
            }
        }
    }
    
    // удаление мёртвых NPC (сначала устраняем дубликаты)
    std::sort(toRemove.begin(), toRemove.end());
    toRemove.erase(std::unique(toRemove.begin(), toRemove.end()), toRemove.end());
    
    for (const auto& name : toRemove) {
        npcs_.erase(name);
    }
}

// ==================== МЕТОДЫ ДЛЯ 7-Й ЛАБЫ ====================

// генерация случайных NPC
void Arena::generateRandomNpcs(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, MAP_WIDTH);
    std::uniform_int_distribution<> yDist(0, MAP_HEIGHT);
    std::uniform_int_distribution<> typeDist(0, 2);
    
    std::vector<std::string> types = {"Knight", "Squirrel", "Pegasus"};
    
    for (int i = 0; i < count; ++i) {
        std::string type = types[typeDist(gen)];
        std::string name = type + "_" + std::to_string(i + 1);
        int x = xDist(gen);
        int y = yDist(gen);
        
        try {
            createAndAddNpc(type, name, x, y);
        } catch (const std::exception& e) {
            // пропускаем дубликаты или ошибки
        }
    }
}

// получение списка выживших
std::vector<Npc*> Arena::getSurvivors() const {
    std::shared_lock<std::shared_mutex> lock(npcsMutex_);
    std::vector<Npc*> survivors;
    
    for (const auto& [name, npc] : npcs_) {
        if (npc->isAlive()) {
            survivors.push_back(npc.get());
        }
    }
    
    return survivors;
}

// печать карты 100x100
void Arena::printMap() const {
    std::shared_lock<std::shared_mutex> lock(npcsMutex_);
    
    // создаем пустую карту 100x100
    std::vector<std::vector<char>> map(MAP_HEIGHT + 1, std::vector<char>(MAP_WIDTH + 1, '.'));
    
    // заполняем живыми NPC
    for (const auto& [name, npc] : npcs_) {
        if (npc->isAlive()) {
            int x = npc->getX();
            int y = npc->getY();
            
            if (x >= 0 && x <= MAP_WIDTH && y >= 0 && y <= MAP_HEIGHT) {
                char symbol = '.';
                if (npc->getType() == "Knight") symbol = 'K';
                else if (npc->getType() == "Squirrel") symbol = 'S';
                else if (npc->getType() == "Pegasus") symbol = 'P';
                
                map[y][x] = symbol;
            }
        }
    }
    
    // вывод полной карты 100x100
    std::cout << "\n=== MAP (100x100) ===" << std::endl;
    for (int y = 0; y <= MAP_HEIGHT; ++y) {
        for (int x = 0; x <= MAP_WIDTH; ++x) {
            std::cout << map[y][x];
        }
        std::cout << std::endl;
    }
    std::cout << "K=Knight, S=Squirrel, P=Pegasus" << std::endl;
}

// поток движения NPC
void Arena::movementThreadFunc() {
    while (gameRunning_.load()) {
        {
            std::unique_lock<std::shared_mutex> lock(npcsMutex_);
            
            // двигаем всех живых NPC
            for (auto& [name, npc] : npcs_) {
                if (npc->isAlive()) {
                    npc->moveRandom(MAP_WIDTH, MAP_HEIGHT);
                }
            }
            
            // проверяем столкновения и добавляем в очередь боев
            for (auto& [name1, npc1] : npcs_) {
                if (!npc1->isAlive()) continue;
                
                for (auto& [name2, npc2] : npcs_) {
                    if (!npc2->isAlive()) continue;
                    if (name1 >= name2) continue; // избегаем дубликатов
                    
                    double distance = npc1->distanceTo(*npc2);
                    int killDist = npc1->getKillDistance();
                    
                    if (distance <= killDist) {
                        std::lock_guard<std::mutex> qlock(combatQueueMutex_);
                        combatQueue_.push({name1, name2});
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// поток боевой системы
void Arena::combatThreadFunc() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dice(1, 6);
    
    while (gameRunning_.load()) {
        std::pair<std::string, std::string> combatPair;
        bool hasCombat = false;
        
        {
            std::lock_guard<std::mutex> qlock(combatQueueMutex_);
            if (!combatQueue_.empty()) {
                combatPair = combatQueue_.front();
                combatQueue_.pop();
                hasCombat = true;
            }
        }
        
        if (hasCombat) {
            std::shared_lock<std::shared_mutex> rlock(npcsMutex_);
            
            auto it1 = npcs_.find(combatPair.first);
            auto it2 = npcs_.find(combatPair.second);
            
            if (it1 != npcs_.end() && it2 != npcs_.end()) {
                Npc* npc1 = it1->second.get();
                Npc* npc2 = it2->second.get();
                
                if (npc1->isAlive() && npc2->isAlive()) {
                    CombatVisitor visitor;
                    
                    bool npc1CanKill = visitor.canKill(npc1, npc2);
                    bool npc2CanKill = visitor.canKill(npc2, npc1);
                    
                    rlock.unlock();
                    
                    if (npc1CanKill || npc2CanKill) {
                        int attackRoll = dice(gen);
                        int defenseRoll = dice(gen);
                        
                        if (attackRoll > defenseRoll) {
                            if (npc1CanKill) {
                                npc2->kill();
                                std::string event = npc1->getName() + " (" + npc1->getType() + 
                                                   ") killed " + npc2->getName() + " (" + npc2->getType() + 
                                                   ") [Attack:" + std::to_string(attackRoll) + 
                                                   " > Defense:" + std::to_string(defenseRoll) + "]";
                                notifyObservers(event);
                            } else if (npc2CanKill) {
                                npc1->kill();
                                std::string event = npc2->getName() + " (" + npc2->getType() + 
                                                   ") killed " + npc1->getName() + " (" + npc1->getType() + 
                                                   ") [Attack:" + std::to_string(attackRoll) + 
                                                   " > Defense:" + std::to_string(defenseRoll) + "]";
                                notifyObservers(event);
                            }
                        }
                    }
                }
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

// поток печати карты
void Arena::printThreadFunc() {
    while (gameRunning_.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        {
            std::lock_guard<std::mutex> lock(observerMutex_);
            printMap();
            
            std::shared_lock<std::shared_mutex> rlock(npcsMutex_);
            int aliveCount = 0;
            for (const auto& [name, npc] : npcs_) {
                if (npc->isAlive()) aliveCount++;
            }
            std::cout << "Alive NPCs: " << aliveCount << " / " << npcs_.size() << std::endl;
        }
    }
}

// запуск игры на заданное время
void Arena::startGame(int durationSeconds) {
    gameRunning_.store(true);
    
    std::cout << "Starting game for" << std::endl;
    std::cout << "Initial NPCs: " << npcs_.size() << std::endl;
    
    // запускаем потоки
    std::thread movementThread(&Arena::movementThreadFunc, this);
    std::thread combatThread(&Arena::combatThreadFunc, this);
    std::thread printThread(&Arena::printThreadFunc, this);
    
    // ждем указанное время
    std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
    
    // останавливаем игру
    gameRunning_.store(false);
    
    // ждем завершения потоков
    movementThread.join();
    combatThread.join();
    printThread.join();
    
}