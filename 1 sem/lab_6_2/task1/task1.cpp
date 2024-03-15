#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <string_view>

// Информация о событии
struct EventInfo {
    void* context;  // Контекст или дополнительные данные
};

// Идентификатор обработчика
using EventHandlerHandle = size_t;

// Тип функции обработчика
using EventHandlerFunc = std::function<void(EventInfo*)>;

// Структура обработчика события
struct EventHandler {
    size_t id;
    EventHandlerFunc func;
};

// Идентификатор события
using EventHandle = size_t;

struct NamedEvent
{
    std::string name;
    std::vector<EventHandler> eventHandlers;
};

// Структура для хранения обработчиков событий
struct EventSystem {
    std::vector<NamedEvent> events;
    EventHandle nextEventId = 0;  // Идентификатор следующего события

    // Функция для создания нового события
    EventHandle createEvent(std::string_view eventName) {
        NamedEvent event = {std::string(eventName), {}};
        events.push_back(std::move(event));
        
        return nextEventId++;
    }

    // Функция для подписки обработчика на событие
    EventHandlerHandle subscribe(EventHandle eventId, EventHandlerFunc&& func) {
        // Проверка существования события с eventId
        if (eventId >= events.size()) {
            // Обработка ошибки, например, вывод сообщения об ошибке
            std::cerr << "Error: Event with id " << eventId << " does not exist." << std::endl;
            return 0; // или какое-то другое значение-метку ошибки
        }

        auto& handlers = events[eventId].eventHandlers;
        EventHandlerHandle handle = handlers.empty() ? 1 : (handlers.back().id + 1);
        handlers.push_back({ handle, std::move(func) });
        return handle;
    }

    // Функция для отписки обработчика от события
    bool unsubscribe(EventHandle eventId, EventHandlerHandle handle) {
        // Проверка существования события с eventId
        if (eventId >= events.size()) {
            // Обработка ошибки, например, вывод сообщения об ошибке
            std::cerr << "Error: Event with id " << eventId << " does not exist." << std::endl;
            return false;
        }

        auto& handlers = events[eventId].eventHandlers; 
        auto it = std::remove_if(handlers.begin(), handlers.end(),
                                 [handle](const EventHandler& handler) {
                                     return handler.id == handle;
                                 });
        if (it != handlers.end()) {
            handlers.erase(it, handlers.end());
            return true;
        }
        return false;
    }

    // Функция для диспетчеризации события
    void dispatch(EventHandle eventId, EventInfo* event) {
        // Проверка существования события с eventId
        if (eventId >= events.size()) {
            // Обработка ошибки, например, вывод сообщения об ошибке
            std::cerr << "Error: Event with id " << eventId << " does not exist." << std::endl;
            return;
        }

        auto& handlers = events[eventId].eventHandlers; 
        for (auto& handler : handlers) {
            handler.func(event);
        }
    }
};


int main() {
    EventSystem eventSystem;

    // Создаем события
    EventHandle eventId1 = eventSystem.createEvent("Event1");
    EventHandle eventId2 = eventSystem.createEvent("Event2");

    // Подписываем обработчики
    EventHandlerHandle handle1 = eventSystem.subscribe(eventId1, [](EventInfo* event) {
        std::cout << "Handler 1 for Event1\n";
    });

    EventHandlerHandle handle2 = eventSystem.subscribe(eventId1, [](EventInfo* event) {
        std::cout << "Handler 2 for Event1\n";
    });

    EventHandlerHandle handle3 = eventSystem.subscribe(eventId2, [](EventInfo* event) {
        std::cout << "Handler 1 for Event2\n";
    });

    // Диспетчируем события
    EventInfo eventInfo;
    eventSystem.dispatch(eventId1, &eventInfo);
    eventSystem.dispatch(eventId2, &eventInfo);

    // Отписываем обработчик
    eventSystem.unsubscribe(eventId1, handle1);

    // Диспетчируем событие еще раз
    eventSystem.dispatch(eventId1, &eventInfo);

    return 0;
}
