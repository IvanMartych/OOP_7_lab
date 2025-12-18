#include "include/arena.h"
#include "include/factory.h"
#include "include/console_observer.h"
#include "include/file_observer.h"
#include <iostream>
#include <memory>

int main() {
    try {
        std::cout << "=== LAB 7 - Multithreaded RPG Arena ===" << std::endl;
        std::cout << std::endl;

        // Создаем арену 100x100
        Arena arena(100, 100);

        // Добавляем наблюдателей
        auto consoleObserver = std::make_shared<ConsoleObserver>();
        auto fileObserver = std::make_shared<FileObserver>("battle_log.txt");
        
        arena.addObserver(consoleObserver);
        arena.addObserver(fileObserver);

        // Генерируем 50 случайных NPC
        std::cout << "Generating 50 random NPCs..." << std::endl;
        arena.generateRandomNpcs(50);
        std::cout << "Total NPCs created: " << arena.getNpcCount() << std::endl;
        std::cout << std::endl;

        std::cout << "Initial NPCs:" << std::endl;
        arena.printAllNpcs();
        std::cout << std::endl;

        // Запускаем игру на 30 секунд
        std::cout << "Starting game with 3 threads:" << std::endl;
        std::cout << "  - Movement thread (moving NPCs every 100ms)" << std::endl;
        std::cout << "  - Combat thread (processing battles with dice rolls)" << std::endl;
        std::cout << "  - Print thread (displaying map every 1 second)" << std::endl;
        std::cout << std::endl;
        
        arena.startGame(30);

        // Выводим список выживших
        std::cout << "\n=== SURVIVORS ===" << std::endl;
        auto survivors = arena.getSurvivors();
        std::cout << "Survivors: " << survivors.size() << " / " << arena.getNpcCount() << std::endl;
        
        for (const auto* npc : survivors) {
            std::cout << "  - " << npc->getName() << " (" << npc->getType() << ")"
                     << " at (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
        }

        std::cout << "\n=== Program completed successfully ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
