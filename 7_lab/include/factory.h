#pragma once
#include <memory>
#include <string>
#include "npc.h"

// фабричный класс npc
class NpcFactory {
public:
    // парсинг из строки
    static std::unique_ptr<Npc> createFromString(const std::string& line);
    
    // создание по параметрам
    static std::unique_ptr<Npc> createNpc(
        const std::string& type,
        const std::string& name,
        int x, 
        int y
    );
};