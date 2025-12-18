#include "../include/factory.h"
#include "../include/knight.h"
#include "../include/pegasus.h"
#include "../include/squirrel.h"
#include <sstream>
#include <stdexcept>
#include <memory>
#include <map>
#include <functional>

std::unique_ptr<Npc> NpcFactory::createNpc(
    const std::string& type,
    const std::string& name,
    int x,
    int y) {
    
    // таблица создателей для разных типов NPC
    static const std::map<std::string, std::function<std::unique_ptr<Npc>(int, int, const std::string&)>> creators = {
        {"Knight", [](int x, int y, const std::string& n) { return std::make_unique<Knight>(x, y, n); }},
        {"Squirrel", [](int x, int y, const std::string& n) { return std::make_unique<Squirrel>(x, y, n); }},
        {"Pegasus", [](int x, int y, const std::string& n) { return std::make_unique<Pegasus>(x, y, n); }}
    };
    
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(x, y, name);
    }
    
    throw std::invalid_argument("Unknown NPC type: " + type);
}

std::unique_ptr<Npc> NpcFactory::createFromString(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name;
    int x, y;

    iss >> type >> name >> x >> y;
    
    if (iss.fail()) {
        throw std::runtime_error("Failed to read line: " + line);
    }

    return createNpc(type, name, x ,y);
}