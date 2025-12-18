#pragma once
#include <string>

// интерфейс паттерна наблюдатель для уведомлений о событиях
class Observer {
public:
    virtual ~Observer() = default;

    // метод уведомления, вызываемый при возникновении событий
    virtual void notify(const std::string& event) = 0;
};