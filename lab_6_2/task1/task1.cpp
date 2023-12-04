#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

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

// Структура для хранения обработчиков событий
struct EventSystem {
    std::unordered_map<EventHandle, std::vector<EventHandler>> eventHandlers;
    std::unordered_map<EventHandle, std::string> eventNames;
    EventHandle nextEventId = 1;  // Идентификатор следующего события

    // Функция для создания нового события
    EventHandle createEvent(const std::string& eventName) {
        eventNames[nextEventId] = eventName;
        return nextEventId++;
    }

    // Функция для подписки обработчика на событие
    EventHandlerHandle subscribe(EventHandle eventId, EventHandlerFunc&& func) {
        auto& handlers = eventHandlers[eventId];
        EventHandlerHandle handle = handlers.empty() ? 1 : (handlers.back().id + 1);
        handlers.push_back({ handle, std::move(func) });
        return handle;
    }

    // Функция для отписки обработчика от события
    bool unsubscribe(EventHandle eventId, EventHandlerHandle handle) {
        auto& handlers = eventHandlers[eventId];
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
        auto& handlers = eventHandlers[eventId];
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
