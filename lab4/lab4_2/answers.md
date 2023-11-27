﻿**Ответы на вопросы**
1. Ошибки, позволяющие совершить процедурная версия по сравнению с OOP версией:

Публичные поля могут быть неправильно изменены извне, нарушая инварианты объекта.
Создание копий при передаче объектов в функции может привести к неэффективности.

2. Какую версию проще понять / использовать? В каких ситуациях? Почему?

Версия с использованием класса, методов и принципов ООП обычно более выразительна, модульна и поддерживает лучшую инкапсуляцию. Она проще понять и использовать, особенно в больших проектах. Процедурный стиль может быть проще в некоторых маленьких задачах, но он менее масштабируем.

3. Итераторы:

Для поддержки итераций foreach мы определили методы begin и end, позволяющие итерироваться по элементам объекта с использованием цикла for (int value : arr).

**3. Encapsulation**

В моем коде энкапуляция реализуется следующим образом:

- Приватные члены: Поля data, length и capacity являются приватными, что означает, что они доступны только внутри класса. Это предотвращает прямой доступ к данным извне и обеспечивает контролируемый доступ через публичные методы.

- Публичные методы: Все операции над данными выполняются через публичные методы класса (add, size, capacity, begin, end). Эти методы предоставляют интерфейс для взаимодействия с объектом, скрывая детали его реализации.

- Конструкторы и деструктор: Конструкторы и деструкторы обеспечивают правильную инициализацию объекта и освобождение ресурсов после его использования.

Интерфейс представляет собой совокупность методов, которые класс предоставляет для взаимодействия с внешним миром. В коде примера публичные методы, такие как add, size, capacity, а также итераторы begin и end, формируют интерфейс класса. Интерфейс определяет, как объект может быть использован, а энкапсуляция скрывает детали его реализации.