#pragma once
#include "observer.h"
#include <fstream>
#include <string>

// наблюдатель, который логирует события в файл
class FileObserver : public Observer {
public:
    FileObserver(const std::string& filename) : filename_(filename) {}

    void notify(const std::string& event) override {
        std::ofstream file(filename_, std::ios::app);
        if (file.is_open()) {
            file << event << std::endl;
        }
    }

private:
    std::string filename_;
};