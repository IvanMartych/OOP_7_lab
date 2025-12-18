#pragma once
#include "observer.h"
#include <iostream>

// наблюдатель, который логирует события в консоль
class ConsoleObserver : public Observer {
public:
    void notify(const std::string& event) override {
        std::cout << "[COMBAT EVENT] " << event << std::endl;
    }
};