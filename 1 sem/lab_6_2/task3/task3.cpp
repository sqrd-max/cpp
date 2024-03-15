#include <iostream>
#include <functional>
#include <vector>
#include <cmath>

// Структура события для математического pipeline
struct MathEvent {
    double number;
};

// Тип обработчика события для математического pipeline
using MathHandler = std::function<void(MathEvent&)>;

// Структура для хранения обработчиков событий математического pipeline'а
struct MathEventSystem {
    std::vector<MathHandler> mathEventHandlers;

    // Функция для подписки обработчика на событие математического pipeline'а
    void subscribeMathEvent(MathHandler handler) {
        mathEventHandlers.push_back(handler);
    }

    // Функция для диспетчеризации события математического pipeline'а
    void dispatchMathEvent(MathEvent& mathEvent) {
        for (const auto& handler : mathEventHandlers) {
            handler(mathEvent);
        }
    }
};

int main() {
    MathEventSystem mathEventSystem;

    // Пример обработчиков событий для математического pipeline'а
    MathHandler addTwo = [](MathEvent& mathEvent) {
        mathEvent.number += 2.0;
    };

    MathHandler multiplyByThree = [](MathEvent& mathEvent) {
        mathEvent.number *= 3.0;
    };

    MathHandler takeSquareRoot = [](MathEvent& mathEvent) {
        mathEvent.number = std::sqrt(mathEvent.number);
    };

    // Подписываем обработчики на событие математического pipeline'а
    mathEventSystem.subscribeMathEvent(addTwo);
    mathEventSystem.subscribeMathEvent(multiplyByThree);
    mathEventSystem.subscribeMathEvent(takeSquareRoot);

    // Моделируем событие для математического pipeline'а (например, число, которое нужно обработать)
    MathEvent mathEventData = {16.0};

    // Диспетчеризуем событие и применяем все обработчики
    mathEventSystem.dispatchMathEvent(mathEventData);

    // Выводим результат после обработки события
    std::cout << "Processed Number: " << mathEventData.number << std::endl;

    return 0;
}
