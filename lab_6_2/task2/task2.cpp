#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <sstream>

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

// Примеры функций-обработчиков
void handler1(EventInfo* event) {
    std::cout << "Handler 1 called\n";
}

void handler2(EventInfo* event) {
    std::cout << "Handler 2 called\n";
}

void handler3(EventInfo* event) {
    std::cout << "Handler 3 called\n";
}

// Примеры функций, ожидающих контекст
void addN(EventInfo* event) {
    int n;
    std::cout << "Enter the value of N: ";
    std::cin >> n;
    std::cout << "Adding " << n << " to the context\n";
}

void multiplyByN(EventInfo* event) {
    int n;
    std::cout << "Enter the value of N: ";
    std::cin >> n;
    std::cout << "Multiplying the context by " << n << "\n";
}

void printContext(EventInfo* event) {
    std::cout << "Printing the context: " << event->context << "\n";
}

int main() {
    EventSystem eventSystem;

    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1. Create new event\n";
        std::cout << "2. Subscribe handler to event\n";
        std::cout << "3. Unsubscribe handler from event\n";
        std::cout << "4. Print identifiers of events and handlers\n";
        std::cout << "5. Dispatch event\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string eventName;
                std::cout << "Enter the name of the event: ";
                std::cin.ignore();  // Очищаем символ новой строки из буфера
                std::getline(std::cin, eventName);
                EventHandle eventId = eventSystem.createEvent(eventName);
                std::cout << "Created event with ID: " << eventId << "\n";
                break;
            }
            case 2: {
                std::cout << "Enter the ID of the event to subscribe to: ";
                EventHandle eventId;
                std::cin >> eventId;

                std::cout << "Choose handler:\n";
                std::cout << "1. Handler 1\n";
                std::cout << "2. Handler 2\n";
                std::cout << "3. Handler 3\n";
                std::cout << "4. Add N to context\n";
                std::cout << "5. Multiply context by N\n";
                std::cout << "6. Print context\n";
                std::cout << "Enter your choice: ";

                int handlerChoice;
                std::cin >> handlerChoice;

                EventHandlerFunc handlerFunc;
                switch (handlerChoice) {
                    case 1:
                        handlerFunc = handler1;
                        break;
                    case 2:
                        handlerFunc = handler2;
                        break;
                    case 3:
                        handlerFunc = handler3;
                        break;
                    case 4:
                        handlerFunc = addN;
                        break;
                    case 5:
                        handlerFunc = multiplyByN;
                        break;
                    case 6:
                        handlerFunc = printContext;
                        break;
                    default:
                        std::cout << "Invalid choice\n";
                        continue;
                }

                EventHandlerHandle handlerId = eventSystem.subscribe(eventId, std::move(handlerFunc));
                std::cout << "Subscribed handler with ID " << handlerId << " to event " << eventId << "\n";
                break;
            }
            case 3: {
                std::cout << "Enter the ID of the event: ";
                EventHandle eventId;
                std::cin >> eventId;

                std::cout << "Enter the ID of the handler to unsubscribe: ";
                EventHandlerHandle handlerId;
                std::cin >> handlerId;

                if (eventSystem.unsubscribe(eventId, handlerId)) {
                    std::cout << "Unsubscribed handler " << handlerId << " from event " << eventId << "\n";
                } else {
                    std::cout << "Handler not found in the event\n";
                }
                break;
            }
            case 4: {
                std::cout << "Event identifiers:\n";
                for (const auto& pair : eventSystem.eventNames) {
                    std::cout << pair.first << ": " << pair.second << "\n";
                }

                std::cout << "Handler identifiers in events:\n";
                for (const auto& pair : eventSystem.eventHandlers) {
                    std::cout << "Event " << pair.first << " handlers:\n";
                    for (const auto& handler : pair.second) {
                        std::cout << "  " << handler.id << "\n";
                    }
                }
                break;
            }
            case 5: {
                std::cout << "Enter the ID of the event to dispatch: ";
                EventHandle eventId;
                std::cin >> eventId;

                EventInfo eventInfo;
                // For simplicity, context is assumed to be an integer in this
                    
                eventInfo.context = new int(42);  // Пример контекста

                eventSystem.dispatch(eventId, &eventInfo);

                // Освобождаем выделенную память для контекста
                delete static_cast<int*>(eventInfo.context);
                break;
            }
            case 6:
                return 0;
            default:
                std::cout << "Invalid choice\n";
                break;
        }
    }
    
}
